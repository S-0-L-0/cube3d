/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_position_extraction.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:47 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 00:52:46 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	process_player_found(t_map *map, t_player *player, int i, int j)
{
	char	c;

	c = map->grid[i][j];
	player->pos_x = (double)j + 0.5;
	player->pos_y = (double)i + 0.5;
	player->direction = c;
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
	i = -1;
	while (++i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			if (is_player_character(map->grid[i][j]))
			{
				if (player_count > 0)
					return (handle_multiple_players(player, i, j));
				if (process_player_found(map, player, i, j) != 0)
					return (1);
				player_count++;
			}
			j++;
		}
	}
	if (player_count == 0)
		return (printf("Error\nNo player starting position found\n"), 1);
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
