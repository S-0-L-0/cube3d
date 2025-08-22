#include "../../includes/cub3d.h"

void draw_crosshair(t_game *game)
{
	int         screen_x;
    int         screen_y;
    int tex_y = 0;
	unsigned int color;
	int start_x = (game->mlx.win_width / 2) - (game->textures[4].width / 2);
    int start_y = (game->mlx.win_height / 2) - (game->textures[4].height / 2);
    while (tex_y < game->textures[4].height)
    {
		int tex_x = 0;
        while (tex_x < game->textures[4].width)
        {
			color = get_pixel_color(&game->textures[4], tex_x, tex_y);
			screen_x = start_x + tex_x;
			screen_y = start_y + tex_y;
			if (color != (unsigned int)-16777216)
				put_pixel(game, screen_x, screen_y, color);
			tex_x++;
        }
		tex_y++;
    }

}

void	draw_torch(t_game *game)
{
	int         screen_x;
	int         screen_y;
	int			num;
    int tex_y = 0;
	unsigned int color;

	if (game->time.time - game->time.start_time <= 200)
		num = 5;
	else if (game->time.time - game->time.start_time <= 400)
		num = 6;
	else if (game->time.time - game->time.start_time <= 600)
		num = 7;
	else if (game->time.time - game->time.start_time <= 800)
		num = 8;
	else if (game->time.time - game->time.start_time <= 1000)
		num = 9;
	else
	{
		num = 5;
		game->time.start_time = get_time_ms();
	}
	int start_x = (game->mlx.win_width / 2) - (game->textures[num].width / 2) + (game->mlx.win_width / 3) - (game->textures[num].width / 3);
	int start_y = (game->mlx.win_height / 2) - (game->textures[num].height / 2) + (game->mlx.win_height / 1.5) - (game->textures[num].height / 1.5);
	while (tex_y < game->textures[num].height)
	{
		int tex_x = 0;
		while (tex_x < game->textures[num].width)
		{
			color = get_pixel_color(&game->textures[num], tex_x, tex_y);
			screen_x = start_x + tex_x;
			screen_y = start_y + tex_y;
			if (color != (unsigned int)-16777216)
				put_pixel(game, screen_x, screen_y, color);
			tex_x++;
		}
		tex_y++;
	}

}
