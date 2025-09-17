/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:50:50 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 13:50:51 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	toggle_door(t_game *game)
{
	int	target_x;
	int	target_y;

	target_x = (int)(game->player.pos_x + game->player.dir_x);
	target_y = (int)(game->player.pos_y + game->player.dir_y);
	if (target_x >= 0 && target_x < game->map.width
		&& target_y >= 0 && target_y < game->map.height)
	{
		if (game->map.grid[target_y][target_x] == 'D')
			game->map.grid[target_y][target_x] = 'd';
		else if (game->map.grid[target_y][target_x] == 'd')
		{
			if ((int)game->player.pos_x != target_x
				|| (int)game->player.pos_y != target_y)
				game->map.grid[target_y][target_x] = 'D';
		}
	}
}

void	check_door(t_game *game)
{
	int	target_x;
	int	target_y;

	target_x = (int)(game->player.pos_x + game->player.dir_x);
	target_y = (int)(game->player.pos_y + game->player.dir_y);
	if (target_x >= 0 && target_x < game->map.width
		&& target_y >= 0 && target_y < game->map.height)
	{
		if (game->map.grid[target_y][target_x] == 'D')
			mlx_string_put(game->mlx.mlx, game->mlx.win,
				game->mlx.win_width / 2 - 65,
				game->mlx.win_height / 2 - 40, 0xFFFFFF,
				"Press E to close the door");
		else if (game->map.grid[target_y][target_x] == 'd')
		{
			mlx_string_put(game->mlx.mlx, game->mlx.win,
				game->mlx.win_width / 2 - 65,
				game->mlx.win_height / 2 - 40, 0xFFFFFF,
				"Press E to open the door");
		}
	}
}
