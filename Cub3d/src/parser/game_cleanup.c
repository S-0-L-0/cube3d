/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 01:06:19 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 01:06:39 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	cleanup_parse_data(t_parse_data *parse)
{
	if (!parse)
		return ;
	if (parse->file_content)
	{
		free_split(parse->file_content);
		parse->file_content = NULL;
	}
	parse->map_start_line = -1;
	parse->texture_count = 0;
	parse->floor_set = 0;
	parse->ceiling_set = 0;
	parse->north_loaded = 0;
	parse->south_loaded = 0;
	parse->east_loaded = 0;
	parse->west_loaded = 0;
}

void	reset_player_pos(t_game *game)
{
	game->player.pos_x = 0.0;
	game->player.pos_y = 0.0;
	game->player.dir_x = 0.0;
	game->player.dir_y = 0.0;
	game->player.plane_x = 0.0;
	game->player.plane_y = 0.0;
	game->player.direction = 0;
}

void	cleanup_game(t_game *game)
{
	int	i;

	if (game->map.grid)
	{
		i = 0;
		while (game->map.grid[i])
		{
			free(game->map.grid[i]);
			game->map.grid[i] = NULL;
			i++;
		}
		free(game->map.grid);
		game->map.grid = NULL;
	}
	free_game_texture(game);
	game->map.width = 0;
	game->map.height = 0;
	i = 0;
	while (i < 3)
	{
		game->map.floor_col[i] = 0;
		game->map.sky_col[i] = 0;
		i++;
	}
	reset_player_pos(game);
}

void	free_map_grid(t_map *map)
{
	int	i;

	if (!map || !map->grid)
		return ;
	i = 0;
	while (map->grid[i])
	{
		free(map->grid[i]);
		map->grid[i] = NULL;
		i++;
	}
	free(map->grid);
	map->grid = NULL;
	map->width = 0;
	map->height = 0;
}
