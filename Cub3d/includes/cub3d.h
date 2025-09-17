/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:05:34 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 16:17:44 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <math.h>
# include <stdbool.h>
# include "../mlx_linux/mlx.h"

# define BONUS 1
# define TILE_SIZE 12
# define VIEW_RADIUS 5
# define MINIMAP_DIM 11

typedef struct s_parse_data
{
	char	**file_content;
	int		map_start_line;
	int		texture_count;
	int		floor_set;
	int		ceiling_set;
	int		north_loaded;
	int		south_loaded;
	int		east_loaded;
	int		west_loaded;
}	t_parse_data;

typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
	char	*north_texture;
	char	*south_texture;
	char	*west_texture;
	char	*east_texture;
	int		floor_col[3];
	int		sky_col[3];
}	t_map;

typedef struct s_player
{
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	char	direction;
}	t_player;

typedef struct s_ray
{
	double	camera_x;
	double	dir_x;
	double	dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	perp_wall_dist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	int		door;
	double	intersection_x;
	double	intersection_y;
}	t_ray;

typedef struct s_texture
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}	t_texture;

typedef struct s_mlx
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		win_width;
	int		win_height;
}	t_mlx;

typedef struct s_time
{
	double	time;
	double	old_time;
	double	frame_time;
	double	start_time;
}	t_time;

typedef struct s_keys
{
	bool	w;
	bool	a;
	bool	s;
	bool	d;
	bool	left;
	bool	right;
	int		last_mouse_x;
}	t_keys;

typedef struct s_render
{
	int			line_height;
	int			draw_start;
	int			draw_end;
	int			texture_x;
	double		wall_x;
	t_texture	*texture;
}	t_render;

typedef struct s_game
{
	t_map		map;
	t_player	player;
	t_mlx		mlx;
	t_ray		ray;
	t_texture	textures[11];
	t_keys		keys;
	t_time		time;
	t_render	render;
}	t_game;

// Main functions

int				validate_and_initialize_mlx_struct(t_mlx *mlx);
int				create_mlx_connection(t_mlx *mlx);
int				create_window(t_mlx *mlx);
int				create_image(t_mlx *mlx);
int				setup_image_data(t_mlx *mlx);
int				init_mlx(t_mlx *mlx);
char			*get_next_line(int fd);
int				parser(int argc, char **argv, t_game *game,
					t_parse_data *parse);
int				init_texture(t_texture *texture, char *texture_path, void *mlx);
char			*get_texture_path(t_game *game, int index);
char			*get_error_texture_name(int index);
void			cleanup_loaded_textures(t_game *game, int loaded_count);
int				load_single_texture(t_game *game, int index);
int				set_textures(t_game *game);

// parser_validation.c
int				validate_arguments(int argc, char **argv);
int				open_and_validate_file(char *filepath);
int				file_exists(char *path);
int				validate_texture_file(char *path, char *texture_name);

// parser_file.c
int				allocate_file_content(t_parse_data *parse);
char			*remove_newline(char *line);
int				handle_file_too_large(char *line, t_parse_data *parse,
					int line_count);
int				read_lines_from_file(int fd, t_parse_data *parse);
int				read_file_content(int fd, t_parse_data *parse);
int				is_empty_line(char *line);
int				is_map_line(char *line);

// parser_config.c
int				process_config_line(t_parse_data *parse, t_map *map,
					int line_index);
int				parse_config_elements(t_parse_data *parse, t_map *map);
int				get_texture_type_index(char *line);
char			*get_texture_identifier(int type_index);
char			**get_texture_destination(int type_index, t_map *map);
int				*get_loaded_flag(int type_index, t_parse_data *parse);
char			*extract_and_clean_path(char *path_start, char *identifier);
int				store_texture_path(char *line, int type_index, t_map *map,
					t_parse_data *parse);
int				parse_texture_line(char *line, t_map *map, t_parse_data *parse);
int				get_color_type_index(char *line);
char			*get_color_identifier(int type_index);
int				*get_color_destination(int type_index, t_map *map);
int				*get_color_loaded_flag(int type_index, t_parse_data *parse);
int				check_duplicate_color(int type_index, t_parse_data *parse);
int				process_color_values(char *color_values, int type_index,
					t_map *map, t_parse_data *parse);
int				parse_color_line(char *line, t_map *map, t_parse_data *parse);
int				validate_config_vertical(int error, t_map *map);
int				validate_config_horizontal(int error, t_map *map);
int				validate_config_complete(t_map *map, t_parse_data *parse);
int				parse_single_rgb_value(char *str, int *pos);
int				expect_comma(char *str, int *pos);
int				extract_rgb_values(char *str, int *rgb);

// parse_map.c
int				validate_content_after_map(t_parse_data *parse, int map_end);
int				allocate_map_grid(t_map *map, int height);
void			cleanup_map_grid(t_map *map);
int				copy_map_rows(t_parse_data *parse, t_map *map, int map_start);
int				parse_map(t_parse_data *parse, t_map *map, t_player *player);
int				find_max_width(t_map *map);
int				validate_map_size(t_map *map);
char			*create_padded_row(char *original_row, int current_len,
					int max_width);
int				pad_all_rows(t_map *map, int max_width);
int				normalize_map_width(t_map *map);
int				validate_map_start(t_parse_data *parse);
int				scan_map_end(t_parse_data *parse, int start, int *end);
int				validate_map_height(int map_start, int map_end);
int				find_map_boundaries(t_parse_data *parse, int *map_start,
					int *map_end);
void			initialize_player_data(t_player *player);
int				handle_multiple_players(t_player *player, int i, int j);
void			set_player_direction_vectors(t_player *player, char direction);
int				process_player_found(t_map *map, t_player *player, int i,
					int j);
int				scan_for_player(t_map *map, t_player *player);
int				validate_player_position(t_player *player, t_map *map);
int				extract_player_position(t_map *map, t_player *player);

// parser_map_validation.c
int				validate_map(t_map *map, t_player *player);
int				validate_map_characters(t_map *map);
int				flood_fill_check(t_map *map, t_player *player);
int				flood_fill_recursive(t_map *map, int **visited, int x, int y);

// parser_utils.c
int				validate_dimensions(int height, int width);
int				**allocate_rows_array(int height);
void			cleanup_partial_array(int **array, int allocated_rows);
int				*allocate_and_initialize_row(int width, int **array,
					int row_index);
int				**allocate_2d_int_array(int height, int width);
void			free_2d_int_array(int **array, int height);
void			free_split(char **split);
int				is_player_character(char c);

// parser_cleanup.c
void			cleanup_parse_data(t_parse_data *parse);
void			free_game_texture(t_game *game);
void			reset_player_pos(t_game *game);
void			cleanup_game(t_game *game);
void			free_map_grid(t_map *map);

// init_struct.c (funzioni effettivamente utilizzate)
int				init_texture(t_texture *texture, char *texture_path, void *mlx);
void			free_mlx(t_mlx *mlx);
void			free_game(t_game *game);

// game_loop.c
int				game_loop(t_game *game);

// raycasting.c
void			raycasting(t_game *game, int screen_x);
void			draw_map_2d(t_game *game);
void			draw_player_2d(t_game *game);
void			draw_minimap(t_game *game);

// movement.c
void			update_player(t_game *game);
void			rot_player(t_game *game, double speed);
void			move_player(t_game *game, double dir_x, double dir_y);

// hooks.c
int				mouse_hook(int x, int y, t_game *game);
int				key_press(int keycode, t_game *game);
int				key_release(int keycode, t_game *game);
int				close_window(t_game *game);
void			handle_move(int keycode, t_game *game);
void			handle_arrows(int keycode, t_game *game);
void			handle_actions(int keycode, t_game *game);

// utils.c
long long		get_time_ms(void);
unsigned int	get_pixel_color(t_texture *texture, int x, int y);
void			put_pixel(t_game *game, int x, int y, int color);
void			draw_circle(t_game *game, int cx, int cy, int radius);
void			*ft_memset(void *b, int c, size_t len);
char			*ft_strdup(const char *s1);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				ft_strlen(const char *s);
void			free_texture_struct(t_texture *texture, void *mlx);
void			free_map(t_map *map);

// minimap.c
void			draw_minimap(t_game *game);
void			draw_border(t_game *game);

// hud.c
void			draw_crosshair(t_game *game);
void			draw_torch(t_game *game);

// door.c
void			toggle_door(t_game *game);
void			check_door(t_game *game);

// ray_help.c
void			calculate_perp_dist(t_game *game, int side);
void			init_render_params(t_game *game);
void			init_steps(t_game *game);
void			init_ray(t_game *game, int screen_x);

// dda.c
int				dda(t_game *game);

#endif
