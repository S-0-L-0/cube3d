/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:50:11 by edforte           #+#    #+#             */
/*   Updated: 2025/09/15 17:56:03 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

#define TILE_SIZE 12
#define VIEW_RADIUS 5
#define MINIMAP_DIM 11

void	draw_minimap(t_game *game)
{
	int	minimap_x;
	int	minimap_y;
	int	map_to_read_x;
	int	map_to_read_y;
	int	color;
	int	screen_x;
	int	screen_y;
	int	player_pixel_x;
	int	player_pixel_y;
	int	pixel_x;
	int	pixel_y;

	minimap_y = 0;
	while (minimap_y < MINIMAP_DIM)
	{
		minimap_x = 0;
		while (minimap_x < MINIMAP_DIM)
		{
			map_to_read_x = (int)game->player.pos_x - VIEW_RADIUS + minimap_x;
			map_to_read_y = (int)game->player.pos_y - VIEW_RADIUS + minimap_y;
			color = (game->map.floor_col[0] << 16)
				| (game->map.floor_col[1] << 8) | game->map.floor_col[2];
			if (map_to_read_y >= 0 && map_to_read_y < game->map.height
				&& map_to_read_x >= 0 && map_to_read_x < game->map.width
				&& game->map.grid[map_to_read_y][map_to_read_x] == '1')
			{
				color = 0x939393;
			}
			screen_x = 0;
			while (screen_x < TILE_SIZE)
			{
				screen_y = 0;
				while (screen_y < TILE_SIZE)
				{
					pixel_x = minimap_x * TILE_SIZE + screen_x;
					pixel_y = minimap_y * TILE_SIZE + screen_y;
					put_pixel(game, pixel_x, pixel_y, color);
					screen_y++;
				}
				screen_x++;
			}
			minimap_x++;
		}
		minimap_y++;
	}
	player_pixel_x = VIEW_RADIUS * TILE_SIZE + TILE_SIZE / 2;
	player_pixel_y = VIEW_RADIUS * TILE_SIZE + TILE_SIZE / 2;
	draw_circle(game, player_pixel_x, player_pixel_y, 3, 0xFFFFFF);
}
