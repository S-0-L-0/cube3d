NAME = cub3d

CFILES = \
			src/main.c \
			src/game/game_loop.c \
			src/game/raycasting.c \
			src/game/movement.c \
			src/game/hooks.c \
			src/game/utils.c \
			src/game/2d.c \
			src/game/minimap.c \
			src/game/hud.c \
			src/game/door.c \
			src/game/hook_help.c \
			src/game/ray_help.c \
			src/game/dda.c \
			src/utils/utils.c \
			src/utils/more_utils.c \
			src/utils/get_next_lime/get_next_line.c\
			src/utils/get_next_lime/get_next_line_utils.c \
			src/parser/mlx_initialization.c \
			src/parser/mlx_setup.c \
			src/parser/file_operations.c \
			src/parser/file_content_reading.c \
			src/parser/file_reading_control.c \
			src/parser/line_parsing_utilities.c \
			src/parser/texture_type_management.c \
			src/parser/texture_path_processing.c \
			src/parser/color_type_management.c \
			src/parser/color_processing.c \
			src/parser/rgb_parsing.c \
			src/parser/configuration_validation.c \
			src/parser/texture_loading.c \
			src/parser/texture_management.c \
			src/parser/map_grid_operations.c \
			src/parser/map_parsing.c \
			src/parser/map_width_normalization.c \
			src/parser/map_width_processing.c \
			src/parser/map_boundary_detection.c \
			src/parser/player_data_management.c \
			src/parser/player_position_extraction.c \
			src/parser/map_validation.c \
			src/parser/2d_array_operations.c \
			src/parser/memory_management.c \
			src/parser/game_cleanup.c \

OBJ = $(CFILES:.c=.o)
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I./includes -I./mlx_linux
# INCLUDES = -I./includes -I./mlx_apple
LIBS = -L./mlx_linux -lmlx -lXext -lX11 -lm
# LIBS = -Lmlx -lmlx -framework OpenGL -framework AppKit -lm
RM = rm -f

# Directory MLX
MLX_DIR = ./mlx_linux
MLX_LIB = $(MLX_DIR)/libmlx.a

# Regola per compilare i file .o
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: all clean fclean re

all: $(MLX_LIB) $(NAME)

# Regola per compilare MLX - ora dipende dal file libmlx.a
$(MLX_LIB):
	@echo "Compiling MLX..."
	@if [ -d "$(MLX_DIR)" ]; then \
		cd $(MLX_DIR) && ./configure; \
	else \
		echo "Error: MLX directory $(MLX_DIR) not found!"; \
		exit 1; \
	fi

# Compila il programma dopo aver compilato MLX
$(NAME): $(OBJ) $(MLX_LIB)
	$(CC) $(OBJ) $(LIBS) -o $(NAME)

clean:
	$(RM) $(OBJ)
	@if [ -d "$(MLX_DIR)" ]; then \
		cd $(MLX_DIR) && ./configure clean; \
	fi

fclean: clean
	$(RM) $(NAME)

re: fclean all