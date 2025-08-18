/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:23:55 by edforte           #+#    #+#             */
/*   Updated: 2025/06/03 14:27:29 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// Funzione per validare che non ci siano righe vuote nella mappa
int validate_no_empty_lines_in_map(char **lines, int start_line)
{
	int i = start_line;
	int j;
	
	while (lines[i])
	{
		// Controlla se la riga è vuota o contiene solo whitespace
		j = 0;
		while (lines[i][j] && (lines[i][j] == ' ' || lines[i][j] == '\t'))
			j++;
		
		// Se dopo gli spazi c'è solo \n o \0, la riga è vuota
		if (lines[i][j] == '\n' || lines[i][j] == '\0')
		{
			printf("Error\nEmpty line found in map at line %d\n", i + 1);
			return (1);
		}
		i++;
	}
	
	return (0);
}

// Funzione per calcolare le dimensioni della mappa
void calculate_map_dimensions(char **lines, int start_line, int *width, int *height)
{
	int i = start_line;
	int max_width = 0;
	int current_width;
	
	*height = 0;
	
	// Conta TUTTE le righe dal start_line fino alla fine
	while (lines[i])
	{
		(*height)++;
		i++;
	}
	
	// Trova la larghezza massima
	i = start_line;
	while (i < start_line + *height && lines[i])
	{
		current_width = (int)strlen(lines[i]);
		if (current_width > 0 && lines[i][current_width - 1] == '\n')
			current_width--;
		
		if (current_width > max_width)
			max_width = current_width;
		i++;
	}
	
	*width = max_width;
}

// Funzione per creare una copia della griglia per il flood fill
char **copy_grid(char **original, int width, int height)
{
	char **copy;
	int i, j;
	
	copy = malloc(sizeof(char *) * height);
	if (!copy)
		return (NULL);
	
	i = 0;
	while (i < height)
	{
		copy[i] = malloc(sizeof(char) * (width + 1));
		if (!copy[i])
		{
			while (--i >= 0)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		
		j = 0;
		while (j < width)
		{
			copy[i][j] = original[i][j];
			j++;
		}
		copy[i][width] = '\0';
		i++;
	}
	
	return (copy);
}

// Funzione per liberare la copia della griglia
void free_grid_copy(char **grid, int height)
{
	int i = 0;
	
	if (!grid)
		return;
	
	while (i < height)
	{
		if (grid[i])
			free(grid[i]);
		i++;
	}
	free(grid);
}

// Flood fill ricorsivo per validare la mappa
int flood_fill_validate(char **grid, int x, int y, int width, int height)
{
	// Controlli di bounds
	if (x < 0 || x >= width || y < 0 || y >= height)
		return (1); // Fuori dai limiti = errore
	
	// Se è un muro o già visitato, ok
	if (grid[y][x] == '1' || grid[y][x] == 'V')
		return (0);
	
	// Se è spazio vuoto, errore - il player può "uscire"
	if (grid[y][x] == ' ')
	{
		printf("Error\nMap not properly closed - reachable space at (%d,%d)\n", x, y);
		return (1);
	}
	
	// Se è un carattere calpestabile ('0', 'N', 'S', 'E', 'W')
	if (grid[y][x] == '0' || grid[y][x] == 'N' || grid[y][x] == 'S' ||
		grid[y][x] == 'E' || grid[y][x] == 'W')
	{
		// Marca come visitato
		grid[y][x] = 'V';
		
		// Esplora nelle 4 direzioni
		if (flood_fill_validate(grid, x - 1, y, width, height) ||
			flood_fill_validate(grid, x + 1, y, width, height) ||
			flood_fill_validate(grid, x, y - 1, width, height) ||
			flood_fill_validate(grid, x, y + 1, width, height))
		{
			return (1);
		}
	}
	
	return (0);
}

// Funzione per validare la mappa usando flood fill
int validate_map_closed(char **grid, int width, int height, t_player *player)
{
	char **grid_copy;
	int player_x, player_y;
	int result;
	
	// Crea copia della griglia per il flood fill
	grid_copy = copy_grid(grid, width, height);
	if (!grid_copy)
	{
		printf("Error\nMemory allocation failed for flood fill\n");
		return (1);
	}
	
	// Trova posizione player (in coordinate intere)
	player_x = (int)player->pos_x;
	player_y = (int)player->pos_y;
	
	// Esegui flood fill dalla posizione del player
	result = flood_fill_validate(grid_copy, player_x, player_y, width, height);
	
	// Libera la copia
	free_grid_copy(grid_copy, height);
	
	return (result);
}

// Funzione per trovare e validare il giocatore
int find_and_validate_player(char **grid, int width, int height, t_player *player)
{
	int i = 0;
	int j;
	int player_count = 0;
	
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			if (grid[i][j] == 'N' || grid[i][j] == 'S' || 
				grid[i][j] == 'E' || grid[i][j] == 'W')
			{
				player_count++;
				if (player_count > 1)
				{
					printf("Error\nMultiple player positions found\n");
					return (1);
				}
				
				player->pos_x = j + 0.5;
				player->pos_y = i + 0.5;
				player->direction = grid[i][j];
				
				// Imposta direzione e piano camera
				if (grid[i][j] == 'N')
				{
					player->dir_x = 0;
					player->dir_y = -1;
					player->plane_x = 0.66;
					player->plane_y = 0;
				}
				else if (grid[i][j] == 'S')
				{
					player->dir_x = 0;
					player->dir_y = 1;
					player->plane_x = -0.66;
					player->plane_y = 0;
				}
				else if (grid[i][j] == 'E')
				{
					player->dir_x = 1;
					player->dir_y = 0;
					player->plane_x = 0;
					player->plane_y = 0.66;
				}
				else if (grid[i][j] == 'W')
				{
					player->dir_x = -1;
					player->dir_y = 0;
					player->plane_x = 0;
					player->plane_y = -0.66;
				}
				
				// Sostituisci con '0' per il flood fill
				grid[i][j] = '0';
			}
			j++;
		}
		i++;
	}
	
	if (player_count == 0)
	{
		printf("Error\nNo player position found\n");
		return (1);
	}
	
	return (0);
}

/*
 * FIX CORRETTO: Usa start_line direttamente, senza variabili intermedie inutili
 * ELIMINATA: Vera ridondanza di ricerca duplicata
 */
int parse_map_grid(char **lines, int start_line, t_map *map, t_player *player)
{
	int i, j;
	char *line;
	int line_len;
	
	if (!lines || !map || !player)
	{
		printf("Error\nInvalid parameters for map parsing\n");
		return (1);
	}
	
	// Salta eventuali righe vuote subito dopo gli elementi
	while (lines[start_line])
	{
		// Controlla se la riga è vuota
		j = 0;
		while (lines[start_line][j] && (lines[start_line][j] == ' ' || lines[start_line][j] == '\t'))
			j++;
		
		// Se non è vuota, inizia la mappa qui
		if (lines[start_line][j] != '\n' && lines[start_line][j] != '\0')
			break;
		
		start_line++;
	}
	
	if (!lines[start_line])
	{
		printf("Error\nNo valid map found after elements\n");
		return (1);
	}
	
	// Valida che non ci siano righe vuote nella mappa
	if (validate_no_empty_lines_in_map(lines, start_line))
		return (1);
	
	// Calcola dimensioni
	calculate_map_dimensions(lines, start_line, &map->width, &map->height);
	
	if (map->width < 3 || map->height < 3)
	{
		printf("Error\nMap too small (minimum 3x3)\n");
		return (1);
	}
	
	// Alloca griglia
	map->grid = malloc(sizeof(char *) * map->height);
	if (!map->grid)
	{
		printf("Error\nMemory allocation failed for map grid\n");
		return (1);
	}
	
	// Inizializza puntatori
	i = 0;
	while (i < map->height)
	{
		map->grid[i] = NULL;
		i++;
	}
	
	// Alloca righe
	i = 0;
	while (i < map->height)
	{
		map->grid[i] = malloc(sizeof(char) * (map->width + 1));
		if (!map->grid[i])
		{
			while (--i >= 0)
				free(map->grid[i]);
			free(map->grid);
			map->grid = NULL;
			printf("Error\nMemory allocation failed for map row\n");
			return (1);
		}
		
		// Inizializza con spazi
		j = 0;
		while (j < map->width)
		{
			map->grid[i][j] = ' ';
			j++;
		}
		map->grid[i][map->width] = '\0';
		i++;
	}
	
	// Riempi griglia partendo da start_line
	i = 0;
	while (i < map->height && lines[start_line + i])
	{
		line = lines[start_line + i];
		line_len = (int)strlen(line);
		
		if (line_len > 0 && line[line_len - 1] == '\n')
			line_len--;
		
		j = 0;
		while (j < line_len && j < map->width)
		{
			map->grid[i][j] = line[j];
			j++;
		}
		i++;
	}
	
	// Trova e valida player
	if (find_and_validate_player(map->grid, map->width, map->height, player))
	{
		free_map_grid(map);
		return (1);
	}
	
	// Valida mappa con flood fill
	if (validate_map_closed(map->grid, map->width, map->height, player))
	{
		free_map_grid(map);
		return (1);
	}
	
	return (0);
}

// Funzioni di cleanup
void free_map_grid(t_map *map)
{
	int i = 0;
	
	if (!map || !map->grid)
		return;
	
	while (i < map->height && map->grid[i])
	{
		free(map->grid[i]);
		i++;
	}
	free(map->grid);
	map->grid = NULL;
}

void free_map(t_map *map)
{
	if (!map)
		return;
		
	free_map_grid(map);
	
	if (map->north_texture)
	{
		free(map->north_texture);
		map->north_texture = NULL;
	}
	if (map->south_texture)
	{
		free(map->south_texture);
		map->south_texture = NULL;
	}
	if (map->west_texture)
	{
		free(map->west_texture);
		map->west_texture = NULL;
	}
	if (map->east_texture)
	{
		free(map->east_texture);
		map->east_texture = NULL;
	}
}
