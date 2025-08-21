#ifndef CUB3D_H
# define CUB3D_H

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>  // Per stat() in texture_check.c
#include <math.h>
#include <stdbool.h>
#include "../mlx_linux/mlx.h"
// #include "../mlx_apple/mlx.h"

#define BONUS 1
#define DEBUG 0

typedef struct s_map {
	char **grid;          // La mappa come array 2D
	int width;            // Larghezza della mappa
	int height;           // Altezza della mappa
	char *north_texture;  // Percorso della texture nord
	char *south_texture;  // Percorso della texture sud
	char *west_texture;   // Percorso della texture ovest
	char *east_texture;   // Percorso della texture est
	int floor_color[3];   // Colore RGB del pavimento
	int ceiling_color[3]; // Colore RGB del soffitto
} t_map;

typedef struct s_player {
	double pos_x;         // Posizione X del giocatore
	double pos_y;         // Posizione Y del giocatore
	double dir_x;         // Direzione X del giocatore
	double dir_y;         // Direzione Y del giocatore
	double plane_x;       // Piano camera X
	double plane_y;       // Piano camera Y
	char direction;       // Direzione iniziale (N, S, E, W)
} t_player;

typedef struct s_ray {
	double camera_x;      // Coordinata X nello spazio della camera
	double dir_x;         // Direzione X del raggio
	double dir_y;         // Direzione Y del raggio
	int map_x;            // Coordinata X del quadrato della mappa
	int map_y;            // Coordinata Y del quadrato della mappa
	double side_dist_x;   // Distanza fino al prossimo lato X
	double side_dist_y;   // Distanza fino al prossimo lato Y
	double delta_dist_x;  // Distanza tra lati X successivi
	double delta_dist_y;  // Distanza tra lati Y successivi
	double perp_wall_dist; // Distanza perpendicolare al muro
	int step_x;           // Direzione del passo X (+1 o -1)
	int step_y;           // Direzione del passo Y (+1 o -1)
	int hit;              // Se è stato colpito un muro (1) o no (0)
	int side;             // Se è stato colpito un lato NS (0) o EW (1)
	double	intersection_x;
	double	intersection_y;
} t_ray;

typedef struct s_texture {
	void *img;            // Puntatore all'immagine
	char *addr;           // Indirizzo dei dati dell'immagine
	int bits_per_pixel;   // Bits per pixel
	int line_length;      // Lunghezza della linea
	int endian;           // Endianness
	int width;            // Larghezza della texture
	int height;           // Altezza della texture
} t_texture;

typedef struct s_mlx {
	void *mlx;            // Puntatore alla connessione MLX
	void *win;            // Puntatore alla finestra MLX
	void *img;            // Puntatore all'immagine
	char *addr;           // Indirizzo dei dati dell'immagine
	int bits_per_pixel;   // Bits per pixel
	int line_length;      // Lunghezza della linea
	int endian;           // Endianness
	int win_width;        // Larghezza della finestra
	int win_height;       // Altezza della finestra
} t_mlx;

typedef struct s_time {
	double      time;
    double      old_time;
    double      frame_time;
}	t_time;

typedef struct	s_keys
{
	bool w;
	bool a;
	bool s;
	bool d;
	bool left;
	bool right;
}	t_keys;

typedef struct s_game {
	t_map map;            // Struttura della mappa
	t_player player;      // Struttura del giocatore
	t_mlx mlx;            // Struttura MLX
	t_ray ray;
	t_texture textures[4]; // Array di texture (N, S, E, W)
	t_keys	keys;
	t_time	time;
} t_game;

// Main functions
void	init_game(t_game *game);
char	*get_next_line(int fd);
int		parse_map(char *map_path, t_game *game);

// file_check.c
int		validate_file_extension(char *filename);
int		validate_file_access(char *filename);
int		count_file_lines(char *filename);
char	**read_file_lines(char *filename, int line_count);
char	*trim_whitespace(char *str);
int		is_empty_line(char *line);
int		is_map_line(char *line);
int		parse_file_elements(char **lines, t_map *map);
void	free_string_array(char **array);

// texture_check.c
int		parse_texture(char *line, char **texture_path);
void	free_texture(t_texture *texture, void *mlx);

// rgb_check.c
int		validate_rgb_values(int r, int g, int b);
int		parse_rgb_color(char *line, int *color);

// map_check.c
void	calculate_map_dimensions(char **lines, int start_line, int *width, int *height);
int		validate_map_closed(char **grid, int width, int height, t_player *player);
int		find_and_validate_player(char **grid, int width, int height, t_player *player);
int		parse_map_grid(char **lines, int start_line, t_map *map, t_player *player);
void	free_map_grid(t_map *map);
void	free_map(t_map *map);

// init_struct.c (funzioni effettivamente utilizzate)
int		parse_complete_file(char *map_path, t_map *map, t_player *player);
int		init_texture(t_texture *texture, char *texture_path, void *mlx);
int		init_mlx(t_mlx *mlx);
void	free_mlx(t_mlx *mlx);
void	free_game(t_game *game);

// game_loop.c
int		key_press(int keycode, t_game *game);
int		close_window(t_game *game);
int		game_loop(t_game *game);

// raycasting.c
void	raycasting(t_game *game, int screen_x);
void	draw_map_2d(t_game *game);
void	draw_player_2d(t_game *game);
void	draw_minimap(t_game *game);

#endif
