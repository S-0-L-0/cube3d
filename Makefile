NAME = cub3d

CFILES = \
			src/main.c \
			src/parser/main_parser.c \
			src/parser/parser_cleanup.c \
			src/parser/parser_config.c \
			src/parser/parser_file.c \
			src/parser/parser_map_validation.c \
			src/parser/parser_map.c \
			src/parser/parser_utils.c \
			src/parser/parser_validation.c \
			src/game/game_loop.c \
			src/game/raycasting.c \
			src/game/movement.c \
			src/game/hooks.c \
			src/game/utils.c \
			src/game/2d.c \
			src/game/minimap.c \
			src/game/hud.c \
			src/game/door.c \
			src/utils/utils.c \
			src/utils/more_utils.c \
			src/utils/get_next_lime/get_next_line.c\
			src/utils/get_next_lime/get_next_line_utils.c \
			src/purser/mlx_initialization.c \
			src/purser/mlx_setup.c \
			src/purser/file_operations.c \
			src/purser/file_content_reading.c \
			src/purser/file_reading_control.c \
			src/purser/line_parsing_utilities.c \
			src/purser/texture_type_management.c \
			src/purser/texture_path_processing.c \
			src/purser/color_type_management.c \
			src/purser/color_processing.c \
			src/purser/rgb_parsing.c \
			src/purser/configuration_validation.c \
			src/purser/texture_loading.c \
			src/purser/texture_management.c \
			src/purser/map_grid_operations.c \
			src/purser/map_parsing.c \
			src/purser/map_width_normalization.c \
			src/purser/map_width_processing.c \
			src/purser/map_boundary_detection.c \
			src/purser/player_data_management.c \
			src/purser/player_position_extraction.c \
			src/purser/map_validation.c \
			src/purser/2d_array_operations.c \
			src/purser/memory_management.c \
			src/purser/game_cleanup.c \

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