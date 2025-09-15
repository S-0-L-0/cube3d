/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:36:23 by edforte           #+#    #+#             */
/*   Updated: 2025/09/15 17:41:13 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	mouse_hook(int x, int y, t_game *game)
{
	(void)y;
	if (x < game->mlx.win_width / 2)
	{
		rot_player(game, -1.0, 5.0);
	}
	else if (x > game->mlx.win_width / 2)
	{
		rot_player(game, 1.0, 5.0);
	}
	return (0);
}

int	key_press(int keycode, t_game *game)
{
	if (keycode == 65307)
	{
		printf("ESC pressed, closing game...\n");
		free_game(game);
		exit(0);
	}
	if (keycode == 119)
	{
		printf("W pressed\n");
		game->keys.w = true;
	}
	if (keycode == 115)
	{
		printf("S pressed\n");
		game->keys.s = true;
	}
	if (keycode == 97)
	{
		printf("A pressed\n");
		game->keys.a = true;
	}
	if (keycode == 100)
	{
		printf("D pressed\n");
		game->keys.d = true;
	}
	else if (keycode == 65361)
	{
		printf("Left arrow pressed\n");
		game->keys.left = true;
	}
	else if (keycode == 65363)
	{
		printf("Right arrow pressed\n");
		game->keys.right = true;
	}
	return (0);
}

int	key_release(int keycode, t_game *game)
{
	if (keycode == 119)
		game->keys.w = false;
	if (keycode == 115)
		game->keys.s = false;
	if (keycode == 97)
		game->keys.a = false;
	if (keycode == 100)
		game->keys.d = false;
	if (keycode == 65361)
		game->keys.left = false;
	if (keycode == 65363)
		game->keys.right = false;
	return (0);
}

int	close_window(t_game *game)
{
	printf("Window close button pressed\n");
	free_game(game);
	exit(0);
	return (0);
}
