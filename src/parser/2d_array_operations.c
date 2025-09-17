/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2d_array_operations.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:23:43 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 00:54:41 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	validate_dimensions(int height, int width)
{
	if (height <= 0 || width <= 0)
	{
		printf("Error\nInvalid dimensions for 2D array\n");
		return (1);
	}
	return (0);
}

int	**allocate_rows_array(int height)
{
	int	**array;

	array = (int **)malloc(sizeof(int *) * height);
	if (!array)
	{
		printf("Error\nMemory allocation failed for 2D array\n");
		return (NULL);
	}
	return (array);
}

void	cleanup_partial_array(int **array, int allocated_rows)
{
	while (allocated_rows > 0)
	{
		allocated_rows--;
		free(array[allocated_rows]);
	}
	free(array);
}

int	*allocate_and_initialize_row(int width, int **array, int row_index)
{
	int	*row;
	int	j;

	row = (int *)malloc(sizeof(int) * width);
	if (!row)
	{
		printf("Error\nMemory allocation failed for row %d\n", row_index);
		cleanup_partial_array(array, row_index);
		return (NULL);
	}
	j = 0;
	while (j < width)
	{
		row[j] = 0;
		j++;
	}
	return (row);
}
