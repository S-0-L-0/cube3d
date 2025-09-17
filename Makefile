NAME = cub3d

CFILES = \
			Cub3d/src/main.c \
			Cub3d/src/game/game_loop.c \
			Cub3d/src/game/raycasting.c \
			Cub3d/src/game/movement.c \
			Cub3d/src/game/hooks.c \
			Cub3d/src/game/utils.c \
			Cub3d/src/game/2d.c \
			Cub3d/src/game/minimap.c \
			Cub3d/src/game/hud.c \
			Cub3d/src/game/door.c \
			Cub3d/src/game/hook_help.c \
			Cub3d/src/game/ray_help.c \
			Cub3d/src/game/dda.c \
			Cub3d/src/utils/utils.c \
			Cub3d/src/utils/more_utils.c \
			Cub3d/src/utils/get_next_line/get_next_line.c\
			Cub3d/src/utils/get_next_line/get_next_line_utils.c \
			Cub3d/src/parser/mlx_initialization.c \
			Cub3d/src/parser/mlx_setup.c \
			Cub3d/src/parser/file_operations.c \
			Cub3d/src/parser/file_content_reading.c \
			Cub3d/src/parser/file_reading_control.c \
			Cub3d/src/parser/line_parsing_utilities.c \
			Cub3d/src/parser/texture_type_management.c \
			Cub3d/src/parser/texture_path_processing.c \
			Cub3d/src/parser/color_type_management.c \
			Cub3d/src/parser/color_processing.c \
			Cub3d/src/parser/rgb_parsing.c \
			Cub3d/src/parser/configuration_validation.c \
			Cub3d/src/parser/texture_loading.c \
			Cub3d/src/parser/texture_management.c \
			Cub3d/src/parser/map_grid_operations.c \
			Cub3d/src/parser/map_parsing.c \
			Cub3d/src/parser/map_width_normalization.c \
			Cub3d/src/parser/map_width_processing.c \
			Cub3d/src/parser/map_boundary_detection.c \
			Cub3d/src/parser/player_data_management.c \
			Cub3d/src/parser/player_position_extraction.c \
			Cub3d/src/parser/map_validation.c \
			Cub3d/src/parser/2d_array_operations.c \
			Cub3d/src/parser/memory_management.c \
			Cub3d/src/parser/game_cleanup.c \

# BONUS_FILE =  \
# 			Bonus/src/main.c \
# 			Bonus/src/game/game_loop.c \
# 			Bonus/src/game/raycasting.c \
# 			Bonus/src/game/movement.c \
# 			Bonus/src/game/hooks.c \
# 			Bonus/src/game/utils.c \
# 			Bonus/src/game/2d.c \
# 			Bonus/src/game/minimap.c \
# 			Bonus/src/game/hud.c \
# 			Bonus/src/game/door.c \
# 			Bonus/src/game/hook_help.c \
# 			Bonus/src/game/ray_help.c \
# 			Bonus/src/game/dda.c \
# 			Bonus/src/utils/utils.c \
# 			Bonus/src/utils/more_utils.c \
# 			Bonus/src/utils/get_next_line/get_next_line.c\
# 			Bonus/src/utils/get_next_line/get_next_line_utils.c \
# 			Bonus/src/parser/mlx_initialization.c \
# 			Bonus/src/parser/mlx_setup.c \
# 			Bonus/src/parser/file_operations.c \
# 			Bonus/src/parser/file_content_reading.c \
# 			Bonus/src/parser/file_reading_control.c \
# 			Bonus/src/parser/line_parsing_utilities.c \
# 			Bonus/src/parser/texture_type_management.c \
# 			Bonus/src/parser/texture_path_processing.c \
# 			Bonus/src/parser/color_type_management.c \
# 			Bonus/src/parser/color_processing.c \
# 			Bonus/src/parser/rgb_parsing.c \
# 			Bonus/src/parser/configuration_validation.c \
# 			Bonus/src/parser/texture_loading.c \
# 			Bonus/src/parser/texture_management.c \
# 			Bonus/src/parser/map_grid_operations.c \
# 			Bonus/src/parser/map_parsing.c \
# 			Bonus/src/parser/map_width_normalization.c \
# 			Bonus/src/parser/map_width_processing.c \
# 			Bonus/src/parser/map_boundary_detection.c \
# 			Bonus/src/parser/player_data_management.c \
# 			Bonus/src/parser/player_position_extraction.c \
# 			Bonus/src/parser/map_validation.c \
# 			Bonus/src/parser/2d_array_operations.c \
# 			Bonus/src/parser/memory_management.c \
# 			Bonus/src/parser/game_cleanup.c \

OBJ = $(CFILES:.c=.o)
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I./includes -I./mlx_linux

LIBS = -L./mlx_linux -lmlx -lXext -lX11 -lm

RM = rm -f

MLX_DIR = ./mlx_linux
MLX_LIB = $(MLX_DIR)/libmlx.a

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

# bonus:
# 	BONUS_OBJ = $(BONUS_FILE:.c=.o)
# 	$(NAME): $(BONUS_OBJ) $(MLX_LIB)
# 		$(CC) $(BONUS_OBJ) $(LIBS) -o $(NAME)

clean:
	$(RM) $(OBJ)
	@if [ -d "$(MLX_DIR)" ]; then \
		cd $(MLX_DIR) && ./configure clean; \
	fi

fclean: clean
	$(RM) $(NAME)

re: fclean all