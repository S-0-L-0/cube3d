/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:24:27 by edforte           #+#    #+#             */
/*   Updated: 2025/08/18 19:13:37 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/*
 * STEP 2A: PARSING COMPLETO DEL FILE .CUB
 * 
 * FLUSSO DATI DETTAGLIATO:
 * INPUT: map_path, map (vuota), player (vuoto)
 * 
 * 1. Validazione file (.cub extension + readable)
 * 2. Lettura completa file in array di stringhe
 * 3. Parsing elementi (4 texture + 2 colori) -> RIEMPIE map->textures e map->colors
 * 4. Parsing griglia mappa -> RIEMPIE map->grid, map->width, map->height
 * 5. Estrazione posizione player -> RIEMPIE player->pos_x, pos_y, direction, etc.
 * 
 * OUTPUT: map e player completamente popolati con tutti i dati dal file
 * 
 * NOTA: Questa è la funzione "cervello" che coordina tutto il parsing
 */
int parse_complete_file(char *map_path, t_map *map, t_player *player)
{
	char **lines;
	int line_count;
	int start_line;
	
	if (!map_path || !map || !player)
		return (1);
	
	// Inizializza strutture
	memset(map, 0, sizeof(t_map));
	memset(player, 0, sizeof(t_player));
	map->floor_color[0] = -1;
	map->ceiling_color[0] = -1;
	
	// Valida file
	if (validate_file_extension(map_path))
		return (1);
	
	if (validate_file_access(map_path))
		return (1);
	// Leggi file
	line_count = count_file_lines(map_path);
	if (line_count <= 0)
		return (printf("Error\nInvalid map file\n"), 1);
	
	lines = read_file_lines(map_path, line_count);
	if (!lines)
		return (1);
	
	// Parsa elementi
	start_line = parse_file_elements(lines, map);
	if (start_line < 0)
	{
		free_string_array(lines);
		free_map(map);
		return (1);
	}
	
	// Parsa mappa
	if (parse_map_grid(lines, start_line, map, player))
	{
		free_string_array(lines);
		free_map(map);
		return (1);
	}

	// Cleanup finale
	free_string_array(lines);
	
	return (0);
}

/*
 * STEP 2C: CARICAMENTO SINGOLA TEXTURE DA FILE
 * 
 * FLUSSO DATI:
 * INPUT: texture (struttura vuota), texture_path (path .xpm), mlx (connessione attiva)
 * OUTPUT: texture completamente caricata in memoria grafica
 * 
 * 1. mlx_xpm_file_to_image() -> carica .xpm in memoria GPU
 * 2. mlx_get_data_addr() -> ottiene puntatore ai pixel per manipolazione
 * 3. Popola texture->width, height, bits_per_pixel, etc.
 * 
 * NOTA: Viene chiamata 4 volte in parse_map() per N, S, W, E textures
 */
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

/*
 * STEP 2B: INIZIALIZZAZIONE SISTEMA GRAFICO MLX
 * 
 * FLUSSO DATI:
 * INPUT: mlx (struttura vuota)
 * OUTPUT: mlx completamente configurata per il rendering
 * 
 * 1. mlx->mlx = connessione al server grafico
 * 2. mlx->win = finestra di gioco (800x600)
 * 3. mlx->img = buffer immagine per il rendering
 * 4. mlx->addr = puntatore ai pixel dell'immagine per manipolazione diretta
 * 
 * CRITICO: Se qualsiasi step fallisce, libera tutto e restituisce errore
 */
int init_mlx(t_mlx *mlx)
{
	if (!mlx)
		return (1);
	
	// Azzera la struttura
	memset(mlx, 0, sizeof(t_mlx));
	
	mlx->win_width = 1024;
	mlx->win_height = 768;
	
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

// Funzione per liberare MLX
void free_mlx(t_mlx *mlx)
{
	if (!mlx)
		return;
		
	if (mlx->img)
	{
		mlx_destroy_image(mlx->mlx, mlx->img);
		mlx->img = NULL;
	}
	
	if (mlx->win)
	{
		mlx_destroy_window(mlx->mlx, mlx->win);
		mlx->win = NULL;
	}
	
	if (mlx->mlx)
	{
		free(mlx->mlx);
		mlx->mlx = NULL;
	}
	
	memset(mlx, 0, sizeof(t_mlx));
}

/*
 * STEP FINALE: PULIZIA COMPLETA MEMORIA (CLEANUP)
 * 
 * FLUSSO DISTRUZIONE:
 * INPUT: game (struttura completa in memoria)
 * OUTPUT: tutta la memoria liberata, programma pronto per exit()
 * 
 * ORDINE CRITICO DI PULIZIA:
 * 1. game->textures[4] -> mlx_destroy_image() per ogni texture caricata
 * 2. game->map.grid -> free() di ogni riga + array principale
 * 3. game->map texture paths -> free() di tutte le stringhe
 * 4. game->mlx.img -> mlx_destroy_image() del buffer principale
 * 5. game->mlx.win -> mlx_destroy_window() della finestra
 * 6. game->mlx.mlx -> free() della connessione MLX
 * 7. game->player -> memset() a zero (non ha allocazioni dinamiche)
 * 
 * IMPORTANTE: Viene chiamata sempre prima di exit(), sia per ESC che per errori
 * Previene memory leak e disconnette correttamente dal server grafico
 */
void free_game(t_game *game)
{
	int i;
	
	if (!game)
		return;
	
	// Libera le texture
	i = 0;
	while (i < 4)
	{
		free_texture(&game->textures[i], game->mlx.mlx);
		i++;
	}
	
	// Libera la mappa
	free_map(&game->map);
	
	// Libera MLX
	free_mlx(&game->mlx);
	
	// Azzera il player
	memset(&game->player, 0, sizeof(t_player));
}