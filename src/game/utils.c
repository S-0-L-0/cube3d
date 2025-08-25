#include "../../includes/cub3d.h"

long long	get_time_ms()
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

unsigned int	get_pixel_color(t_texture *texture, int x, int y)
{
	char	*pixel;
	int		bytes_per_pixel;
	int		offset;

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

void	put_pixel(t_game *game, int x, int y, int color)
{
	char *dst;
	if (x >= game->mlx.win_width || y >= game->mlx.win_height || x < 0 || y < 0)
		return ;
	dst = game->mlx.addr + (y * game->mlx.line_length + x * (game->mlx.bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	draw_circle(t_game *game, int cx, int cy, int radius, int color)
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
