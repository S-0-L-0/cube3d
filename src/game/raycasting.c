#include "../../includes/cub3d.h"

void	put_pixel(t_game *game, int x, int y, int color)
{
	char *dst;
	if (x >= game->mlx.win_width || y >= game->mlx.win_height || x < 0 || y < 0)
		return ;
	dst = game->mlx.addr + (y * game->mlx.line_length + x * (game->mlx.bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void draw_circle(t_game *game, int cx, int cy, int radius, int color)
{
	int x; 
	int y;
	int r2;
	
	r2 = radius * radius;
	for (y = -radius; y <= radius; y++)
	{
		for (x = -radius; x <= radius; x++)
		{
			if (x*x + y*y <= r2)
				put_pixel(game, cx + x, cy + y, color);
		}
	}
}

void	draw_map_2d(t_game *game)
{
	int	map_x; 
	int	map_y;
	int	screen_x;
	int	screen_y;

	map_y = 0;
	while (map_y < game->map.height)
	{
		map_x = 0;
		while (map_x < game->map.width)
		{
			screen_x = 0;
			while (screen_x < 32)
			{
				screen_y = 0;
				while (screen_y < 32)
				{
					int pixel_x = map_x * 32 + screen_x;
					int pixel_y = map_y * 32 + screen_y;
					
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
	int player_screen_x;
	int player_screen_y;

	player_screen_x = (int)(game->player.pos_x * 32);
	player_screen_y = (int)(game->player.pos_y * 32);
//	draw_square(game, player_screen_x, player_screen_y, 2, 0x0000FF);
	draw_circle(game, player_screen_x, player_screen_y, 2, 0x0000FF);
}

void	draw_ray_2d(t_game *game, int screen_x)
{
   	double distance;
	double max_distance;

    distance = 0;
    max_distance = 100;
    game->ray.hit = 0;
	game->ray.camera_x = 2 * screen_x / (double)game->mlx.win_width - 1;
	game->ray.dir_x = game->player.dir_x + game->player.plane_x * game->ray.camera_x;
	game->ray.dir_y = game->player.dir_y + game->player.plane_y * game->ray.camera_x;
	
	game->ray.map_x = (int)game->player.pos_x;
	game->ray.map_y = (int)game->player.pos_y;

	if (game->ray.dir_x == 0)
		game->ray.delta_dist_x = 1e30;
	else
		game->ray.delta_dist_x = fabs(1.0 / game->ray.dir_x);
	
	if (game->ray.dir_y == 0)
		game->ray.delta_dist_y = 1e30;
	else
		game->ray.delta_dist_y = fabs(1.0 / game->ray.dir_y);
	if (game->ray.dir_x < 0)
	{
		game->ray.step_x = -1;
		game->ray.side_dist_x = (game->player.pos_x - game->ray.map_x) * game->ray.delta_dist_x;
	}
	else
	{
		game->ray.step_x = 1;
		game->ray.side_dist_x = (game->ray.map_x + 1.0 - game->player.pos_x) * game->ray.delta_dist_x;
	}
	if (game->ray.dir_y < 0)
	{
		game->ray.step_y = -1;
		game->ray.side_dist_y = (game->player.pos_y - game->ray.map_y) * game->ray.delta_dist_y;
	}
	else
	{
		game->ray.step_y = 1;
		game->ray.side_dist_y = (game->ray.map_y + 1.0 - game->player.pos_y) * game->ray.delta_dist_y;
	}
	while (!game->ray.hit && distance < max_distance)
	{
		if (game->ray.side_dist_x < game->ray.side_dist_y)
		{
			game->ray.map_x += game->ray.step_x;
			distance = game->ray.side_dist_x;
			game->ray.side_dist_x += game->ray.delta_dist_x;
		}
		else
		{
			game->ray.map_y += game->ray.step_y;
			distance = game->ray.side_dist_y;
			game->ray.side_dist_y += game->ray.delta_dist_y;
		}
        if (game->ray.map_x >= 0 && game->ray.map_x < game->map.width && game->ray.map_y >= 0 && game->ray.map_y < game->map.height)
        {
            if (game->map.grid[game->ray.map_y][game->ray.map_x] == '1')
                game->ray.hit = 1;
        }	}
	if (game->ray.hit)
	{
		double intersection_x = game->player.pos_x + game->ray.dir_x * distance;
		double intersection_y = game->player.pos_y + game->ray.dir_y * distance;
		draw_circle(game, intersection_x * 32, intersection_y * 32, 2, 0xFF0000);
	}
}
