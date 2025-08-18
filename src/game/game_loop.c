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
	
	// Disegna una croce al centro per test
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
	
	// Mostra l'immagine
	mlx_put_image_to_window(game->mlx.mlx, game->mlx.win, game->mlx.img, 0, 0);
}

int key_press(int keycode, t_game *game)
{
	
	if (keycode == 65307) // ESC su macOS (era 65307 che è Linux)
	{
		printf("ESC pressed, closing game...\n");
		free_game(game);
		exit(0);
	}
	// Altri tasti per test
	else if (keycode == 13) // W
	{
		printf("W pressed\n");
	}
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
	
	return (0);
}

int close_window(t_game *game)
{
	printf("Window close button pressed\n");
	free_game(game);
	exit(0);
	return (0);
}

// Funzione di rendering principale
int render_frame(t_game *game)
{
	render_test_screen(game);
	return (0);
}

int game_loop(t_game *game)
{
	printf("Starting game loop...\n");
	printf("Window size: %dx%d\n", game->mlx.win_width, game->mlx.win_height);
	printf("Player position: (%.2f, %.2f)\n", game->player.pos_x, game->player.pos_y);
	printf("Map size: %dx%d\n", game->map.width, game->map.height);
	
	// Renderizza il primo frame
	render_test_screen(game);
	
	// Imposta event handlers
	mlx_hook(game->mlx.win, 2, 1L << 0, key_press, game);          // Key press
	mlx_hook(game->mlx.win, 17, 1L << 17, close_window, game);     // Window close
	mlx_loop_hook(game->mlx.mlx, render_frame, game);              // Rendering continuo
	
	printf("Press ESC to quit, W/A/S/D to test movement\n");
	
	// Avvia loop
	mlx_loop(game->mlx.mlx);
	
	return (0);
}
