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

void    draw_minimap(t_game *game)
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

void	draw_map_2d(t_game *game)
{
	int	map_x; 
	int	map_y;
	int	screen_x;
	int	screen_y;

	map_y = 0;
	while (map_y < (game->map.height))
	{
		map_x = 0;
		while (map_x < (game->map.width))
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

unsigned int get_pixel_color(t_texture *texture, int x, int y)
{
	char    *pixel;
	int     bytes_per_pixel;
	int     offset;

	if (x < 0 || x >= texture->width || y < 0 || y >= texture->height)
		return (0);
	// bits to bytes
	bytes_per_pixel = texture->bits_per_pixel / 8;
	// Calcola offset totale usando la formula chiara
	offset = (y * texture->line_length) + (x * bytes_per_pixel);

	// Punta al pixel corretto
	pixel = texture->addr + offset;
	return (*(unsigned int *)pixel);
}
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
