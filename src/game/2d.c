/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2d.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:51:18 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 13:51:18 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	draw_tile(t_game *game, int map_x, int map_y, int color)
{
	int	screen_x;
	int	screen_y;

	screen_y = 0;
	while (screen_y < 32)
	{
		screen_x = 0;
		while (screen_x < 32)
		{
			put_pixel(game, (map_x * 32) + screen_x,
				(map_y * 32) + screen_y, color);
			screen_x++;
		}
		screen_y++;
	}
}

void	draw_map_2d(t_game *game)
{
	int	map_x;
	int	map_y;

	map_y = 0;
	while (map_y < game->map.height)
	{
		map_x = 0;
		while (map_x < game->map.width)
		{
			if (game->map.grid[map_y][map_x] == '1')
				draw_tile(game, map_x, map_y, 0x000000);
			else
				draw_tile(game, map_x, map_y, 0xFFFFFF);
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
