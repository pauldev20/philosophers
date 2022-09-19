# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pgeeser <pgeeser@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/02 22:40:40 by pgeeser           #+#    #+#              #
#    Updated: 2022/09/20 01:43:47 by pgeeser          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Main
CFLAGS = -Wall -Werror -Wextra #-fsanitize=address,undefined
LDFLAGS =
INCLUDES =
CC = cc
OBJDIR = objs

# Mandatory
NAME = philo
SRCDIR = .
SRCFILES = main.c utils.c lib.c checkers.c philo.c
OBJS = $(addprefix $(OBJDIR)/, $(SRCFILES:.c=.o))
SRCS = $(addprefix $(SRCDIR)/, $(SRCFILES))

# # Bonus
# BONUS_NAME = philo
# BONUS_SRCDIR = .
# BONUS_SRCFILES = 
# BONUS_OBJS = $(addprefix $(OBJDIR)/, $(BONUS_SRCFILES:.c=.o))
# BONUS_SRCS = $(addprefix $(SRCDIR)/, $(BONUS_SRCFILES))

# # Bonus
# $(OBJDIR)/%.o: $(BONUS_SRCDIR)/%.c
# 	@mkdir -p $(@D)
# 	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

# bonus: $(BONUS_OBJS)
# 	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) $(BONUS_OBJS) -o $(BONUS_NAME)

# Mandatory
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
	rm -rf $(BONUS_OBJS)

fclean: clean
	rm -rf $(NAME)
	rm -rf $(BONUS_NAME)

re: fclean
	$(MAKE) all

norm:
	norminette $(SRCS) *.h

.PHONY:	all clean fclean re norm debug
