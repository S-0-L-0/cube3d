/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:57 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 16:20:45 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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
	int	i;

	i = 0;
	while (i <= 10)
	{
		if (load_single_texture(game, i) != 0)
			return (1);
		i++;
	}
	return (0);
}

void	free_texture_struct(t_texture *texture, void *mlx)
{
	if (!texture || !mlx)
		return ;
	if (texture->img)
	{
		mlx_destroy_image(mlx, texture->img);
		texture->img = NULL;
	}
	ft_memset(texture, 0, sizeof(t_texture));
}

void	free_game_texture(t_game *game)
{
	if (game->map.north_texture)
	{
		free(game->map.north_texture);
		game->map.north_texture = NULL;
	}
	if (game->map.south_texture)
	{
		free(game->map.south_texture);
		game->map.south_texture = NULL;
	}
	if (game->map.east_texture)
	{
		free(game->map.east_texture);
		game->map.east_texture = NULL;
	}
	if (game->map.west_texture)
	{
		free(game->map.west_texture);
		game->map.west_texture = NULL;
	}
}
