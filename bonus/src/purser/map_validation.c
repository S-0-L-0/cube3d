/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:29 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 00:53:50 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	validate_map(t_map *map, t_player *player)
{
	if (validate_map_characters(map) != 0)
		return (1);
	if (flood_fill_check(map, player) != 0)
		return (1);
	return (0);
}

int	validate_map_characters(t_map *map)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			c = map->grid[i][j];
			if (c != '0' && c != '1' && c != ' ')
			{
				printf("Error\nInvalid character '%c' line:row [%d,%d]\n",
					c, i, j);
				return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	flood_fill_check(t_map *map, t_player *player)
{
	int	**visited;
	int	result;

	visited = allocate_2d_int_array(map->height, map->width);
	if (!visited)
	{
		printf("Error\nMemory allocation failed for flood fill\n");
		return (1);
	}
	result = flood_fill_recursive(map, visited, (int)player->pos_x,
			(int)player->pos_y);
	free_2d_int_array(visited, map->height);
	visited = NULL;
	if (result != 0)
	{
		printf("Error\nMap is not properly closed\n");
		printf("Player can reach void spaces or map boundaries\n");
		return (1);
	}
	return (0);
}

int	flood_fill_recursive(t_map *map, int **visited, int x, int y)
{
	char	cell;

	if (x < 0 || x >= map->width || y < 0 || y >= map->height)
	{
		return (1);
	}
	if (visited[y][x])
		return (0);
	visited[y][x] = 1;
	cell = map->grid[y][x];
	if (cell == ' ')
	{
		return (printf("Error\nAccessible void space on [%d,%d]\n", y, x), 1);
	}
	if (cell == '1')
		return (0);
	if (flood_fill_recursive(map, visited, x + 1, y) != 0)
		return (1);
	if (flood_fill_recursive(map, visited, x - 1, y) != 0)
		return (1);
	if (flood_fill_recursive(map, visited, x, y + 1) != 0)
		return (1);
	if (flood_fill_recursive(map, visited, x, y - 1) != 0)
		return (1);
	return (0);
}
