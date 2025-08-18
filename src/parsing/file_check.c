/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:43:11 by edforte           #+#    #+#             */
/*   Updated: 2025/06/03 13:39:42 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// Funzione per validare l'estensione del file
int validate_file_extension(char *filename)
{
	int len = strlen(filename);
	if (len < 4)
		return (1);
	if (strcmp(filename + len - 4, ".cub") != 0)
		return (1);
	return (0);
}

// Funzione per validare se il file esiste ed è leggibile
int validate_file_access(char *filename)
{
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (1);
	close(fd);
	return (0);
}

// Funzione per contare le righe del file
int count_file_lines(char *filename)
{
	int fd, count = 0;
	char buffer[1024];
	int bytes_read, i;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (-1);
	
	while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
	{
		i = 0;
		while (i < bytes_read)
		{
			if (buffer[i] == '\n')
				count++;
			i++;
		}
	}
	close(fd);
	return (count);
}

/*
 * STEP 2A-1: LETTURA COMPLETA FILE IN MEMORIA
 * 
 * FLUSSO DATI:
 * INPUT: filename
 * OUTPUT: array di stringhe, una per ogni riga del file
 * 
 * Legge tutto il file usando get_next_line() e crea un array di char*
 * dove ogni elemento è una riga del file con \n incluso
 */
char **read_file_lines(char *filename, int line_count)
{
	int fd, i = 0;
	char **lines;
	char *line;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (NULL);
	
	lines = malloc(sizeof(char *) * (line_count + 1));
	if (!lines)
	{
		close(fd);
		return (NULL);
	}
	
	// Inizializza tutti i puntatori a NULL
	while (i <= line_count)
	{
		lines[i] = NULL;
		i++;
	}
	
	i = 0;
	while (i < line_count && (line = get_next_line(fd)) != NULL)
	{
		lines[i] = line;
		i++;
	}
	
	close(fd);
	return (lines);
}

// Funzione per rimuovere spazi e tab da una stringa
char *trim_whitespace(char *str)
{
	char *start = str;
	char *end;
	
	// Rimuovi spazi all'inizio
	while (*start == ' ' || *start == '\t')
		start++;
	
	if (*start == '\0')
		return (start);
		
	// Rimuovi spazi alla fine
	end = start + strlen(start) - 1;
	while (end > start && (*end == ' ' || *end == '\t' || *end == '\n'))
		end--;
	
	*(end + 1) = '\0';
	return (start);
}

// Funzione per verificare se una linea è vuota o contiene solo spazi
int is_empty_line(char *line)
{
	while (*line)
	{
		if (*line != ' ' && *line != '\t' && *line != '\n')
			return (0);
		line++;
	}
	return (1);
}

// Funzione per verificare se una linea è parte della mappa
int is_map_line(char *line)
{
	while (*line)
	{
		if (*line != '0' && *line != '1' && *line != 'N' && 
			*line != 'S' && *line != 'E' && *line != 'W' && 
			*line != ' ' && *line != '\t' && *line != '\n')
			return (0);
		line++;
	}
	return (1);
}

/*
 * STEP 2A-2: PARSING ELEMENTI TEXTURE E COLORI
 * 
 * FLUSSO DATI:
 * INPUT: lines (array di righe del file), map (vuota)
 * OUTPUT: map popolata con texture paths e colori RGB
 * 
 * Cerca e parsa nell'ordine:
 * - NO, SO, WE, EA (texture paths) -> map->north_texture, south_texture, etc.
 * - F, C (colori RGB) -> map->floor_color[3], ceiling_color[3]
 * 
 * RETURN: indice della prima riga di mappa (dove inizia la griglia)
 *         o -1 se errore (elemento mancante/duplicato/malformato)
 */
int parse_file_elements(char **lines, t_map *map)
{
	int i = 0;
	char *trimmed;
	int elements_found = 0;
	
	while (lines[i] && elements_found < 6)
	{
		trimmed = trim_whitespace(lines[i]);
		
		if (is_empty_line(trimmed))
		{
			i++;
			continue;
		}
		
		if (strncmp(trimmed, "NO ", 3) == 0)
		{
			if (map->north_texture || parse_texture(trimmed, &map->north_texture))
			{
				printf("Error\nInvalid or duplicate north texture\n");
				return (-1);
			}
			elements_found++;
		}
		else if (strncmp(trimmed, "SO ", 3) == 0)
		{
			if (map->south_texture || parse_texture(trimmed, &map->south_texture))
			{
				printf("Error\nInvalid or duplicate south texture\n");
				return (-1);
			}
			elements_found++;
		}
		else if (strncmp(trimmed, "WE ", 3) == 0)
		{
			if (map->west_texture || parse_texture(trimmed, &map->west_texture))
			{
				printf("Error\nInvalid or duplicate west texture\n");
				return (-1);
			}
			elements_found++;
		}
		else if (strncmp(trimmed, "EA ", 3) == 0)
		{
			if (map->east_texture || parse_texture(trimmed, &map->east_texture))
			{
				printf("Error\nInvalid or duplicate east texture\n");
				return (-1);
			}
			elements_found++;
		}
		else if (strncmp(trimmed, "F ", 2) == 0)
		{
			if (map->floor_color[0] != -1 || parse_rgb_color(trimmed + 2, map->floor_color))
			{
				printf("Error\nInvalid or duplicate floor color\n");
				return (-1);
			}
			elements_found++;
		}
		else if (strncmp(trimmed, "C ", 2) == 0)
		{
			if (map->ceiling_color[0] != -1 || parse_rgb_color(trimmed + 2, map->ceiling_color))
			{
				printf("Error\nInvalid or duplicate ceiling color\n");
				return (-1);
			}
			elements_found++;
		}
		else if (is_map_line(trimmed))
		{
			break; // Inizio della mappa trovato
		}
		else
		{
			printf("Error\nUnrecognized element in file: %s\n", trimmed);
			return (-1);
		}
		i++;
	}
	
	if (elements_found != 6)
	{
		printf("Error\nMissing required elements (need 4 textures + 2 colors)\n");
		return (-1);
	}
	
	return (i);
}

// Funzione per liberare array di stringhe
void free_string_array(char **array)
{
	int i = 0;
	
	if (!array)
		return;
	
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}