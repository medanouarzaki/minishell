CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g
SRCS    = main.c builtins.c
OBJS    = $(SRCS:.c=.o)
NAME    = minishell

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
