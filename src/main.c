#include "../includes/cub3d.h"

void init_game(t_game *game)
{
	// Inizializza tutti i valori a zero o NULL
	memset(game, 0, sizeof(t_game));
	
	// Imposta le dimensioni della finestra
	game->mlx.win_width = 1024;  // o altra larghezza a tua scelta
	game->mlx.win_height = 768;  // o altra altezza a tua scelta
}

int main(int argc, char **argv)
{
	t_game game;
	
	if (argc != 2)
	{
		printf("Error\nUsage: ./cub3D <map.cub>\n");
		return (1);
	}
	
	init_game(&game);
	
	// Parsing del file .cub (da implementare)
	if (parse_map(argv[1], &game) != 0)
		return (1);
	
	// Inizializzazione MLX
	game.mlx.mlx = mlx_init();
	if (!game.mlx.mlx)
	{
		printf("Error\nCouldn't initialize MLX\n");
		return (1);
	}
	
	game.mlx.win = mlx_new_window(game.mlx.mlx, game.mlx.win_width, game.mlx.win_height, "cub3D");
	if (!game.mlx.win)
	{
		printf("Error\nCouldn't create window\n");
		return (1);
	}
	
	// Loop del gioco (da implementare)
	game_loop(&game);
	
	return (0);
}
