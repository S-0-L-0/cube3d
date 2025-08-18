/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:42:20 by edforte           #+#    #+#             */
/*   Updated: 2025/06/03 13:40:05 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// Funzione per verificare se un path è un file regolare (non directory)
int is_regular_file(char *path)
{
	struct stat path_stat;
	
	if (stat(path, &path_stat) != 0)
		return (0); // Errore nell'accesso al file
	
	return S_ISREG(path_stat.st_mode); // Restituisce 1 se è un file regolare
}

// Funzione per verificare se un file ha estensione .xpm
int validate_xpm_extension(char *path)
{
	int len = strlen(path);
	
	if (len < 4)
		return (0);
	
	return (strcmp(path + len - 4, ".xpm") == 0);
}

// Funzione per parsare una texture con gestione errori migliorata
int parse_texture(char *line, char **texture_path)
{
	char *path;
	int fd;
	
	if (!line || !texture_path)
	{
		printf("Error: Invalid parameters for texture parsing\n");
		return (1);
	}
		
	if (*texture_path) // Texture già assegnata (duplicato)
	{
		printf("Error: Duplicate texture definition\n");
		return (1);
	}
	
	path = trim_whitespace(line + 2); // Salta "NO", "SO", etc.
	
	if (strlen(path) == 0)
	{
		printf("Error: Empty texture path\n");
		return (1);
	}
	
	// Verifica se il path punta a un file esistente e accessibile
	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		printf("Error: Cannot access texture file: %s\n", path);
		return (1);
	}
	close(fd);
	
	// Verifica che sia un file regolare (non una directory)
	if (!is_regular_file(path))
	{
		printf("Error: Texture path is not a regular file: %s\n", path);
		return (1);
	}
	
	// Verifica che abbia estensione .xpm
	if (!validate_xpm_extension(path))
	{
		printf("Error: Texture file must have .xpm extension: %s\n", path);
		return (1);
	}
	
	*texture_path = strdup(path);
	if (!*texture_path)
	{
		printf("Error: Memory allocation failed for texture path\n");
		return (1);
	}
	
	return (0);
}

// Funzione per liberare le texture
void free_texture(t_texture *texture, void *mlx)
{
	if (!texture || !mlx)
		return;
		
	if (texture->img)
	{
		mlx_destroy_image(mlx, texture->img);
		texture->img = NULL;
	}
	
	memset(texture, 0, sizeof(t_texture));
}