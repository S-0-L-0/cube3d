/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_width_normalization.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:31 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 00:47:18 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	find_max_width(t_map *map)
{
	int	max_width;
	int	current_len;
	int	i;

	max_width = 0;
	i = 0;
	while (i < map->height)
	{
		current_len = ft_strlen(map->grid[i]);
		if (current_len > max_width)
			max_width = current_len;
		i++;
	}
	return (max_width);
}

int	validate_map_size(t_map *map)
{
	if (map->width < 3 || map->height < 3)
	{
		printf("Error\nMap too small (minimum 3x3)\n");
		return (1);
	}
	return (0);
}

char	*create_padded_row(char *original_row, int current_len, int max_width)
{
	char	*new_row;
	int		j;

	new_row = (char *)malloc(sizeof(char) * (max_width + 1));
	if (!new_row)
	{
		printf("Error\nMemory allocation failed during normalization\n");
		return (NULL);
	}
	j = 0;
	while (j < current_len)
	{
		new_row[j] = original_row[j];
		j++;
	}
	while (j < max_width)
	{
		new_row[j] = ' ';
		j++;
	}
	new_row[j] = '\0';
	return (new_row);
}

int	pad_all_rows(t_map *map, int max_width)
{
	int		current_len;
	int		i;
	char	*new_row;

	i = 0;
	while (i < map->height)
	{
		current_len = ft_strlen(map->grid[i]);
		if (current_len == max_width)
		{
			i++;
			continue ;
		}
		new_row = create_padded_row(map->grid[i], current_len, max_width);
		if (!new_row)
			return (1);
		free(map->grid[i]);
		map->grid[i] = new_row;
		i++;
	}
	return (0);
}
