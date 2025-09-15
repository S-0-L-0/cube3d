#include "../../includes/cub3d.h"

void	raycasting(t_game *game, int screen_x)
{
	t_texture		*texture;
	int				side;
	int				y;
	double			intersection_x;
	double			intersection_y;
	int				lineHeight;
	int				drawStart;
	int				drawEnd;
	int				texture_index;
	double			wallX;
	double			step;
	double			texPos;
	int				texX;
	int				texY;
	unsigned int	color;

	side = 0;
	game->ray.hit = 0;
	game->ray.camera_x = ((2 * screen_x) / (double)game->mlx.win_width) - 1;
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
	
	if (game->ray.hit && DEBUG)
	{
		intersection_x = game->player.pos_x + game->ray.dir_x * game->ray.perp_wall_dist;
		intersection_y = game->player.pos_y + game->ray.dir_y * game->ray.perp_wall_dist;
		if (side)
			draw_circle(game, intersection_x * 32, intersection_y * 32, 1, 0xFF0000);
		else
			draw_circle(game, intersection_x * 32, intersection_y * 32, 1, 0xFF00FF);
	}
	else if (game->ray.hit)
	{
		lineHeight = (int)(game->mlx.win_height / game->ray.perp_wall_dist);
		drawStart = -lineHeight / 2 + game->mlx.win_height / 2;
		if(drawStart < 0)
			drawStart = 0;
		drawEnd = lineHeight / 2 + game->mlx.win_height / 2;
		if(drawEnd >= game->mlx.win_height)
			drawEnd = game->mlx.win_height - 1;
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
		texture = &game->textures[texture_index];
		if (side == 0)
			wallX = game->player.pos_y + (game->ray.perp_wall_dist * game->ray.dir_y);
		else
			wallX = game->player.pos_x + (game->ray.perp_wall_dist * game->ray.dir_x);
		wallX -= floor(wallX);
		texX = (int)(wallX * (double)texture->width);
		if (side == 0 && game->ray.dir_x < 0)
			texX = texture->width - texX - 1;
		if (side == 1 && game->ray.dir_y > 0)
			texX = texture->width - texX - 1;
		step = (1.0 * texture->height) / lineHeight;
		texPos = (drawStart - (game->mlx.win_height / 2) + (lineHeight / 2)) * step;
		while(y < drawEnd)
		{
			texY = (int)texPos & (texture->height - 1);
			texPos += step;
			color = get_pixel_color(texture, texX, texY);
			put_pixel(game, screen_x, y, color);
			y ++;
		}
	}
}
