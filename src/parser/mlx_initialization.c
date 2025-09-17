/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_initialization.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:23:20 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 13:53:31 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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
