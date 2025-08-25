#include "../includes/cub3d.h"

/*
 * STEP 1: INIZIALIZZAZIONE STRUTTURA PRINCIPALE
 * - Azzera completamente la struttura t_game
 * - Imposta le dimensioni base della finestra MLX
 * - Prepara tutte le sotto-strutture (map, player, mlx, textures) a zero
 */
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
    
    // Parse data non serve più, libera subito
    cleanup_parse_data(&parse_data);
    
    // Inizializza i valori double del player basandoti su direction
    init_player_direction(&game.player);


	init_game(&game);
    
    // Game loop...
    game_loop(&game);
    
    cleanup_game(&game);
    return (0);
	
	return (0);
}