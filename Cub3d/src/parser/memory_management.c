/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:36 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 00:56:11 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	**allocate_2d_int_array(int height, int width)
{
	int	**array;
	int	i;

	if (validate_dimensions(height, width) != 0)
		return (NULL);
	array = allocate_rows_array(height);
	if (!array)
		return (NULL);
	i = 0;
	while (i < height)
	{
		array[i] = allocate_and_initialize_row(width, array, i);
		if (!array[i])
			return (NULL);
		i++;
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
