/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:11:51 by edforte           #+#    #+#             */
/*   Updated: 2025/09/16 11:51:42 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	**allocate_2d_int_array(int height, int width)
{
	int	**array;
	int	i;
	int	j;

	if (height <= 0 || width <= 0)
	{
		printf("Error\nInvalid dimensions for 2D array\n");
		return (NULL);
	}
	array = (int **)malloc(sizeof(int *) * height);
	if (!array)
	{
		printf("Error\nMemory allocation failed for 2D array\n");
		return (NULL);
	}
	i = 0;
	while (i < height)
	{
		array[i] = (int *)malloc(sizeof(int) * width);
		if (!array[i])
		{
			printf("Error\nMemory allocation failed for row %d\n", i);
			while (i > 0)
			{
				i--;
				free(array[i]);
			}
			free(array);
			return (NULL);
		}
		j = 0;
		while (j < width)
		{
			array[i][j] = 0;
			j++;
		}
		i ++;
	}
	return (array);
}

void	free_2d_int_array(int **array, int height)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (i < height)
	{
		if (array[i])
		{
			free(array[i]);
			array[i] = NULL;
		}
		i++;
	}
	free(array);
}

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		split[i] = NULL;
		i++;
	}
	free(split);
}
