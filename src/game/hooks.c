#include "../../includes/cub3d.h"

int	mouse_hook(int x, int y, t_game *game)
{
	(void)y;

	if (x < game->mlx.win_width / 2)
	{
		rot_player(game, -1.0, 5.0);
	}
	
	else if (x > game->mlx.win_width / 2)
	{
		rot_player(game, 1.0, 5.0);
	}
	return(0);
}

int	key_press(int keycode, t_game *game)
{
	if (keycode == 65307) // ESC su macOS (era 65307 che è Linux)
	{
		printf("ESC pressed, closing game...\n");
		free_game(game);
		exit(0);
	}
	// Altri tasti per test
	if (keycode == 119) // W
	{
		printf("W pressed\n");
		game->keys.w = true;
		//move_player(game, game->player.dir_x, game->player.dir_y);
	}
	if (keycode == 115) // S
	{
		printf("S pressed\n");
		game->keys.s = true;
		//move_player(game, -game->player.dir_x, -game->player.dir_y);
	}
	if (keycode == 97) // A
	{
		printf("A pressed\n");
		game->keys.a = true;
		//move_player(game, -game->player.plane_x, -game->player.plane_y);
	}
	if (keycode == 100) // D
	{
		printf("D pressed\n");
		game->keys.d = true;
		//move_player(game, game->player.plane_x, game->player.plane_y);
	}
	else if (keycode == 65361) // Freccia sinistra
    {
        printf("Left arrow pressed\n");
		game->keys.left = true;
        //rot_player(game, -1.0);
    }
    else if (keycode == 65363) // Freccia destra
    {
        printf("Right arrow pressed\n");
		game->keys.right = true;
        //rot_player(game, 1.0);
    }

/*	APPLE
	else if (keycode == 13) // W
	{
		printf("W pressed\n");
	}mlx_mouse_hook(vars.win, mouse_hook, &vars);
	else if (keycode == 1) // S
	{
		printf("S pressed\n");
	}
	else if (keycode == 0) // A
	{
		printf("A pressed\n");
	}
	else if (keycode == 2) // D
	{
		printf("D pressed\n");
	}
*/	
	return (0);
}

int	key_release(int keycode, t_game *game)
{
	if (keycode == 119) // W
		game->keys.w = false;
	if (keycode == 115) // S
		game->keys.s = false;
	if (keycode == 97) // A
		game->keys.a = false;
	if (keycode == 100) // D
		game->keys.d = false;
	if (keycode == 65361) // Freccia sinistra
		game->keys.left = false;
	if (keycode == 65363) // Freccia destra
		game->keys.right = false;
	return (0);
}

int	close_window(t_game *game)
{
	printf("Window close button pressed\n");
	free_game(game);
	exit(0);
	return (0);
}
