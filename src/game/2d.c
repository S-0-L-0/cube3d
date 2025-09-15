/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2d.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:12:31 by edforte           #+#    #+#             */
/*   Updated: 2025/09/15 17:14:45 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_map_2d(t_game *game)
{
	int	map_x;
	int	map_y;
	int	screen_x;
	int	screen_y;
	int	pixel_x;
	int	pixel_y;

	map_y = 0;
	while (map_y < (game->map.height))
	{
		map_x = 0;
		while (map_x < (game->map.width))
		{
			screen_x = 0;
			while (screen_x < 32)
			{
				screen_y = 0;
				while (screen_y < 32)
				{
					pixel_x = map_x * 32 + screen_x;
					pixel_y = map_y * 32 + screen_y;
					if (game->map.grid[map_y][map_x] == '1')
						put_pixel(game, pixel_x, pixel_y, 0x000000);
					else
						put_pixel(game, pixel_x, pixel_y, 0xFFFFFF);
					screen_y++;
				}
				screen_x++;
			}
			map_x++;
		}
		map_y++;
	}
}

void	draw_player_2d(t_game *game)
{
	int	player_screen_x;
	int	player_screen_y;

	player_screen_x = (int)(game->player.pos_x * 32);
	player_screen_y = (int)(game->player.pos_y * 32);
	draw_circle(game, player_screen_x, player_screen_y, 2, 0x0000FF);
}
