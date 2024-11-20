# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mtewelde <mtewelde@student.42lehavre.fr    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/18 00:01:51 by mtewelde          #+#    #+#              #
#    Updated: 2024/11/20 23:19:59 by mtewelde         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= pipex

SRC			= pipex.c\ 
				src/ft_init.c src/ft_split.c src/utils.c 

CC 			= cc

CFLAGS      = -Wall -Werror -Wextra -g

OBJS 		= $(SRC:.c=.o)

RM 			= rm -rf


all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all