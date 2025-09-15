/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:11:56 by edforte           #+#    #+#             */
/*   Updated: 2025/09/15 16:43:11 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	parse_map(t_parse_data *parse, t_map *map, t_player *player)
{
	int	map_start;
	int	map_end;
	int	i;

	if (find_map_boundaries(parse, &map_start, &map_end) != 0)
		return (1);
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
	map->height = map_end - map_start + 1;
	map->grid = (char **)malloc(sizeof(char *) * (map->height + 1));
	if (!map->grid)
	{
		printf("Error\nMemory allocation failed for map grid\n");
		return (1);
	}
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
	if (normalize_map_width(map) != 0)
	{
		i = 0;
		while (map->grid[i])
		{
			free(map->grid[i]);
			i++;
		}
		free(map->grid);
		map->grid = NULL;
		return (1);
	}
	if (extract_player_position(map, player) != 0)
	{
		i = 0;
		while (map->grid[i])
		{
			free(map->grid[i]);
			i++;
		}
		free(map->grid);
		map->grid = NULL;
		return (1);
	}
	return (0);
}

int	normalize_map_width(t_map *map)
{
	int		max_width;
	int		current_len;
	int		i;
	int		j;
	char	*new_row;

	max_width = 0;
	i = 0;
	while (i < map->height)
	{
		current_len = ft_strlen(map->grid[i]);
		if (current_len > max_width)
			max_width = current_len;
		i++;
	}
	map->width = max_width;
	if (map->width < 3 || map->height < 3)
	{
		printf("Error\nMap too small (minimum 3x3)\n");
		return (1);
	}
	i = 0;
	while (i < map->height)
	{
		current_len = ft_strlen(map->grid[i]);
		if (current_len == max_width)
		{
			i++;
			continue ;
		}
		new_row = (char *)malloc(sizeof(char) * (max_width + 1));
		if (!new_row)
		{
			printf("Error\nMemory allocation failed during normalization\n");
			return (1);
		}
		j = 0;
		while (j < current_len)
		{
			new_row[j] = map->grid[i][j];
			j++;
		}
		while (j < max_width)
		{
			new_row[j] = ' ';
			j++;
		}
		new_row[j] = '\0';
		free(map->grid[i]);
		map->grid[i] = new_row;
		i++;
	}
	return (0);
}

int	find_map_boundaries(t_parse_data *parse, int *map_start, int *map_end)
{
	int	i;

	if (parse->map_start_line == 0)
	{
		printf("Error\nNo map found in file\n");
		return (1);
	}
	*map_start = parse->map_start_line;
	i = *map_start;
	*map_end = *map_start;
	while (parse->file_content[i])
	{
		if (is_empty_line(parse->file_content[i]))
		{
			break ;
		}
		if (!is_map_line(parse->file_content[i]))
		{
			printf("Error\nInvalid line in map: %s\n", parse->file_content[i]);
			return (1);
		}
		*map_end = i;
		i++;
	}
	if (*map_end - *map_start + 1 < 3)
	{
		printf("Error\nMap too small (minimum 3 rows)\n");
		return (1);
	}
	if (*map_end - *map_start + 1 > 999994)
	{
		printf("Error\nMap too large (maximum 999994 rows)\n");
		printf("Current map has %d rows\n", *map_end - *map_start + 1);
		return (1);
	}
	return (0);
}

int	extract_player_position(t_map *map, t_player *player)
{
	int		i;
	int		j;
	int		player_count;
	char	c;

	player_count = 0;
	player->pos_x = -1.0;
	player->pos_y = -1.0;
	player->direction = 0;
	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			c = map->grid[i][j];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				if (player_count > 0)
				{
					printf("Error\nMultiple player found\n");
					printf("First at [%d,%d], second at [%d,%d]\n",
						(int)player->pos_y, (int)player->pos_x, i, j);
					return (1);
				}
				player->pos_x = (double)j + 0.5;
				player->pos_y = (double)i + 0.5;
				player->direction = c;
				player_count++;
				if (map->grid[i][j] == 'N')
				{
					player->dir_x = 0.0;
					player->dir_y = -1.0;
					player->plane_x = 0.66;
					player->plane_y = 0;
				}
				else if (map->grid[i][j] == 'S')
				{
					player->dir_x = 0.0;
					player->dir_y = 1.0;
					player->plane_x = -0.66;
					player->plane_y = 0.0;
				}
				else if (map->grid[i][j] == 'E')
				{
					player->dir_x = 1.0;
					player->dir_y = 0.0;
					player->plane_x = 0.0;
					player->plane_y = 0.66;
				}
				else if (map->grid[i][j] == 'W')
				{
					player->dir_x = -1.0;
					player->dir_y = 0.0;
					player->plane_x = 0.0;
					player->plane_y = -0.66;
				}
				map->grid[i][j] = '0';
			}
			j++;
		}
		i++;
	}
	if (player_count == 0)
	{
		printf("Error\nNo player starting position found in map\n");
		printf("Use N, S, E, or W to mark player position\n");
		return (1);
	}
	if ((int)(player->pos_x - 0.5) == 0
		|| (int)(player->pos_x - 0.5) == map->width - 1
		|| (int)(player->pos_y - 0.5) == 0
		|| (int)(player->pos_y - 0.5) == map->height - 1)
	{
		printf("Error\nPlayer cannot start on map border\n");
		return (1);
	}
	return (0);
}
