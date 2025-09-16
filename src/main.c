/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:00:54 by edforte           #+#    #+#             */
/*   Updated: 2025/09/16 11:29:02 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	init_mlx(t_mlx *mlx)
{
	if (!mlx)
		return (1);
	memset(mlx, 0, sizeof(t_mlx));
	mlx->win_width = 1280;
	mlx->win_height = 960;
	mlx->mlx = mlx_init();
	if (!mlx->mlx)
		return (1);
	mlx->win = mlx_new_window(mlx->mlx, mlx->win_width, mlx->win_height,
			"cub3D");
	if (!mlx->win)
	{
		free(mlx->mlx);
		mlx->mlx = NULL;
		return (1);
	}
	mlx->img = mlx_new_image(mlx->mlx, mlx->win_width, mlx->win_height);
	if (!mlx->img)
	{
		mlx_destroy_window(mlx->mlx, mlx->win);
		free(mlx->mlx);
		mlx->mlx = NULL;
		mlx->win = NULL;
		return (1);
	}
	mlx->addr = mlx_get_data_addr(mlx->img, &mlx->bits_per_pixel,
			&mlx->line_length, &mlx->endian);
	if (!mlx->addr)
	{
		mlx_destroy_image(mlx->mlx, mlx->img);
		mlx_destroy_window(mlx->mlx, mlx->win);
		free(mlx->mlx);
		memset(mlx, 0, sizeof(t_mlx));
		return (1);
	}
	return (0);
}

int	init_texture(t_texture *texture, char *texture_path, void *mlx)
{
	if (!texture || !texture_path || !mlx)
		return (1);
	memset(texture, 0, sizeof(t_texture));
	texture->img = mlx_xpm_file_to_image(mlx, texture_path, &texture->width,
			&texture->height);
	if (!texture->img)
		return (1);
	texture->addr = mlx_get_data_addr(texture->img, &texture->bits_per_pixel,
			&texture->line_length, &texture->endian);
	if (!texture->addr)
	{
		mlx_destroy_image(mlx, texture->img);
		texture->img = NULL;
		return (1);
	}
	return (0);
}

int	set_textures(t_game *game)
{
	if (init_texture(&game->textures[0], game->map.north_texture,
			game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load north texture: %s\n",
			game->map.north_texture);
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[1], game->map.south_texture,
			game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load south texture: %s\n",
			game->map.south_texture);
		free_texture_struct(&game->textures[0], game->mlx.mlx);
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[2], game->map.west_texture,
			game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load west texture: %s\n",
			game->map.west_texture);
		free_texture_struct(&game->textures[0], game->mlx.mlx);
		free_texture_struct(&game->textures[1], game->mlx.mlx);
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[3], game->map.east_texture,
			game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load east texture: %s\n",
			game->map.east_texture);
		free_texture_struct(&game->textures[0], game->mlx.mlx);
		free_texture_struct(&game->textures[1], game->mlx.mlx);
		free_texture_struct(&game->textures[2], game->mlx.mlx);
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[4], "assets/textures/cross.xpm",
			game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load cross: %s\n",
			"assets/textures/cross.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[5], "assets/textures/torch20_1.xpm",
			game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load torch: %s\n",
			"assets/textures/torch1.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[6], "assets/textures/torch10_2.xpm",
			game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load torch: %s\n",
			"assets/textures/torch1.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[7], "assets/textures/torch40_3.xpm",
			game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load torch: %s\n",
			"assets/textures/torch1.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[8], "assets/textures/torch60_4.xpm",
			game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load torch: %s\n",
			"assets/textures/torch1.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	if (init_texture(&game->textures[9], "assets/textures/torch70_5.xpm",
			game->mlx.mlx) != 0)
	{
		printf("Error\nFailed to load torch: %s\n",
			"assets/textures/torch1.xpm");
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_game			game;
	t_parse_data	parse_data;

	ft_memset(&game, 0, sizeof(t_game));
	ft_memset(&parse_data, 0, sizeof(t_parse_data));
	if (parser(argc, argv, &game, &parse_data) != 0)
	{
		cleanup_parse_data(&parse_data);
		return (cleanup_game(&game), 1);
	}
	cleanup_parse_data(&parse_data);
	if (init_mlx(&game.mlx) != 0)
	{
		printf("Error\nFailed to initialize graphics\n");
		return (cleanup_game(&game), 1);
	}
	if (set_textures(&game) != 0)
	{
		free_mlx(&game.mlx);
		return (cleanup_game(&game), 1);
	}
	game_loop(&game);
	free_mlx(&game.mlx);
	cleanup_game(&game);
	return (0);
}
