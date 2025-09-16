/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_grid_operations.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:24 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 00:45:41 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	validate_content_after_map(t_parse_data *parse, int map_end)
{
	int	i;

	i = map_end + 1;
	while (parse->file_content[i])
	{
		if (!is_empty_line(parse->file_content[i]))
		{
			printf("Error\n");
			printf("Invalid content after map: %s\n", parse->file_content[i]);
			printf("Map must be the last element in the file\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	allocate_map_grid(t_map *map, int height)
{
	map->height = height;
	map->grid = (char **)malloc(sizeof(char *) * (map->height + 1));
	if (!map->grid)
	{
		printf("Error\nMemory allocation failed for map grid\n");
		return (1);
	}
	return (0);
}

void	cleanup_map_grid(t_map *map)
{
	int	i;

	i = 0;
	while (map->grid[i])
	{
		free(map->grid[i]);
		i++;
	}
	free(map->grid);
	map->grid = NULL;
}

int	copy_map_rows(t_parse_data *parse, t_map *map, int map_start)
{
	int	i;

	i = 0;
	while (i < map->height)
	{
		map->grid[i] = ft_strdup(parse->file_content[map_start + i]);
		if (!map->grid[i])
		{
			printf("Error\nMemory allocation failed for map row %d\n", i);
			while (i > 0)
			{
				i--;
				free(map->grid[i]);
			}
			free(map->grid);
			map->grid = NULL;
			return (1);
		}
		i++;
	}
	map->grid[map->height] = NULL;
	return (0);
}
