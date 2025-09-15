/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map_validation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:12:01 by edforte           #+#    #+#             */
/*   Updated: 2025/09/15 16:12:23 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	validate_map(t_map *map, t_player *player)
{
	if (validate_map_characters(map) != 0)
		return (1);
	if (check_map_borders(map) != 0)
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
				if (c == '\t')
					printf("Tab characters should be converted to spaces\n");
				else if (c >= 'A' && c <= 'Z' && c != 'N' && c != 'S'
					&& c != 'E' && c != 'W')
					printf("Only N, S, E, W are valid letters\n");
				else
					printf("Valid characters are: 0, 1, space\n");
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

int	check_map_borders(t_map *map)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	j = 0;
	while (j < map->width)
	{
		c = map->grid[0][j];
		if (c == '0')
		{
			printf("Error\nMap not closed, column: %d\n", j);
			return (1);
		}
		j++;
	}
	i = map->height - 1;
	j = 0;
	while (j < map->width)
	{
		c = map->grid[i][j];
		if (c == '0')
		{
			printf("Error\nMap not closed, column: %d\n", j);
			return (1);
		}
		j++;
	}
	i = 0;
	while (i < map->height)
	{
		c = map->grid[i][0];
		if (c == '0')
		{
			printf("Error\nMap not closed, row:  %d\n", i);
			return (1);
		}
		i++;
	}
	j = map->width - 1;
	i = 0;
	while (i < map->height)
	{
		c = map->grid[i][j];
		if (c == '0')
		{
			printf("Error\nMap not closed, row: %d\n", i);
			return (1);
		}
		i++;
	}
	return (0);
}
