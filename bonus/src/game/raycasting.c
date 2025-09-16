#include "../../includes/cub3d.h"

static void	init_ray(t_game *game, int screen_x)
{
	game->ray.hit = 0;
	game->ray.camera_x = ((2 * screen_x) / (double)game->mlx.win_width) - 1;
	game->ray.dir_x = game->player.dir_x
		+ (game->player.plane_x * game->ray.camera_x);
	game->ray.dir_y = game->player.dir_y
		+ (game->player.plane_y * game->ray.camera_x);
	game->ray.map_x = (int)game->player.pos_x;
	game->ray.map_y = (int)game->player.pos_y;
	game->ray.delta_dist_x = fabs(1.0 / game->ray.dir_x);
	game->ray.delta_dist_y = fabs(1.0 / game->ray.dir_y);
}

static void	init_steps(t_game *game)
{
	if (game->ray.dir_x < 0)
	{
		game->ray.step_x = -1;
		game->ray.side_dist_x = (game->player.pos_x - game->ray.map_x)
			* game->ray.delta_dist_x;
	}
	else
	{
		game->ray.step_x = 1;
		game->ray.side_dist_x = (game->ray.map_x + 1.0 - game->player.pos_x)
			* game->ray.delta_dist_x;
	}
	if (game->ray.dir_y < 0)
	{
		game->ray.step_y = -1;
		game->ray.side_dist_y = (game->player.pos_y - game->ray.map_y)
			* game->ray.delta_dist_y;
	}
	else
	{
		game->ray.step_y = 1;
		game->ray.side_dist_y = (game->ray.map_y + 1.0 - game->player.pos_y)
			* game->ray.delta_dist_y;
	}
}

static int	dda(t_game *game)
{
	int	side;

	side = 0;
	while (!game->ray.hit)
	{
		if (game->ray.side_dist_x < game->ray.side_dist_y)
		{
			game->ray.side_dist_x += game->ray.delta_dist_x;
			game->ray.map_x += game->ray.step_x;
			side = 0;
		}
		else
		{
			game->ray.side_dist_y += game->ray.delta_dist_y;
			game->ray.map_y += game->ray.step_y;
			side = 1;
		}
		if (game->ray.map_x >= 0 && game->ray.map_x < game->map.width
			&& game->ray.map_y >= 0 && game->ray.map_y < game->map.height)
		{
			if (game->map.grid[game->ray.map_y][game->ray.map_x] == '1')
			{
				game->ray.hit = 1;
				game->ray.door = 0;
			}
			else if (game->map.grid[game->ray.map_y][game->ray.map_x] == 'D')
			{
				game->ray.hit = 1;
				game->ray.door = 1;
			}
		}
	}
	return (side);
}

static void	calculate_perp_dist(t_game *game, int side)
{
	if (side == 0)
		game->ray.perp_wall_dist = game->ray.side_dist_x
			- game->ray.delta_dist_x;
	else
		game->ray.perp_wall_dist = game->ray.side_dist_y
			- game->ray.delta_dist_y;
}

static void	render_debug_hit(t_game *game, int side)
{
	double	intersection_x;
	double	intersection_y;

	intersection_x = game->player.pos_x
		+ (game->ray.dir_x * game->ray.perp_wall_dist);
	intersection_y = game->player.pos_y
		+ (game->ray.dir_y * game->ray.perp_wall_dist);
	if (side)
		draw_circle(game, intersection_x * 32,
			intersection_y * 32, 1, 0xFF0000);
	else
		draw_circle(game, intersection_x * 32,
			intersection_y * 32, 1, 0xFF00FF);
}

static void	init_render_params(t_game *game)
{
	game->render.line_height = (int)(game->mlx.win_height
			/ game->ray.perp_wall_dist);
	game->render.draw_start = -game->render.line_height
		/ 2 + game->mlx.win_height / 2;
	if (game->render.draw_start < 0)
		game->render.draw_start = 0;
	game->render.draw_end = game->render.line_height
		/ 2 + game->mlx.win_height / 2;
	if (game->render.draw_end >= game->mlx.win_height)
		game->render.draw_end = game->mlx.win_height - 1;
}

static void	calculate_texture_coords(t_game *game, int side)
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

static void	draw_wall_slice(t_game *game, int screen_x)
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
	while (y < game->render.draw_end)
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

static void	render_wall(t_game *game, int screen_x, int side)
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
	if (game->ray.hit && DEBUG)
		render_debug_hit(game, side);
	else if (game->ray.hit)
		render_wall(game, screen_x, side);
}
