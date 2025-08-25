/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:27:47 by edforte           #+#    #+#             */
/*   Updated: 2025/08/18 20:03:23 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	init_player(t_game *game)
{
	game->player.pos_x = 5.0;
	game->player.pos_y = 5.0;
	game->player.dir_x = -1.0; // left
	game->player.dir_y = 0.0; // center
	game->player.plane_x = 0.0;
	game->player.plane_y = 0.66; // FOV
}

/*
 * STEP 2: ORCHESTRATORE PARSING + SETUP GRAFICO
 * 
 * FLUSSO DATI:
 * INPUT: map_path (path al file .cub), game (struttura vuota)
 * 
 * 1. Chiama parse_complete_file() -> RIEMPIE game->map e game->player
 * 2. Chiama init_mlx() -> RIEMPIE game->mlx (connessione grafica)
 * 3. Carica 4 texture -> RIEMPIE game->textures[0-3]
 * 
 * OUTPUT: game completamente popolata e pronta per il rendering
 * 
 * GESTIONE ERRORI: Se qualsiasi step fallisce, libera tutto e restituisce errore
 */
int parse_map(char *map_path, t_game *game)
{
	// Parsa file e popola map + player
	if (parse_complete_file(map_path, &game->map, &game->player) != 0)
	{
		return (1);
	}
	
	// Inizializza MLX
	if (init_mlx(&game->mlx) != 0)
	{
		printf("Error\nFailed to initialize graphics\n");
		free_map(&game->map);
		return (1);
	}
	
	// Inizializza texture
	if (init_texture(&game->textures[0], game->map.north_texture, game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load north texture: %s\n", game->map.north_texture);
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[1], game->map.south_texture, game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load south texture: %s\n", game->map.south_texture);
		free_texture(&game->textures[0], game->mlx.mlx);
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[2], game->map.west_texture, game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load west texture: %s\n", game->map.west_texture);
		free_texture(&game->textures[0], game->mlx.mlx);
		free_texture(&game->textures[1], game->mlx.mlx);
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[3], game->map.east_texture, game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load east texture: %s\n", game->map.east_texture);
		free_texture(&game->textures[0], game->mlx.mlx);
		free_texture(&game->textures[1], game->mlx.mlx);
		free_texture(&game->textures[2], game->mlx.mlx);
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	// added crosshair texture
	if (init_texture(&game->textures[4], "assets/textures/cross.xpm", game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load cross: %s\n", "assets/textures/cross.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[5], "assets/textures/torch20_1.xpm", game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load torch: %s\n", "assets/textures/torch1.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[6], "assets/textures/torch10_2.xpm", game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load torch: %s\n", "assets/textures/torch1.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[7], "assets/textures/torch40_3.xpm", game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load torch: %s\n", "assets/textures/torch1.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[8], "assets/textures/torch60_4.xpm", game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load torch: %s\n", "assets/textures/torch1.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[9], "assets/textures/torch70_5.xpm", game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load torch: %s\n", "assets/textures/torch1.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}

	// TODO: init player con dati dalla mappa
	return (0);
}
