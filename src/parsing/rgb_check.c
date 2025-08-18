/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:42:34 by edforte           #+#    #+#             */
/*   Updated: 2025/05/25 18:26:24 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// Funzione per validare i valori RGB
int validate_rgb_values(int r, int g, int b)
{
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (1);
	return (0);
}

// Funzione per parsare i colori RGB con gestione errori
int parse_rgb_color(char *line, int *color)
{
	char *line_copy;
	char *token;
	int i = 0;
	int temp_color[3];
	
	if (!line || !color)
		return (1);
		
	if (color[0] != -1) // Colore già assegnato (duplicato)
		return (1);
	
	line_copy = strdup(line);
	if (!line_copy)
		return (1);
	
	token = strtok(line_copy, ",");
	while (token && i < 3)
	{
		token = trim_whitespace(token);
		if (strlen(token) == 0)
		{
			free(line_copy);
			return (1);
		}
		temp_color[i] = atoi(token);
		token = strtok(NULL, ",");
		i++;
	}
	
	free(line_copy);
	
	if (i != 3)
		return (1);
	
	if (validate_rgb_values(temp_color[0], temp_color[1], temp_color[2]))
		return (1);
		
	color[0] = temp_color[0];
	color[1] = temp_color[1];
	color[2] = temp_color[2];
	
	return (0);
}