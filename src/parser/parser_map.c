/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:11:56 by edforte           #+#    #+#             */
/*   Updated: 2025/09/16 16:55:22 by edforte          ###   ########.fr       */
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

int	parse_map(t_parse_data *parse, t_map *map, t_player *player)
{
	int	map_start;
	int	map_end;

	if (find_map_boundaries(parse, &map_start, &map_end) != 0)
		return (1);
	if (validate_content_after_map(parse, map_end) != 0)
		return (1);
	if (allocate_map_grid(map, map_end - map_start + 1) != 0)
		return (1);
	if (copy_map_rows(parse, map, map_start) != 0)
		return (1);
	if (normalize_map_width(map) != 0)
	{
		cleanup_map_grid(map);
		return (1);
	}
	if (extract_player_position(map, player) != 0)
	{
		cleanup_map_grid(map);
		return (1);
	}
	return (0);
}

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

int	normalize_map_width(t_map *map)
{
	int	max_width;

	max_width = find_max_width(map);
	map->width = max_width;
	if (validate_map_size(map) != 0)
		return (1);
	return (pad_all_rows(map, max_width));
}

int	validate_map_start(t_parse_data *parse)
{
	if (parse->map_start_line == 0)
	{
		printf("Error\nNo map found in file\n");
		return (1);
	}
	return (0);
}

int	scan_map_end(t_parse_data *parse, int start, int *end)
{
	int	i;

	i = start;
	*end = start;
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
		*end = i;
		i++;
	}
	return (0);
}

int	validate_map_height(int map_start, int map_end)
{
	int	height;

	height = map_end - map_start + 1;
	if (height < 3)
	{
		printf("Error\nMap too small (minimum 3 rows)\n");
		return (1);
	}
	if (height > 999994)
	{
		printf("Error\nMap too large (maximum 999994 rows)\n");
		printf("Current map has %d rows\n", height);
		return (1);
	}
	return (0);
}

int	find_map_boundaries(t_parse_data *parse, int *map_start, int *map_end)
{
	if (validate_map_start(parse) != 0)
		return (1);
	
	*map_start = parse->map_start_line;
	
	if (scan_map_end(parse, *map_start, map_end) != 0)
		return (1);
	
	return (validate_map_height(*map_start, *map_end));
}

void	initialize_player_data(t_player *player)
{
	player->pos_x = -1.0;
	player->pos_y = -1.0;
	player->direction = 0;
}

int	is_player_character(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

int	handle_multiple_players(t_player *player, int i, int j)
{
	printf("Error\nMultiple player found\n");
	printf("First at [%d,%d], second at [%d,%d]\n",
		(int)player->pos_y, (int)player->pos_x, i, j);
	return (1);
}

void	set_player_direction_vectors(t_player *player, char direction)
{
	if (direction == 'N' || direction == 'S')
	{
		player->dir_x = 0.0;
		player->plane_y = 0.0;
		player->dir_y = -1.0;
		player->plane_x = 0.66;
		if (direction == 'S')
		{
			player->dir_y = 1.0;
			player->plane_x = -0.66;
		}
	}
	else if (direction == 'E' || direction == 'W')
	{
		player->plane_x = 0.0;
		player->dir_y = 0.0;
		player->dir_x = 1.0;
		player->plane_y = 0.66;
		if (direction == 'W')
		{
			player->dir_x = -1.0;
			player->plane_y = -0.66;
		}
	}
}

int	process_player_found(t_map *map, t_player *player, int i, int j, int *player_count)
{
	char	c;

	if (*player_count > 0)
		return (handle_multiple_players(player, i, j));
	
	c = map->grid[i][j];
	player->pos_x = (double)j + 0.5;
	player->pos_y = (double)i + 0.5;
	player->direction = c;
	(*player_count)++;
	set_player_direction_vectors(player, c);
	map->grid[i][j] = '0';
	return (0);
}

int	scan_for_player(t_map *map, t_player *player)
{
	int		i;
	int		j;
	int		player_count;

	player_count = 0;
	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			if (is_player_character(map->grid[i][j]))
			{
				if (process_player_found(map, player, i, j, &player_count) != 0)
					return (1);
			}
			j++;
		}
		i++;
	}
	if (player_count == 0)
		return (printf("Error\nNo player start_position found in map\n"), 1);
	return (0);
}

int	validate_player_position(t_player *player, t_map *map)
{
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

int	extract_player_position(t_map *map, t_player *player)
{
	initialize_player_data(player);
	if (scan_for_player(map, player) != 0)
		return (1);
	return (validate_player_position(player, map));
}
