/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:03:26 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 16:03:26 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	advance_ray(t_game *game, int *side)
{
	if (game->ray.side_dist_x < game->ray.side_dist_y)
	{
		game->ray.side_dist_x += game->ray.delta_dist_x;
		game->ray.map_x += game->ray.step_x;
		*side = 0;
	}
	else
	{
		game->ray.side_dist_y += game->ray.delta_dist_y;
		game->ray.map_y += game->ray.step_y;
		*side = 1;
	}
}

static void	check_cell(t_game *game)
{
	char	c;

	if (game->ray.map_x < 0 || game->ray.map_x >= game->map.width
		|| game->ray.map_y < 0 || game->ray.map_y >= game->map.height)
		return ;
	c = game->map.grid[game->ray.map_y][game->ray.map_x];
	if (c == '1' || c == 'D')
	{
		game->ray.hit = 1;
		game->ray.door = (c == 'D');
	}
}

int	dda(t_game *game)
{
	int	side;

	side = 0;
	while (!game->ray.hit)
	{
		advance_ray(game, &side);
		check_cell(game);
	}
	return (side);
}
