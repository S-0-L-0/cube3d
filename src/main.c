/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:00:54 by edforte           #+#    #+#             */
/*   Updated: 2025/09/16 17:32:51 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	validate_and_initialize_mlx_struct(t_mlx *mlx)
{
	if (!mlx)
		return (1);
	memset(mlx, 0, sizeof(t_mlx));
	mlx->win_width = 1280;
	mlx->win_height = 960;
	return (0);
}

int	create_mlx_connection(t_mlx *mlx)
{
	mlx->mlx = mlx_init();
	if (!mlx->mlx)
		return (1);
	return (0);
}

int	create_window(t_mlx *mlx)
{
	mlx->win = mlx_new_window(mlx->mlx, mlx->win_width, mlx->win_height,
			"cub3D");
	if (!mlx->win)
	{
		free(mlx->mlx);
		mlx->mlx = NULL;
		return (1);
	}
	return (0);
}

int	create_image(t_mlx *mlx)
{
	mlx->img = mlx_new_image(mlx->mlx, mlx->win_width, mlx->win_height);
	if (!mlx->img)
	{
		mlx_destroy_window(mlx->mlx, mlx->win);
		free(mlx->mlx);
		mlx->mlx = NULL;
		mlx->win = NULL;
		return (1);
	}
	return (0);
}

int	setup_image_data(t_mlx *mlx)
{
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

int	init_mlx(t_mlx *mlx)
{
	if (validate_and_initialize_mlx_struct(mlx) != 0)
		return (1);
	if (create_mlx_connection(mlx) != 0)
		return (1);
	if (create_window(mlx) != 0)
		return (1);
	if (create_image(mlx) != 0)
		return (1);
	return (setup_image_data(mlx));
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

char	*get_texture_path(t_game *game, int index)
{
	if (index == 0)
		return (game->map.north_texture);
	else if (index == 1)
		return (game->map.south_texture);
	else if (index == 2)
		return (game->map.west_texture);
	else if (index == 3)
		return (game->map.east_texture);
	else if (index == 4)
		return ("assets/textures/cross.xpm");
	else if (index == 5)
		return ("assets/textures/torch20_1.xpm");
	else if (index == 6)
		return ("assets/textures/torch10_2.xpm");
	else if (index == 7)
		return ("assets/textures/torch40_3.xpm");
	else if (index == 8)
		return ("assets/textures/torch60_4.xpm");
	else if (index == 9)
		return ("assets/textures/torch70_5.xpm");
	return (NULL);
}

char	*get_error_texture_name(int index)
{
	if (index == 0)
		return ("north texture");
	else if (index == 1)
		return ("south texture");
	else if (index == 2)
		return ("west texture");
	else if (index == 3)
		return ("east texture");
	else if (index == 4)
		return ("cross");
	return ("torch");
}

void	cleanup_loaded_textures(t_game *game, int loaded_count)
{
	int	i;

	i = 0;
	while (i < loaded_count)
	{
		free_texture_struct(&game->textures[i], game->mlx.mlx);
		i++;
	}
}

int	load_single_texture(t_game *game, int index)
{
	char	*path;
	char	*error_name;

	path = get_texture_path(game, index);
	if (init_texture(&game->textures[index], path, game->mlx.mlx) != 0)
	{
		error_name = get_error_texture_name(index);
		printf("Error\nFailed to load %s: %s\n", error_name, path);
		cleanup_loaded_textures(game, index);
		free_map(&game->map);
		free_mlx(&game->mlx);
		return (1);
	}
	return (0);
}

int	set_textures(t_game *game)
{
	int	i;

	i = 0;
	while (i < 10)
	{
		if (load_single_texture(game, i) != 0)
			return (1);
		i++;
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
