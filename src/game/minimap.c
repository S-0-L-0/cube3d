#include "../../includes/cub3d.h"

#define TILE_SIZE 8
#define VIEW_RADIUS 5
#define MINIMAP_DIM (VIEW_RADIUS * 2 + 1)

void	draw_minimap(t_game *game)
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
