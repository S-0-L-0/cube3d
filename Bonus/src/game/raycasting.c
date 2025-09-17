/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:51:29 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 13:51:29 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	calculate_texture_coords(t_game *game, int side)
{
	if (side == 0)
		game->render.wall_x = game->player.pos_y
			+ (game->ray.perp_wall_dist * game->ray.dir_y);
	else
		game->render.wall_x = game->player.pos_x
			+ (game->ray.perp_wall_dist * game->ray.dir_x);
	game->render.wall_x -= floor(game->render.wall_x);
	game->render.texture_x = (int)(game->render.wall_x
			* (double)game->render.texture->width);
	if (side == 0 && game->ray.dir_x < 0)
		game->render.texture_x = game->render.texture->width
			- game->render.texture_x - 1;
	if (side == 1 && game->ray.dir_y > 0)
		game->render.texture_x = game->render.texture->width
			- game->render.texture_x - 1;
}

void	draw_wall_slice(t_game *game, int screen_x)
{
	double			step;
	double			texture_pos;
	int				y;
	int				texture_y;
	unsigned int	color;

	step = (1.0 * game->render.texture->height) / game->render.line_height;
	texture_pos = (game->render.draw_start - game->mlx.win_height / 2
			+ game->render.line_height / 2) * step;
	y = game->render.draw_start;
	while (y <= game->render.draw_end)
	{
		texture_y = (int)texture_pos & (game->render.texture->height - 1);
		texture_pos += step;
		color = get_pixel_color(game->render.texture,
				game->render.texture_x, texture_y);
		if (color != 0xFF000000)
			put_pixel(game, screen_x, y, color);
		y++;
	}
}

void	render_wall(t_game *game, int screen_x, int side)
{
	init_render_params(game);
	if (game->ray.door)
		game->render.texture = &game->textures[10];
	else if (side == 0 && game->ray.dir_x > 0)
		game->render.texture = &game->textures[3];
	else if (side == 0)
		game->render.texture = &game->textures[2];
	else if (side == 1 && game->ray.dir_y > 0)
		game->render.texture = &game->textures[1];
	else
		game->render.texture = &game->textures[0];
	calculate_texture_coords(game, side);
	draw_wall_slice(game, screen_x);
}

void	raycasting(t_game *game, int screen_x)
{
	int	side;

	init_ray(game, screen_x);
	init_steps(game);
	side = dda(game);
	calculate_perp_dist(game, side);
	if (game->ray.hit)
		render_wall(game, screen_x, side);
}
