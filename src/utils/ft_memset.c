/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:33:56 by edforte           #+#    #+#             */
/*   Updated: 2025/09/05 16:27:13 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;

	ptr = (unsigned char *) b;
	while (len-- > 0)
	{
		*ptr = (unsigned char) c;
		ptr ++;
	}
	return (b);
}

char	*ft_strdup(const char *s1)
{
	char	*s2;
	size_t	i;

	s2 = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!s2)
		return (0);
	i = 0;
	while (s1[i])
	{
		s2[i] = s1[i];
		i ++;
	}
	s2[i] = '\0';
	return (s2);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*a;
	unsigned char	*b;

	i = 0;
	a = (unsigned char *)s1;
	b = (unsigned char *)s2;
	while ((a[i] || b[i]) && i < n)
	{
		if (a[i] != b[i])
			return (a[i] - b[i]);
		i ++;
	}
	return (0);
}

int		ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i ++;
	return (i);
}

void free_game(t_game *game)
{
	int i;
	
	if (!game)
		return;
	
	// Libera le texture
	i = 0;
	while (i < 10)
	{
		free_texture(&game->textures[i], game->mlx.mlx);
		i++;
	}
	
	// Libera la mappa
	free_map(&game->map);
	
	// Libera MLX
	free_mlx(&game->mlx);
	
	// Azzera il player
	ft_memset(&game->player, 0, sizeof(t_player));
}

// Funzione per liberare le texture
void free_texture(t_texture *texture, void *mlx)
{
	if (!texture || !mlx)
		return;
		
	if (texture->img)
	{
		mlx_destroy_image(mlx, texture->img);
		texture->img = NULL;
	}
	
	ft_memset(texture, 0, sizeof(t_texture));
}

// Funzione per liberare MLX

void	free_mlx(t_mlx *mlx)
{
	if (!mlx)
		return;
		
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

void free_map(t_map *map)
{
	if (!map)
		return;
		
	free_map_grid(map);
	
	if (map->north_texture)
	{
		free(map->north_texture);
		map->north_texture = NULL;
	}
	if (map->south_texture)
	{
		free(map->south_texture);
		map->south_texture = NULL;
	}
	if (map->west_texture)
	{
		free(map->west_texture);
		map->west_texture = NULL;
	}
	if (map->east_texture)
	{
		free(map->east_texture);
		map->east_texture = NULL;
	}
}
