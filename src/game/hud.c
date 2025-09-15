#include "../../includes/cub3d.h"

static void	draw_texture(t_game *game, t_texture *tex, int start_x, int start_y)
{
	int				tex_x;
	int				tex_y;
	unsigned int	color;

	tex_y = 0;
	while (tex_y < tex->height)
	{
		tex_x = 0;
		while (tex_x < tex->width)
		{
			color = get_pixel_color(tex, tex_x, tex_y);
			if (color != 0xFF000000)
				put_pixel(game, start_x + tex_x, start_y + tex_y, color);
			tex_x++;
		}
		tex_y++;
	}
}

void	draw_crosshair(t_game *game)
{
	int	start_x;
	int	start_y;

	start_x = (game->mlx.win_width / 2) - (game->textures[4].width / 2);
	start_y = (game->mlx.win_height / 2) - (game->textures[4].height / 2);
	draw_texture(game, &game->textures[4], start_x, start_y);
}

static int	get_torch_frame_index(t_game *game)
{
	long	elapsed_time;

	elapsed_time = game->time.time - game->time.start_time;
	if (elapsed_time <= 200)
		return (5);
	if (elapsed_time <= 400)
		return (6);
	if (elapsed_time <= 600)
		return (7);
	if (elapsed_time <= 800)
		return (8);
	if (elapsed_time <= 1000)
		return (9);
	game->time.start_time = get_time_ms();
	return (5);
}

void	draw_torch(t_game *game)
{
	int	num;
	int	start_x;
	int	start_y;

	num = get_torch_frame_index(game);
	start_x = (game->mlx.win_width / 2) - (game->textures[num].width / 2)
		+ (game->mlx.win_width / 3) - (game->textures[num].width / 3);
	start_y = (game->mlx.win_height / 2) - (game->textures[num].height / 2)
		+ (game->mlx.win_height / 1.5) - (game->textures[num].height / 1.5);
	draw_texture(game, &game->textures[num], start_x, start_y);
}
