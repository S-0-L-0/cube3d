/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:33:46 by edforte           #+#    #+#             */
/*   Updated: 2025/09/16 17:42:23 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	render_test_screen(t_game *game)
{
	int		x;
	int		y;
	int		color;
	char	*dst;

	y = 0;
	x = 0;
	while (y < game->mlx.win_height)
	{
		x = 0;
		while (x < game->mlx.win_width)
		{
			dst = game->mlx.addr + (y * game->mlx.line_length + x
					* (game->mlx.bits_per_pixel / 8));
			if (y < game->mlx.win_height / 2)
				color = (game->map.sky_col[0] << 16)
					| (game->map.sky_col[1] << 8) | game->map.sky_col[2];
			else
				color = (game->map.floor_col[0] << 16)
					| (game->map.floor_col[1] << 8) | game->map.floor_col[2];
			*(unsigned int *)dst = color;
			x ++;
		}
		y ++;
	}
}

void	drawcast(t_game *game)
{
	int	x;

	x = 0;
	while (x < game->mlx.win_width)
	{
		raycasting(game, x);
		x++;
	}
	if (BONUS)
	{
		draw_minimap(game);
	}
}

int	render_frame(t_game *game)
{
	game->time.old_time = game->time.time;
	game->time.time = get_time_ms();
	game->time.frame_time = (game->time.time - game->time.old_time) / 1000.0;
	update_player(game);
	mlx_mouse_move(game->mlx.mlx, game->mlx.win, game->mlx.win_width / 2,
		game->mlx.win_height / 2);
	render_test_screen(game);
	drawcast(game);
	draw_torch(game);
	draw_crosshair(game);
	mlx_put_image_to_window(game->mlx.mlx, game->mlx.win, game->mlx.img, 0, 0);
	check_door(game);
	return (0);
}

static void	print_pn(void)
{
	char	*line;
	int		fd;

	fd = open("assets/scritta.txt", O_RDONLY);
	line = get_next_line(fd);
	while (line != NULL)
	{
		printf("\033[38;5;46m");
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
}

int	game_loop(t_game *game)
{
	game->time.time = get_time_ms();
	game->time.old_time = get_time_ms();
	game->time.frame_time = 0;
	game->time.start_time = get_time_ms();
	printf("Starting game loop...\n");
	printf("Window size: %dx%d\n", game->mlx.win_width, game->mlx.win_height);
	printf("Player position: (%.2f, %.2f)\n", game->player.pos_x,
		game->player.pos_y);
	printf("Map size: %dx%d\n", game->map.width, game->map.height);
	print_pn();
	mlx_hook(game->mlx.win, 2, 1L << 0, key_press, game);
	mlx_hook(game->mlx.win, 3, 1L << 1, key_release, game);
	mlx_hook(game->mlx.win, 17, 1L << 17, close_window, game);
	mlx_hook(game->mlx.win, 6, 1L << 6, mouse_hook, game);
	mlx_loop_hook(game->mlx.mlx, render_frame, game);
	mlx_mouse_hide(game->mlx.mlx, game->mlx.win);
	printf("Press ESC to quit, W/A/S/D to test movement\n");
	mlx_loop(game->mlx.mlx);
	return (0);
}
