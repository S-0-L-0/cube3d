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

#define TILE_SIZE 8
#define VIEW_RADIUS 5
#define MINIMAP_DIM (VIEW_RADIUS * 2 + 1)

void    draw_map_2d(t_game *game)
{
	int minimap_x;
	int minimap_y;
	int map_to_read_x;
	int map_to_read_y;

	minimap_y = 0;
	while (minimap_y < MINIMAP_DIM)
	{
		minimap_x = 0;
		while (minimap_x < MINIMAP_DIM)
		{
			map_to_read_x = (int)game->player.pos_x - VIEW_RADIUS + minimap_x;
			map_to_read_y = (int)game->player.pos_y - VIEW_RADIUS + minimap_y;

			int color = 0xFFFFFF;
			
			if (map_to_read_y >= 0 && map_to_read_y < game->map.height &&
				map_to_read_x >= 0 && map_to_read_x < game->map.width &&
				game->map.grid[map_to_read_y][map_to_read_x] == '1')
			{
				color = 0x000000;
			}
			int screen_x = 0;
			while (screen_x < TILE_SIZE)
			{
				int screen_y = 0;
				while (screen_y < TILE_SIZE)
				{
					int pixel_x = minimap_x * TILE_SIZE + screen_x;
					int pixel_y = minimap_y * TILE_SIZE + screen_y;
					put_pixel(game, pixel_x, pixel_y, color);
					screen_y++;
				}
				screen_x++;
			}
			minimap_x++;
		}
		minimap_y++;
	}
	int player_pixel_x = VIEW_RADIUS * TILE_SIZE + TILE_SIZE / 2;
	int player_pixel_y = VIEW_RADIUS * TILE_SIZE + TILE_SIZE / 2;
	draw_circle(game, player_pixel_x, player_pixel_y, 2, 0xFF0000);
}

void	draw_player_2d(t_game *game)
{
	int player_screen_x;
	int player_screen_y;

	player_screen_x = (int)(game->player.pos_x * 8);
	player_screen_y = (int)(game->player.pos_y * 8);
//	draw_square(game, player_screen_x, player_screen_y, 2, 0x0000FF);
	draw_circle(game, player_screen_x, player_screen_y, 2, 0x0000FF);
}

void	raycasting(t_game *game, int screen_x)
{
	int		side;

	side = 0;
	game->ray.hit = 0;
	game->ray.camera_x = ((2 * screen_x) / (double)game->mlx.win_width) - 1; // [-1, 0, 1]
	game->ray.dir_x = game->player.dir_x + (game->player.plane_x * game->ray.camera_x);
	game->ray.dir_y = game->player.dir_y + (game->player.plane_y * game->ray.camera_x);
	game->ray.map_x = (int)game->player.pos_x;
	game->ray.map_y = (int)game->player.pos_y;
	game->ray.delta_dist_x = fabs(1.0 / game->ray.dir_x);
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
		if (game->ray.map_x >= 0 && game->ray.map_x < game->map.width && game->ray.map_y >= 0 && game->ray.map_y < game->map.height)
		{
			if (game->map.grid[game->ray.map_y][game->ray.map_x] == '1')
				game->ray.hit = 1;
		}
	}
	if (side == 0)
		game->ray.perp_wall_dist = (game->ray.side_dist_x - game->ray.delta_dist_x);
	else
		game->ray.perp_wall_dist = (game->ray.side_dist_y - game->ray.delta_dist_y);
	
	if (game->ray.hit)
	{
		int lineHeight = (int)(game->mlx.win_height / game->ray.perp_wall_dist);
		int drawStart = -lineHeight / 2 + game->mlx.win_height / 2;
		if(drawStart < 0)
			drawStart = 0;
		int drawEnd = lineHeight / 2 + game->mlx.win_height / 2;
		if(drawEnd >= game->mlx.win_height)
			drawEnd = game->mlx.win_height - 1;
		while (drawStart <= drawEnd)
			put_pixel(game, screen_x, drawStart++, 0x0000FF);
	}
	if (game->ray.hit && BONUS)
	{
		/*
		double intersection_x = game->player.pos_x + game->ray.dir_x * game->ray.perp_wall_dist;
		double intersection_y = game->player.pos_y + game->ray.dir_y * game->ray.perp_wall_dist;
		draw_circle(game, intersection_x * 8, intersection_y * 8, 1, 0xFF0000);
		*/
	}
}
