# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pgeeser <pgeeser@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/02 22:40:40 by pgeeser           #+#    #+#              #
#    Updated: 2022/09/13 13:41:32 by pgeeser          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CFLAGS = -Wall -Werror -Wextra
CC = gcc

SRCS = main.c utils.c lib.c
OBJS = $(SRCS:.c=.o)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -L. $(OBJS) -o $(NAME) -fsanitize=address,undefined
	
all: $(NAME)

debug:
	$(MAKE) CFLAGS='-g' re

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean
	$(MAKE) all

norm:
	norminette $(SRCS) *.h

.PHONY:	all clean fclean re norm debug