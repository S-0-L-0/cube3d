#include "../../includes/cub3d.h"

void	init_ray(t_game *game, int screen_x)
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

void	init_steps(t_game *game)
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

void	init_render_params(t_game *game)
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

void	calculate_perp_dist(t_game *game, int side)
{
	if (side == 0)
		game->ray.perp_wall_dist = game->ray.side_dist_x
			- game->ray.delta_dist_x;
	else
		game->ray.perp_wall_dist = game->ray.side_dist_y
			- game->ray.delta_dist_y;
}
