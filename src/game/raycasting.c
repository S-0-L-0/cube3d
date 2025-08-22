#include "../../includes/cub3d.h"

void	raycasting(t_game *game, int screen_x)
{
	int	side;

	side = 0;
	game->ray.hit = 0;
	game->ray.camera_x = ((2 * screen_x) / (double)game->mlx.win_width) - 1; // [-1, 0, 1]
	// ray direction
	game->ray.dir_x = game->player.dir_x + (game->player.plane_x * game->ray.camera_x);
	game->ray.dir_y = game->player.dir_y + (game->player.plane_y * game->ray.camera_x);
	// current box of the map
	game->ray.map_x = (int)game->player.pos_x;
	game->ray.map_y = (int)game->player.pos_y;
	// costant increment for side_dist
	game->ray.delta_dist_x = fabs(1.0 / game->ray.dir_x);
	game->ray.delta_dist_y = fabs(1.0 / game->ray.dir_y);

	if (game->ray.dir_x < 0)
	{
		game->ray.step_x = -1;
		// (distance from the ray starting position to the first side to the left) * delta_dist_x = Euclidean distance
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

	// DDA
	// Increments the ray by 1 square until hit
	while (!game->ray.hit)
	{
		// jump to nex square x or else y
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
			// check for an hit
			if (game->map.grid[game->ray.map_y][game->ray.map_x] == '1')
				game->ray.hit = 1;
		}
	}
	if (side == 0)
		// calculate distace = side_dist, but we need to go a step back because with dda it's touching the wall.
		game->ray.perp_wall_dist = (game->ray.side_dist_x - game->ray.delta_dist_x);
	else
		game->ray.perp_wall_dist = (game->ray.side_dist_y - game->ray.delta_dist_y);
	
	if (game->ray.hit && DEBUG)
	{
		double intersection_x = game->player.pos_x + game->ray.dir_x * game->ray.perp_wall_dist;
		double intersection_y = game->player.pos_y + game->ray.dir_y * game->ray.perp_wall_dist;
		if (side)
			draw_circle(game, intersection_x * 32, intersection_y * 32, 1, 0xFF0000);
		else
			draw_circle(game, intersection_x * 32, intersection_y * 32, 1, 0xFF00FF);
	}
	else if (game->ray.hit)
	{
		int lineHeight = (int)(game->mlx.win_height / game->ray.perp_wall_dist);
		int drawStart = -lineHeight / 2 + game->mlx.win_height / 2;
		if(drawStart < 0)
			drawStart = 0;
		int drawEnd = lineHeight / 2 + game->mlx.win_height / 2;
		if(drawEnd >= game->mlx.win_height)
			drawEnd = game->mlx.win_height - 1;

		int texture_index;
		if (side == 0)
		{
			if (game->ray.dir_x > 0)
				texture_index = 3;
			else
				texture_index = 2;
		}
		else
		{
			if (game->ray.dir_y > 0)
				texture_index = 1;
			else
				texture_index = 0;
		}
		t_texture *texture = &game->textures[texture_index];

		// exact position of the hit
		double wallX;
		if (side == 0)
			wallX = game->player.pos_y + (game->ray.perp_wall_dist * game->ray.dir_y);
		else
			wallX = game->player.pos_x + (game->ray.perp_wall_dist * game->ray.dir_x);
		wallX -= floor(wallX);

		// exact pixel of the texture
		int texX = (int)(wallX * (double)texture->width);
		if (side == 0 && game->ray.dir_x < 0)
			texX = texture->width - texX - 1;
		if (side == 1 && game->ray.dir_y > 0)
			texX = texture->width - texX - 1;
		
		// How much to increase the texture coordinate per screen pixel
		double step = (1.0 * texture->height) / lineHeight;
		// Starting texture coordinate
		double texPos = (drawStart - (game->mlx.win_height / 2) + (lineHeight / 2)) * step;

		// draw a line in the y dir

		for(int y = drawStart; y<drawEnd; y++)
		{
			// Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
			int texY = (int)texPos & (texture->height - 1);
			texPos += step;
			unsigned int color = get_pixel_color(texture, texX, texY);
			//if (side == 1) color = (color >> 1) & 8355711;
			put_pixel(game, screen_x, y, color);
		}
		/*
		while (drawStart <= drawEnd)
		{
			put_pixel(game, screen_x, drawStart, 0x0000FF);
			drawStart++;
		}
		*/
	}
}
