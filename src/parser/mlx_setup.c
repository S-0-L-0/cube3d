/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:42 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 00:27:28 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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

void	free_mlx(t_mlx *mlx)
{
	if (!mlx)
		return ;
	if (mlx->img)
	{
		mlx_destroy_image(mlx->mlx, mlx->img);
		mlx->img = NULL;
	}
	if (mlx->win)
	{
		mlx_destroy_window(mlx->mlx, mlx->win);
		mlx->win = NULL;
	}
	if (mlx->mlx)
	{
		free(mlx->mlx);
		mlx->mlx = NULL;
	}
	ft_memset(mlx, 0, sizeof(t_mlx));
}
