# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pgeeser <pgeeser@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/02 22:40:40 by pgeeser           #+#    #+#              #
#    Updated: 2022/09/13 17:16:57 by pgeeser          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CFLAGS = -Wall -Werror -Wextra -fsanitize=address,undefined
LDFLAGS =
INCLUDES =
CC = cc

OBJDIR = objs
SRCDIR = .
SRCFILES = main.c utils.c lib.c checkers.c philo.c
OBJS = $(addprefix $(OBJDIR)/, $(SRCFILES:.c=.o))
SRCS = $(addprefix $(SRCDIR)/, $(SRCFILES))

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) $(OBJS) -o $(NAME)
	
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
