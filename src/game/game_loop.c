/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:33:46 by edforte           #+#    #+#             */
/*   Updated: 2025/06/01 12:15:54 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// Funzione temporanea per disegnare un semplice test invece di schermo nero
void render_test_screen(t_game *game)
{
	int x, y;
	int color;
	char *dst;
	
	// Pulisci lo schermo con il colore del soffitto
	for (y = 0; y < game->mlx.win_height; y++)
	{
		for (x = 0; x < game->mlx.win_width; x++)
		{
			// Calcola la posizione nel buffer dell'immagine
			dst = game->mlx.addr + (y * game->mlx.line_length + x * (game->mlx.bits_per_pixel / 8));
			
			// Colore del soffitto nella parte superiore, pavimento nella parte inferiore
			if (y < game->mlx.win_height / 2)
			{
				// Soffitto
				color = (game->map.ceiling_color[0] << 16) | (game->map.ceiling_color[1] << 8) | game->map.ceiling_color[2];
			}
			else
			{
				// Pavimento
				color = (game->map.floor_color[0] << 16) | (game->map.floor_color[1] << 8) | game->map.floor_color[2];
			}
			
			*(unsigned int*)dst = color;
		}
	}
	
	/* Disegna una croce al centro per test
	int center_x = game->mlx.win_width / 2;
	int center_y = game->mlx.win_height / 2;
	
	// Linea orizzontale
	for (x = center_x - 20; x <= center_x + 20; x++)
	{
		if (x >= 0 && x < game->mlx.win_width)
		{
			dst = game->mlx.addr + (center_y * game->mlx.line_length + x * (game->mlx.bits_per_pixel / 8));
			*(unsigned int*)dst = 0xFFFFFF; // Bianco
		}
	}
	
	// Linea verticale
	for (y = center_y - 20; y <= center_y + 20; y++)
	{
		if (y >= 0 && y < game->mlx.win_height)
		{
			dst = game->mlx.addr + (y * game->mlx.line_length + center_x * (game->mlx.bits_per_pixel / 8));
			*(unsigned int*)dst = 0xFFFFFF; // Bianco
		}
	}
		*/
	
	// Mostra l'immagine
	//mlx_put_image_to_window(game->mlx.mlx, game->mlx.win, game->mlx.img, 0, 0);
}

void	move_player(t_game *game, double dir_x, double dir_y)
{
	double	new_x;
	double	new_y;
	double	delta_x;
	double	delta_y;
	double	move_speed;

	move_speed = 0.01;
	delta_x = dir_x * move_speed;
	delta_y = dir_y * move_speed;
	new_x = game->player.pos_x + delta_x;
	new_y = game->player.pos_y + delta_y;
	if (game->map.grid[(int)new_y][(int)game->player.pos_x] == '0')
		game->player.pos_y = new_y;
	if (game->map.grid[(int)game->player.pos_y][(int)new_x] == '0')
		game->player.pos_x = new_x;
}

void    rot_player(t_game *game, double direction)
{
	double  rot_speed;
	double  old_dir_x;
	double  old_plane_x;

	rot_speed = 0.01 * direction;
	old_dir_x = game->player.dir_x;
	old_plane_x = game->player.plane_x;
	game->player.dir_x = game->player.dir_x * cos(rot_speed) - game->player.dir_y * sin(rot_speed);
	game->player.dir_y = old_dir_x * sin(rot_speed) + game->player.dir_y * cos(rot_speed);
	game->player.plane_x = game->player.plane_x * cos(rot_speed) - game->player.plane_y * sin(rot_speed);
	game->player.plane_y = old_plane_x * sin(rot_speed) + game->player.plane_y * cos(rot_speed);
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

int	mouse_hook(int x, int y, t_game *game)
{
	(void)y;

	if (x < game->mlx.win_width / 2 - 5)
	{
		rot_player(game, -1.0);
	}
	
	else if (x > game->mlx.win_width / 2 + 5)
	{
		rot_player(game, 1.0);
	}

	return(0);

}

void update_player(t_game *game)
{
    if (game->keys.w)
        move_player(game, game->player.dir_x, game->player.dir_y);
    if (game->keys.s)
        move_player(game, -game->player.dir_x, -game->player.dir_y);
    if (game->keys.a)
        move_player(game, -game->player.plane_x, -game->player.plane_y);
    if (game->keys.d)
        move_player(game, game->player.plane_x, game->player.plane_y);
    if (game->keys.left)
        rot_player(game, -1.0);
    if (game->keys.right)
        rot_player(game, 1.0);
}

int close_window(t_game *game)
{
	printf("Window close button pressed\n");
	free_game(game);
	exit(0);
	return (0);
}

void	drawcast(t_game *game)
{
	int	x;

	x = 0;
	if (DEBUG)
	{
		draw_map_2d(game);
		draw_player_2d(game);
	}
	while (x < game->mlx.win_width)
	{
		raycasting(game, x);
		x++;
	}
	if (BONUS)
		draw_minimap(game);
}

// Funzione di rendering principale
int render_frame(t_game *game)
{
	mlx_mouse_move(game->mlx.mlx, game->mlx.win, game->mlx.win_width / 2, game->mlx.win_height / 2);
	update_player(game);
	render_test_screen(game);
	drawcast(game);
	mlx_put_image_to_window(game->mlx.mlx, game->mlx.win, game->mlx.img, 0, 0);
	return (0);
}

int game_loop(t_game *game)
{
	printf("Starting game loop...\n");
	printf("Window size: %dx%d\n", game->mlx.win_width, game->mlx.win_height);
	printf("Player position: (%.2f, %.2f)\n", game->player.pos_x, game->player.pos_y);
	printf("Map size: %dx%d\n", game->map.width, game->map.height);
	
	// Renderizza il primo frame
	//render_test_screen(game);
	
	// Imposta event handlers
	mlx_hook(game->mlx.win, 2, 1L << 0, key_press, game);          // Key press
	mlx_hook(game->mlx.win, 3, 1L << 1, key_release, game); // Key release
	mlx_hook(game->mlx.win, 17, 1L << 17, close_window, game);     // Window close
	mlx_hook(game->mlx.win, 6, 1L << 6, mouse_hook, game);
	//mlx_loop_hook(game->mlx.mlx, render_frame, game);            // Rendering continuo
	mlx_loop_hook(game->mlx.mlx, render_frame, game);                 // raycasting 2d

	printf("Press ESC to quit, W/A/S/D to test movement\n");
	
	// Avvia loop
	mlx_loop(game->mlx.mlx);
	
	return (0);
}
