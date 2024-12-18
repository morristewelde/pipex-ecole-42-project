# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/18 00:01:51 by mtewelde          #+#    #+#              #
#    Updated: 2024/12/02 01:05:56 by mtewelde         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = pipex
NAME_BONUS  = pipex_bonus

SRC         = pipex.c\
              src/ft_init.c src/ft_split.c src/utils.c src/pipex_utils.c

SRCB        = bonus.c bonus/get_next_line.c bonus/bonus_init.c\
              src/ft_split.c src/utils.c src/pipex_utils.c\

CC          = cc
CFLAGS      = -Wall -Werror -Wextra -g

OBJS        = $(SRC:.c=.o)
OBJSB       = $(SRCB:.c=.o)

RM          = rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJSB)
	$(CC) $(CFLAGS) $(OBJSB) -o $(NAME_BONUS)

clean:
	$(RM) $(OBJS) $(OBJSB)

fclean: clean
	$(RM) $(NAME) $(NAME_BONUS)

re: fclean all bonus

.PHONY: all bonus clean fclean re
