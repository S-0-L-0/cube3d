# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: edforte <edforte@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/07 16:31:02 by edforte           #+#    #+#              #
#    Updated: 2025/05/19 20:16:17 by edforte          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d

CFILES = \
			main.c \
			

OBJ = $(CFILES:.c=.o)
CC = @ gcc
FLAGS = -g -Wall -Wextra -Werror
RM = rm -rf

%.o: %.c
	$(CC) -Wall -Wextra -Werror -I/usr/include -Imlx -c $< -o $@


name : $(NAME)

all : $(NAME)

$(NAME) : $(OBJ)
	@make -C ./mlx
	$(NAME): $(OBJ)
	$(CC) $(OBJ) -Lmlx -lmlx -framework OpenGL -framework AppKit -o $(NAME)

clean :
	@ $(RM) $(OBJ)

fclean : clean
	@ $(RM) $(NAME)

re : fclean all
