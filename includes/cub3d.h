/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:05:34 by edforte           #+#    #+#             */
/*   Updated: 2025/09/15 17:24:15 by edforte          ###   ########.fr       */
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
# define DEBUG 0

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

typedef struct s_game
{
	t_map		map;
	t_player	player;
	t_mlx		mlx;
	t_ray		ray;
	t_texture	textures[10];
	t_keys		keys;
	t_time		time;
}	t_game;

// Main functions
void			init_game(t_game *game);
char			*get_next_line(int fd);
int				parser(int argc, char **argv, t_game *game,
					t_parse_data *parse);
int				set_textures(t_game *game);
int				init_texture(t_texture *texture, char *texture_path, void *mlx);

// parser_validation.c
int				validate_arguments(int argc, char **argv);
int				open_and_validate_file(char *filepath);
int				validate_texture_file(char *path, char *texture_name);

// parser_file.c
int				read_file_content(int fd, t_parse_data *parse);
int				is_empty_line(char *line);
int				is_map_line(char *line);

// parser_config.c
int				parse_config_elements(t_parse_data *parse, t_map *map);
int				parse_texture_line(char *line, t_map *map, t_parse_data *parse);
int				parse_color_line(char *line, t_map *map, t_parse_data *parse);
int				validate_config_complete(t_map *map, t_parse_data *parse);
int				extract_rgb_values(char *str, int *rgb);

// parse_map.c
int				parse_map(t_parse_data *parse, t_map *map, t_player *player);
int				normalize_map_width(t_map *map);
int				find_map_boundaries(t_parse_data *parse, int *map_start,
					int *map_end);
int				extract_player_position(t_map *map, t_player *player);

// parser_map_validation.c
int				validate_map(t_map *map, t_player *player);
int				validate_map_characters(t_map *map);
int				flood_fill_check(t_map *map, t_player *player);
int				flood_fill_recursive(t_map *map, int **visited, int x, int y);
int				check_map_borders(t_map *map);

// parser_utils.c
char			**ft_split_whitespace(char *str);
char			*trim_spaces(char *str);
int				**allocate_2d_int_array(int height, int width);
void			free_2d_int_array(int **array, int height);
void			free_split(char **split);

// parser_cleanup.c
void			cleanup_parse_data(t_parse_data *parse);
void			cleanup_game(t_game *game);
void			cleanup_config(t_game *game);
void			free_map_grid(t_map *map);

// init_struct.c (funzioni effettivamente utilizzate)
int				parse_complete_file(char *map_path, t_map *map,
					t_player *player);
int				init_texture(t_texture *texture, char *texture_path, void *mlx);
int				init_mlx(t_mlx *mlx);
void			free_mlx(t_mlx *mlx);
void			free_game(t_game *game);

// game_loop.c
int				key_press(int keycode, t_game *game);
int				close_window(t_game *game);
int				game_loop(t_game *game);

// raycasting.c
void			raycasting(t_game *game, int screen_x);
void			draw_map_2d(t_game *game);
void			draw_player_2d(t_game *game);
void			draw_minimap(t_game *game);

// movement.c
void			update_player(t_game *game);
void			rot_player(t_game *game, double direction, double speed);
void			move_player(t_game *game, double dir_x, double dir_y);

// hooks.c
int				mouse_hook(int x, int y, t_game *game);
int				key_press(int keycode, t_game *game);
int				key_release(int keycode, t_game *game);
int				close_window(t_game *game);

// utils.c
long long		get_time_ms(void);
unsigned int	get_pixel_color(t_texture *texture, int x, int y);
void			put_pixel(t_game *game, int x, int y, int color);
void			draw_circle(t_game *game, int cx, int cy, int radius, int color);
void			*ft_memset(void *b, int c, size_t len);
char			*ft_strdup(const char *s1);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				ft_strlen(const char *s);
void			free_game(t_game *game);
void			free_texture(t_texture *texture, void *mlx);
void			free_mlx(t_mlx *mlx);
void			free_map(t_map *map);

// 2d.c
void			draw_map_2d(t_game *game);
void			draw_player_2d(t_game *game);

// minimap.c
void			draw_minimap(t_game *game);
void			draw_border(t_game *game);

// hud.c
void			draw_crosshair(t_game *game);
void			draw_torch(t_game *game);

#endif
