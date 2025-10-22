#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>


static char *query_dns(const char *hostname) {
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(hostname, NULL, &hints, &result) != 0) {
        fprintf(stderr, "ft_traceroute: %s: Name or service not known\n", hostname);
        exit(EXIT_FAILURE);
    }
    char *ipstr = malloc(INET_ADDRSTRLEN);
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)result->ai_addr;
    strcpy(ipstr, inet_ntoa(ipv4->sin_addr));
    freeaddrinfo(result);
    return ipstr;
}

static unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

static void ft_traceroute(const char *ip_addr, const char *hostname) {
    enum { STATUS_TIMEOUT = 0, STATUS_INTERMEDIATE = 1, STATUS_REACHED = 2 };
    const size_t payload_len = 60;
    int sockfd;
    struct sockaddr_in addr_con;
    int addrlen;
    struct timeval tv;
    uint16_t id;
    unsigned char buffer[1024];
    struct icmphdr icmp_template;
    char payload[60] = "hello";
    int reached = 0;
    if (!hostname)
        hostname = ip_addr;
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        fprintf(stderr, "Socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    tv.tv_sec = 0;
    tv.tv_usec = 800000;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv)) < 0) {
        fprintf(stderr, "Setting socket options failed\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    addr_con.sin_family = AF_INET;
    addr_con.sin_port = htons(0);
    addr_con.sin_addr.s_addr = inet_addr(ip_addr);
    addrlen = sizeof(addr_con);

    id = getpid() & 0xFFFF;
    memset(&icmp_template, 0, sizeof(icmp_template));
    icmp_template.type = ICMP_ECHO;
    icmp_template.code = 0;
    icmp_template.un.echo.id = htons(id);

    printf("ft_traceroute to %s (%s), %d hops max, %zu byte packets\n",
           hostname, ip_addr, 30, payload_len);

    for (int ttl = 1; ttl <= 30 && !reached; ++ttl) {
        int identity_printed = 0;
        printf("%2d  ", ttl);
        fflush(stdout);
        for (int probe = 0; probe < 3 &&  !reached; ++probe) 
        {
            struct icmphdr icmp_packet;
            char packet[sizeof(struct icmphdr) + payload_len];
            struct timespec time_start;
            struct timespec time_end;
            struct sockaddr_in sender;
            socklen_t sender_len;
            ssize_t bytes;
            int status = STATUS_TIMEOUT;
            double rtt_ms = 0.0;

            if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
                printf(" *");
                continue;
            }
            memcpy(&icmp_packet, &icmp_template, sizeof(icmp_packet));
            icmp_packet.checksum = 0;
            memcpy(packet, &icmp_packet, sizeof(icmp_packet));
            memcpy(packet + sizeof(icmp_packet), payload, payload_len);
            ((struct icmphdr *)packet)->checksum = checksum(packet, sizeof(packet));

            clock_gettime(CLOCK_MONOTONIC, &time_start);
            if (sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&addr_con, addrlen) < 0) {
                printf(" *");
                continue;
            }
            sender_len = sizeof(sender);
            while (1337) {
                bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&sender, &sender_len);
                if (bytes < 0) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                        break;
                    if (errno == EINTR)
                        continue;
                    perror("recvfrom");
                    break;
                }

                if (bytes < (ssize_t)(sizeof(struct iphdr) + sizeof(struct icmphdr))) continue;

                clock_gettime(CLOCK_MONOTONIC, &time_end);

                struct iphdr *outer_ip = (struct iphdr *)buffer;
                int outer_header_len = outer_ip->ihl * 4;
                if (bytes < outer_header_len + (int)sizeof(struct icmphdr)) continue;
                struct icmphdr *outer_icmp = (struct icmphdr *)(buffer + outer_header_len);
                if (outer_icmp->type == ICMP_ECHOREPLY) {
                    if (outer_icmp->un.echo.id != htons(id)) continue;
                    status = STATUS_REACHED;
                } 
                else if (outer_icmp->type == ICMP_TIME_EXCEEDED || outer_icmp->type == ICMP_DEST_UNREACH) 
                {
                    size_t needed = outer_header_len + sizeof(struct icmphdr) + sizeof(struct iphdr);
                    if ((size_t)bytes < needed)
                        continue;
                    struct iphdr *inner_ip = (struct iphdr *)(buffer + outer_header_len + sizeof(struct icmphdr));
                    int inner_len = inner_ip->ihl * 4;
                    if ((size_t)bytes < outer_header_len + sizeof(struct icmphdr) + inner_len + sizeof(struct icmphdr))
                        continue;
                    struct icmphdr *inner_icmp = (struct icmphdr *)((unsigned char *)inner_ip + inner_len);
                    if (inner_icmp->un.echo.id != htons(id))
                        continue;
                    status = STATUS_INTERMEDIATE;
                } 
                else continue;

                rtt_ms = (time_end.tv_sec - time_start.tv_sec) * 1000.0 +
                         (time_end.tv_nsec - time_start.tv_nsec) / 1000000.0;

                if (!identity_printed) {
                    char host_buf[NI_MAXHOST];
                    char addr_buf[INET_ADDRSTRLEN];
                    int res = getnameinfo((struct sockaddr *)&sender, sizeof(sender), host_buf, sizeof(host_buf), NULL, 0, NI_NAMEREQD);
                    const char *check = inet_ntoa(sender.sin_addr);
                    if (check) {
                        strncpy(addr_buf, check, sizeof(addr_buf));
                        addr_buf[sizeof(addr_buf) - 1] = '\0';
                    } else {
                        addr_buf[0] = '\0';
                    }
                    if (res == 0)
                        printf("%s ", host_buf);
                    else
                        printf("%s ", addr_buf);
                    if (check)
                        printf("(%s) ", addr_buf);
                    identity_printed = 1;
                }
                printf(" %.3f ms", rtt_ms);
                break;
            }

            if (status == STATUS_REACHED) reached = 1;
            if (status == STATUS_TIMEOUT) printf(" *");
        }
        printf("\n");
    }

    close(sockfd);
}


void print_usage(const char *prog_name) {
    printf("Usage: \n");
    printf("  %s <destination>\n", prog_name);
    printf("Options:\n");
    printf("  ––help       Show this help message and exit\n");
    exit(EXIT_SUCCESS);
}

void parse_args(int argc, char **argv) 
{
    bool found = false;
    if (argc < 2) {
        print_usage(argv[0]);
    }
    for (int i = 1; i < argc; i++) 
    {
        if (strcmp(argv[i], "--help") == 0) print_usage(argv[0]);
        else if (!found) found = true;
        else {
            fprintf(stderr, "ft_traceroute: too many arguments\n");
            print_usage(argv[0]);
        }
    }
}

int	main(int argc, char **argv)
{
    char *ip_addr;
    char *hostname = NULL;
    parse_args(argc, argv);
    hostname = argv[1];
    ip_addr = query_dns(hostname);
    ft_traceroute(ip_addr, hostname);
    free(ip_addr);
    return 0;
}
