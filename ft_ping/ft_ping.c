#define _POSIX_C_SOURCE 200809L

#include <arpa/inet.h>
#include <errno.h>
#include <math.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif

#ifndef NI_NAMEREQD
#define NI_NAMEREQD 0x04
#endif


enum
{
    PING_PAYLOAD_SIZE = 56,
    PING_BUFFER_SIZE = 1024
};

typedef struct {
    int running;
    unsigned long transmitted;
    unsigned long received;
    double rtt_min;
    double rtt_max;
    double rtt_sum;
    double rtt_sq_sum;
    struct timespec start_time;
    bool verbose;
    char ip[INET_ADDRSTRLEN];
    bool resolve_hostname;
    char hostname[NI_MAXHOST];
    bool fqdn_cached;
} ping_stats_t;

typedef struct {
    int sockfd;
    struct sockaddr_in destination;
    socklen_t dest_len;
    uint16_t identifier;
    uint8_t packet[sizeof(struct icmphdr) + PING_PAYLOAD_SIZE];
    size_t packet_len;
} ping_session_t;

typedef struct {
    struct sockaddr_in sender;
    ssize_t data_len;
    int ttl;
    uint16_t sequence;
} ping_reply_t;

static ping_stats_t g_stats = {
    .running = 1,
    .rtt_min = INFINITY,
    .rtt_max = 0.0,
    .verbose = false,
    .resolve_hostname = false,
    .fqdn_cached = false,
};

static void print_usage(const char *program)
{
    printf("Usage: %s <IP> [-v] [-?]\n", program);
    printf("  Parameters:\n");
    printf("    IP  : IP address\n");
    printf("    -v  : Verbose mode (optional)\n");
    printf("    -?  : Display this help message\n");
    exit(EXIT_FAILURE);
}


static double total_time(struct timespec start)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (now.tv_sec - start.tv_sec) * 1000.0 +
           (now.tv_nsec - start.tv_nsec) / 1000000.0;
}

static double elapsed_ms(const struct timespec *start, const struct timespec *end)
{
    return (end->tv_sec - start->tv_sec) * 1000.0 +
           (end->tv_nsec - start->tv_nsec) / 1000000.0;
}

static void update_rtt(double rtt)
{
    if (rtt < g_stats.rtt_min)
        g_stats.rtt_min = rtt;
    if (rtt > g_stats.rtt_max)
        g_stats.rtt_max = rtt;
    g_stats.rtt_sum += rtt;
    g_stats.rtt_sq_sum += rtt * rtt;
}

static void reset_run_stats(const char *ip_addr)
{
    g_stats.running = 1;
    g_stats.transmitted = 0;
    g_stats.received = 0;
    g_stats.rtt_min = INFINITY;
    g_stats.rtt_max = 0.0;
    g_stats.rtt_sum = 0.0;
    g_stats.rtt_sq_sum = 0.0;
    g_stats.fqdn_cached = false;
    g_stats.hostname[0] = '\0';
    strncpy(g_stats.ip,  ip_addr, sizeof(g_stats.ip) - 1);
    clock_gettime(CLOCK_MONOTONIC, &g_stats.start_time);
}

static void handle_sigint(int signo)
{
    (void)signo;

    double total_ms = total_time(g_stats.start_time);
    double loss = 0.0;
    if (g_stats.transmitted > 0)
        loss = 100.0 * (g_stats.transmitted - g_stats.received) / g_stats.transmitted;

    double avg = g_stats.received ? g_stats.rtt_sum / g_stats.received : 0.0;
    double mdev = 0.0;
    if (g_stats.received) {
        double mean_sq = g_stats.rtt_sq_sum / g_stats.received;
        double var = mean_sq - avg * avg;
        if (var > 0.0)
            mdev = sqrt(var);
    }

    printf("\n--- %s ft_ping statistics ---\n", g_stats.ip);
    printf("%lu packets transmitted, %lu received, %.0f%% packet loss, time %.0fms\n",
           g_stats.transmitted, g_stats.received, loss, total_ms);
    if (g_stats.received) {
        printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
               g_stats.rtt_min == INFINITY ? 0.0 : g_stats.rtt_min,
               avg,
               g_stats.rtt_max,
               mdev);
    }
    g_stats.running = 0;
}

static unsigned short checksum(void *buffer, int len)
{
    unsigned short *buf = buffer;
    unsigned int sum = 0;

    for (; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return (unsigned short)~sum;
}

static void get_cache_hostname(const struct sockaddr_in *sender, const char *sender_ip)
{
    if (!g_stats.resolve_hostname || g_stats.fqdn_cached)
        return;

    if (getnameinfo((const struct sockaddr *)sender, sizeof(*sender),
                    g_stats.hostname, sizeof(g_stats.hostname),
                    NULL, 0, NI_NAMEREQD) == 0) {
        g_stats.fqdn_cached = true;
    } else {
        g_stats.resolve_hostname = false;
        g_stats.hostname[0] = '\0';
        (void)sender_ip;
    }
}

static void print_ping_reply(size_t data_len, const struct sockaddr_in *sender, uint16_t seq, int ttl, double rtt)
{
    char sender_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &sender->sin_addr, sender_ip, sizeof(sender_ip));

    get_cache_hostname(sender, sender_ip);

    if (g_stats.resolve_hostname && g_stats.fqdn_cached) {
        printf("%zu bytes from %s (%s): icmp_seq=%u ttl=%d time=%.2f ms\n",
               data_len, g_stats.hostname, sender_ip, seq, ttl, rtt);
    } else {
        printf("%zu bytes from %s: icmp_seq=%u ttl=%d time=%.2f ms\n",
               data_len, sender_ip, seq, ttl, rtt);
    }
}

static void ft_verbose(int sockfd, const char *hostname)
{
    if (!g_stats.verbose)
        return;

    printf("ft_ping: sock.fd: %d (socktype: SOCK_RAW), family: AF_INET\n\n", sockfd);
    printf("ai->ai_family: AF_INET, ai->ai_canonname: '%s'\n", hostname ? hostname : "");
}

static int create_icmp_socket(void)
{
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

static void configure_socket_timeout(int sockfd)
{
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv)) < 0) {
        perror("setsockopt failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

static void prepare_destination(struct sockaddr_in *destination, const char *ip_addr)
{
    memset(destination, 0, sizeof(*destination));
    destination->sin_family = AF_INET;
    destination->sin_port = htons(1);
    destination->sin_addr.s_addr = inet_addr(ip_addr);
}

static void prepare_packet_template(ping_session_t *session)
{
    memset(session->packet, 0, sizeof(session->packet));
    session->packet_len = sizeof(session->packet);

    struct icmphdr *hdr = (struct icmphdr *)session->packet;
    hdr->type = ICMP_ECHO;
    hdr->code = 0;
    hdr->un.echo.id = htons(session->identifier);
    hdr->un.echo.sequence = 0;

    memcpy(session->packet + sizeof(struct icmphdr), "hello", 6);
}

static void prepare_packet_sequence(ping_session_t *session, uint16_t sequence)
{
    struct icmphdr *hdr = (struct icmphdr *)session->packet;
    hdr->un.echo.sequence = htons(sequence);
    hdr->checksum = 0;
    hdr->checksum = checksum(session->packet, (int)session->packet_len);
}

static bool wait_for_reply(ping_session_t *session, uint16_t sequence, struct timespec *recv_time, ping_reply_t *reply)
{
    unsigned char buffer[PING_BUFFER_SIZE];

    while (g_stats.running) {
        socklen_t sender_len = sizeof(reply->sender);
        ssize_t bytes = recvfrom(session->sockfd, buffer, sizeof(buffer), 0,
                                 (struct sockaddr *)&reply->sender, &sender_len);
        if (bytes <= 0) {
            if (bytes < 0 && errno == EINTR)
                continue;
            if (bytes < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
                return false;
            if (bytes == 0)
                continue;
            if (bytes < 0)
                continue;
        }

        struct iphdr *ip_hdr = (struct iphdr *)buffer;
        int header_len = ip_hdr->ihl * 4;
        if (bytes < header_len + (int)sizeof(struct icmphdr))
            continue;

        struct icmphdr *icmp_hdr = (struct icmphdr *)(buffer + header_len);
        if (icmp_hdr->type != ICMP_ECHOREPLY)
            continue;

        struct icmphdr *sent_hdr = (struct icmphdr *)session->packet;
        if (icmp_hdr->un.echo.id != sent_hdr->un.echo.id ||
            ntohs(icmp_hdr->un.echo.sequence) != sequence)
            continue;

        clock_gettime(CLOCK_MONOTONIC, recv_time);
        reply->data_len = bytes - header_len;
        reply->ttl = ip_hdr->ttl;
        reply->sequence = ntohs(icmp_hdr->un.echo.sequence);
        return true;
    }

    return false;
}

static void ft_ping(const char *ip_addr, const char *hostname)
{
    ping_session_t session;

    session.sockfd = create_icmp_socket();
    configure_socket_timeout(session.sockfd);
    ft_verbose(session.sockfd, hostname);

    prepare_destination(&session.destination, ip_addr);
    session.dest_len = sizeof(session.destination);
    session.identifier = (uint16_t)(getpid() & 0xFFFF);
    prepare_packet_template(&session);

    reset_run_stats(ip_addr);

    printf("FT_PING %s (%s) %d(%zu) bytes of data.\n",
           hostname ? hostname : ip_addr,
           ip_addr,
           PING_PAYLOAD_SIZE,
           session.packet_len + sizeof(struct iphdr));

    uint16_t sequence = 0;

    while (g_stats.running) {
        sequence++;
        g_stats.transmitted++;
        prepare_packet_sequence(&session, sequence);

        struct timespec send_time;
        clock_gettime(CLOCK_MONOTONIC, &send_time);

        if (sendto(session.sockfd, session.packet, session.packet_len, 0,
                   (struct sockaddr *)&session.destination, session.dest_len) <= 0) {
            continue;
        }

        struct timespec recv_time;
        ping_reply_t reply;
        bool got_reply = wait_for_reply(&session, sequence, &recv_time, &reply);

        if (!g_stats.running)
            break;
        if (!got_reply)
            continue;

        g_stats.received++;
        double rtt = elapsed_ms(&send_time, &recv_time);
        update_rtt(rtt);
        print_ping_reply((size_t)reply.data_len, &reply.sender, reply.sequence, reply.ttl, rtt);
        sleep(1);
    }

    close(session.sockfd);
}

static char *query_dns(const char *hostname)
{
    struct addrinfo hints;
    struct addrinfo *result = NULL;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int err = getaddrinfo(hostname, NULL, &hints, &result);
    if (err != 0) {
        fprintf(stderr, "ft_ping: %s: Name or service not known\n", hostname);
        exit(EXIT_FAILURE);
    }

    char *ipstr = malloc(INET_ADDRSTRLEN);
    if (!ipstr) {
        perror("malloc");
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in *ipv4 = (struct sockaddr_in *)result->ai_addr;
    inet_ntop(result->ai_family, &(ipv4->sin_addr), ipstr, INET_ADDRSTRLEN);
    freeaddrinfo(result);
    return ipstr;
}

static char *parse_args(int argc, char *argv[])
{
    char *destination = NULL;

    if (argc < 2)
        print_usage(argv[0]);

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-?") == 0) {
            print_usage(argv[0]);
        } else if (strcmp(argv[i], "-v") == 0) {
            g_stats.verbose = true;
        } else {
            destination = argv[i];
        }
    }

    if (destination == NULL) {
        fprintf(stderr, "ft_ping: usage error: Destination address required\n");
        exit(EXIT_FAILURE);
    }

    return destination;
}

int main(int argc, char *argv[])
{
    signal(SIGINT, handle_sigint);

    char *dest = parse_args(argc, argv);
    char *ip = query_dns(dest);

    if (strcmp(dest, ip) != 0)
        g_stats.resolve_hostname = true;

    ft_ping(ip, dest);
    free(ip);
    return 0;
}
