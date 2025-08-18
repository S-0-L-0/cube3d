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

/*
 * FLUSSO PRINCIPALE DEL PROGRAMMA:
 * 
 * 1. Valida argomenti da linea di comando
 * 2. Inizializza struttura principale t_game
 * 3. Parsing completo del file .cub (riempie game->map e game->player)
 * 4. Inizializzazione grafica MLX e caricamento texture
 * 5. Avvio del game loop (rendering + gestione eventi)
 * 
 * DATI IN USCITA:
 * - game->map: griglia 2D, dimensioni, path texture, colori RGB
 * - game->player: posizione, direzione, piano camera
 * - game->mlx: connessione grafica, finestra, buffer immagine
 * - game->textures[4]: array di texture caricate (N,S,W,E)
 */
int main(int argc, char **argv)
{
	t_game game;
	
	// VALIDAZIONE INPUT: Deve essere fornito esattamente un file .cub
	if (argc != 2)
	{
		printf("Error\nmissing .cub maps file");
		return (1);
	}
	
	// STEP 1: Inizializza struttura principale a valori di default
	init_game(&game);
	
	// STEP 2: PARSING COMPLETO + SETUP GRAFICO
	// Questo step riempie completamente game->map e game->player,
	// inizializza MLX e carica tutte le texture
	if (parse_map(argv[1], &game) != 0)
		return (1);
	
	// STEP 3: AVVIO GAME LOOP INFINITO
	// Gestisce rendering continuo + eventi tastiera/mouse
	game_loop(&game);
	
	return (0);
}