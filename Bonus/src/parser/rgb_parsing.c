/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 00:24:50 by edforte           #+#    #+#             */
/*   Updated: 2025/09/17 14:01:15 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	parse_single_rgb_value(char *str, int *pos)
{
	int	value;
	int	has_digit;

	value = 0;
	has_digit = 0;
	while (str[*pos] && (str[*pos] == ' ' || str[*pos] == '\t'))
		(*pos)++;
	while (str[*pos] && str[*pos] >= '0' && str[*pos] <= '9')
	{
		value = value * 10 + (str[*pos] - '0');
		has_digit = 1;
		if (value > 255)
		{
			return (printf("Error\nRGB value exceeds 255\n"), -1);
		}
		(*pos)++;
	}
	while (str[*pos] && (str[*pos] == ' ' || str[*pos] == '\t'))
		(*pos)++;
	if (!has_digit)
	{
		printf("Error\nWrong RGB component\n");
		return (-1);
	}
	return (value);
}

int	expect_comma(char *str, int *pos)
{
	if (str[*pos] != ',')
	{
		printf("Error\nExpected comma in RGB format\n");
		return (1);
	}
	(*pos)++;
	return (0);
}

int	extract_rgb_values(char *str, int *rgb)
{
	int	pos;
	int	value;
	int	i;

	pos = 0;
	if (!str || !rgb)
		return (1);
	i = -1;
	while (++i < 3)
	{
		value = parse_single_rgb_value(str, &pos);
		if (value < 0)
			return (1);
		rgb[i] = value;
		if (i < 2)
		{
			if (expect_comma(str, &pos) != 0)
				return (1);
		}
	}
	while (str[pos] && (str[pos] == ' ' || str[pos] == '\t'))
		pos++;
	if (str[pos] != '\0')
		return (printf("Error\nExtra content after RGB: '%c'\n", str[pos]), 1);
	return (0);
}
