#include "../includes/cub3d.h"

/*
 * STEP 1: INIZIALIZZAZIONE STRUTTURA PRINCIPALE
 * - Azzera completamente la struttura t_game
 * - Imposta le dimensioni base della finestra MLX
 * - Prepara tutte le sotto-strutture (map, player, mlx, textures) a zero
 */
/* void init_game(t_game *game)
{
	// Inizializza tutti i valori a zero o NULL
	memset(game, 0, sizeof(t_game));
	
	// Imposta le dimensioni della finestra
	game->mlx.win_width = 1024;  // o altra larghezza a tua scelta
	game->mlx.win_height = 768;  // o altra altezza a tua scelta
} */

int init_mlx(t_mlx *mlx)
{
	if (!mlx)
		return (1);
	
	// Azzera la struttura
	memset(mlx, 0, sizeof(t_mlx));
	
	mlx->win_width = 1280;
	mlx->win_height = 960;
	
	mlx->mlx = mlx_init();
	if (!mlx->mlx)
		return (1);
		
	mlx->win = mlx_new_window(mlx->mlx, mlx->win_width, mlx->win_height, "cub3D");
	if (!mlx->win)
	{
		free(mlx->mlx);
		mlx->mlx = NULL;
		return (1);
	}
		
	mlx->img = mlx_new_image(mlx->mlx, mlx->win_width, mlx->win_height);
	if (!mlx->img)
	{
		mlx_destroy_window(mlx->mlx, mlx->win);
		free(mlx->mlx);
		mlx->mlx = NULL;
		mlx->win = NULL;
		return (1);
	}
		
	mlx->addr = mlx_get_data_addr(mlx->img, &mlx->bits_per_pixel, 
								 &mlx->line_length, &mlx->endian);
	if (!mlx->addr)
	{
		mlx_destroy_image(mlx->mlx, mlx->img);
		mlx_destroy_window(mlx->mlx, mlx->win);
		free(mlx->mlx);
		memset(mlx, 0, sizeof(t_mlx));
		return (1);
	}
	
	return (0);
}


int init_texture(t_texture *texture, char *texture_path, void *mlx)
{
	if (!texture || !texture_path || !mlx)
		return (1);
	
	// Azzera la struttura
	memset(texture, 0, sizeof(t_texture));
	
	// Carica la texture usando MLX
	texture->img = mlx_xpm_file_to_image(mlx, texture_path, &texture->width, &texture->height);
	if (!texture->img)
		return (1);
	
	texture->addr = mlx_get_data_addr(texture->img, &texture->bits_per_pixel, 
									 &texture->line_length, &texture->endian);
	if (!texture->addr)
	{
		mlx_destroy_image(mlx, texture->img);
		texture->img = NULL;
		return (1);
	}
	
	return (0);
}

int	set_textures(t_game *game)
{
	if (init_texture(&game->textures[0], game->map.north_texture, game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load north texture: %s\n", game->map.north_texture);
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[1], game->map.south_texture, game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load south texture: %s\n", game->map.south_texture);
		free_texture(&game->textures[0], game->mlx.mlx);
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[2], game->map.west_texture, game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load west texture: %s\n", game->map.west_texture);
		free_texture(&game->textures[0], game->mlx.mlx);
		free_texture(&game->textures[1], game->mlx.mlx);
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[3], game->map.east_texture, game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load east texture: %s\n", game->map.east_texture);
		free_texture(&game->textures[0], game->mlx.mlx);
		free_texture(&game->textures[1], game->mlx.mlx);
		free_texture(&game->textures[2], game->mlx.mlx);
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	// added crosshair texture
	if (init_texture(&game->textures[4], "assets/textures/cross.xpm", game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load cross: %s\n", "assets/textures/cross.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[5], "assets/textures/torch20_1.xpm", game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load torch: %s\n", "assets/textures/torch1.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[6], "assets/textures/torch10_2.xpm", game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load torch: %s\n", "assets/textures/torch1.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[7], "assets/textures/torch40_3.xpm", game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load torch: %s\n", "assets/textures/torch1.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[8], "assets/textures/torch60_4.xpm", game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load torch: %s\n", "assets/textures/torch1.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[9], "assets/textures/torch70_5.xpm", game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load torch: %s\n", "assets/textures/torch1.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	return (0);
}


int main(int argc, char **argv)
{
	t_game          game;
	t_parse_data    parse_data;  // Struttura temporanea

	
	// Inizializzazione
	ft_memset(&game, 0, sizeof(t_game));
	ft_memset(&parse_data, 0, sizeof(t_parse_data));
	
	// Parsing con struttura temporanea
	if (parser(argc, argv, &game, &parse_data) != 0)
	{
		cleanup_parse_data(&parse_data);  // Pulisci dati temporanei
		cleanup_game(&game);              // Pulisci dati gioco
		return (1);
	}
	
	
   
	// Inizializza i valori double del player basandoti su direction
	// init_player_direction(&game.player);


/* 	init_game(&game);
 */	
	// STEP 2: PARSING COMPLETO + SETUP GRAFICO
	// Questo step riempie completamente game->map e game->player,
	// inizializza MLX e carica tutte le texture
	if (parse_map(&parse_data, &game.map, &game.player) != 0)
		return (1);
	
	// Parse data non serve più, libera subito
	// cleanup_parse_data(&parse_data);

	// STEP 3: AVVIO GAME LOOP INFINITO
	// Gestisce rendering continuo + eventi tastiera/mouse

	// Inizializza MLX
	if (init_mlx(&game.mlx) != 0)
	{
		printf("Error\nFailed to initialize graphics\n");
		free_map(&game.map);
		return (1);
	}
	set_textures(&game);

	game_loop(&game);
	
	return (0);
}