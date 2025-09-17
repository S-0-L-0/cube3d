#include "../../includes/cub3d.h"

void	handle_move(int keycode, t_game *game)
{
	if (keycode == 119)
	{
		printf("W pressed\n");
		game->keys.w = true;
	}
	else if (keycode == 115)
	{
		printf("S pressed\n");
		game->keys.s = true;
	}
	else if (keycode == 97)
	{
		printf("A pressed\n");
		game->keys.a = true;
	}
	else if (keycode == 100)
	{
		printf("D pressed\n");
		game->keys.d = true;
	}
}

void	handle_arrows(int keycode, t_game *game)
{
	if (keycode == 65361)
	{
		printf("Left arrow pressed\n");
		game->keys.left = true;
	}
	else if (keycode == 65363)
	{
		printf("Right arrow pressed\n");
		game->keys.right = true;
	}
}

void	handle_actions(int keycode, t_game *game)
{
	if (keycode == 101)
	{
		mlx_string_put(game->mlx.mlx, game->mlx.win,
			100, 100, 0xFFFFFF, "Press E");
		toggle_door(game);
	}
}
