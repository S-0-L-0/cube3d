/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_data_management.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:44 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 13:53:55 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	initialize_player_data(t_player *player)
{
	player->pos_x = -1.0;
	player->pos_y = -1.0;
	player->direction = 0;
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
