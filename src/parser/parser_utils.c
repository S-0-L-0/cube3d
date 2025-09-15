/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:11:51 by edforte           #+#    #+#             */
/*   Updated: 2025/09/15 16:49:38 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

char	**ft_split_whitespace(char *str)
{
	char	**result;
	int		word_count;
	int		i;
	int		j;
	int		k;
	int		start;

	if (!str)
		return (NULL);
	word_count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == ' ' || str[i] == '\t'))
			i++;
		if (str[i])
		{
			word_count++;
			while (str[i] && str[i] != ' ' && str[i] != '\t')
				i++;
		}
	}
	result = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!result)
	{
		printf("Error\nMemory allocation failed in split_whitespace\n");
		return (NULL);
	}
	i = 0;
	j = 0;
	while (str[i] && j < word_count)
	{
		while (str[i] && (str[i] == ' ' || str[i] == '\t'))
			i++;
		if (str[i])
		{
			start = i;
			while (str[i] && str[i] != ' ' && str[i] != '\t')
				i++;
			result[j] = (char *)malloc(sizeof(char) * (i - start + 1));
			if (!result[j])
			{
				printf("Error\nMemory allocation failed for word\n");
				while (j > 0)
				{
					j--;
					free(result[j]);
				}
				free(result);
				return (NULL);
			}
			k = 0;
			while (start < i)
			{
				result[j][k] = str[start];
				k++;
				start++;
			}
			result[j][k] = '\0';
			j++;
		}
	}
	result[j] = NULL;
	return (result);
}

char	*trim_spaces(char *str)
{
	int		start;
	int		end;
	int		len;
	char	*trimmed;
	int		i;

	if (!str)
		return (NULL);
	start = 0;
	while (str[start] && (str[start] == ' ' || str[start] == '\t'))
		start++;
	if (!str[start])
	{
		trimmed = (char *)malloc(sizeof(char) * 1);
		if (!trimmed)
			return (NULL);
		trimmed[0] = '\0';
		return (trimmed);
	}
	len = ft_strlen(str);
	end = len - 1;
	while (end > start && (str[end] == ' ' || str[end] == '\t'))
		end--;
	len = end - start + 1;
	trimmed = (char *)malloc(sizeof(char) * (len + 1));
	if (!trimmed)
	{
		printf("Error\nMemory allocation failed in trim_spaces\n");
		return (NULL);
	}
	i = 0;
	while (i < len)
	{
		trimmed[i] = str[start + i];
		i++;
	}
	trimmed[i] = '\0';
	return (trimmed);
}

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
