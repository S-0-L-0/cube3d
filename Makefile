
NAME = cub3d

CFILES = \
			src/main.c \
			src/parsing/file_check.c \
			src/parsing/init_struct.c \
			src/parsing/map_check.c \
			src/parsing/rgb_check.c \
			src/parsing/texture_check.c \
			src/parsing/parse_map.c \
			src/game/game_loop.c \
			src/game/raycasting.c \
			src/utils/get_next_line.c \

OBJ = $(CFILES:.c=.o)
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I./includes -I./mlx_linux
# INCLUDES = -I./includes -I./mlx_apple
LIBS = -L./mlx_linux -lmlx -lXext -lX11 -lm
# LIBS = -Lmlx -lmlx -framework OpenGL -framework AppKit -lm
RM = rm -f

# Regola per compilare i file .o
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: all clean fclean re

all: $(NAME)

# Compila MLX prima, poi il programma
$(NAME): $(OBJ)
	#@make -C ./mlx_apple
	$(CC) $(OBJ) $(LIBS) -o $(NAME)

clean:
	$(RM) $(OBJ)


fclean: clean
	$(RM) $(NAME)


re: fclean all
