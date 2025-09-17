/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:50:57 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 13:50:57 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_minimap_tile(t_game *game, int map_x, int map_y, int color)
{
	int	screen_x;
	int	screen_y;

	screen_y = 0;
	while (screen_y < TILE_SIZE)
	{
		screen_x = 0;
		while (screen_x < TILE_SIZE)
		{
			put_pixel(game, (map_x * TILE_SIZE) + screen_x,
				(map_y * TILE_SIZE) + screen_y, color);
			screen_x++;
		}
		screen_y++;
	}
}

static int	get_tile_color(t_game *game, int minimap_x, int minimap_y)
{
	int	map_to_read_x;
	int	map_to_read_y;

	map_to_read_x = (int)game->player.pos_x - VIEW_RADIUS + minimap_x;
	map_to_read_y = (int)game->player.pos_y - VIEW_RADIUS + minimap_y;
	if (map_to_read_y >= 0 && map_to_read_y < game->map.height
		&& map_to_read_x >= 0 && map_to_read_x < game->map.width
		&& game->map.grid[map_to_read_y][map_to_read_x] == '1')
	{
		return (0x939393);
	}
	return ((game->map.floor_col[0] << 16)
		| (game->map.floor_col[1] << 8) | game->map.floor_col[2]);
}

static void	draw_player_on_minimap(t_game *game)
{
	int	player_pixel_x;
	int	player_pixel_y;

	player_pixel_x = VIEW_RADIUS * TILE_SIZE + TILE_SIZE / 2;
	player_pixel_y = VIEW_RADIUS * TILE_SIZE + TILE_SIZE / 2;
	draw_circle(game, player_pixel_x, player_pixel_y, 3);
}

void	draw_minimap(t_game *game)
{
	int	minimap_x;
	int	minimap_y;
	int	color;

	minimap_y = 0;
	while (minimap_y < MINIMAP_DIM)
	{
		minimap_x = 0;
		while (minimap_x < MINIMAP_DIM)
		{
			color = get_tile_color(game, minimap_x, minimap_y);
			draw_minimap_tile(game, minimap_x, minimap_y, color);
			minimap_x++;
		}
		minimap_y++;
	}
	draw_player_on_minimap(game);
}
