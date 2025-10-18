CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lm
NAME = ft_ping
SRC = ft_ping.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)
	@sudo setcap cap_net_raw+ep $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re