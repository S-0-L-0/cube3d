/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_config.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:12:11 by edforte           #+#    #+#             */
/*   Updated: 2025/09/15 17:24:15 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	parse_config_elements(t_parse_data *parse, t_map *map)
{
	int	i;
	int	ret;

	i = 0;
	parse->map_start_line = -1;
	while (parse->file_content[i])
	{
		if (is_empty_line(parse->file_content[i]))
		{
			i++;
			continue ;
		}
		if (is_map_line(parse->file_content[i]))
		{
			printf("\n");
			parse->map_start_line = i;
			break ;
		}
		ret = parse_texture_line(parse->file_content[i], map, parse);
		if (ret == 0)
		{
			i++;
			continue ;
		}
		ret = parse_color_line(parse->file_content[i], map, parse);
		if (ret == 0)
		{
			i++;
			continue ;
		}
		printf("Error\nInvalid configuration line: %s\n",
			parse->file_content[i]);
		return (1);
	}
	if (parse->map_start_line == -1)
	{
		printf("Error\nNo map found in file\n");
		return (1);
	}
	return (0);
}

int	parse_texture_line(char *line, t_map *map, t_parse_data *parse)
{
	char	*identifier;
	char	*path_start;
	char	**dest_texture;
	int		*loaded_flag;
	int		len;

	if (!line || !map || !parse)
		return (1);
	while (*line == ' ' || *line == '\t')
		line++;
	if (ft_strncmp(line, "NO ", 3) == 0)
	{
		identifier = "NO";
		dest_texture = &map->north_texture;
		loaded_flag = &parse->north_loaded;
		path_start = line + 3;
	}
	else if (ft_strncmp(line, "SO ", 3) == 0)
	{
		identifier = "SO";
		dest_texture = &map->south_texture;
		loaded_flag = &parse->south_loaded;
		path_start = line + 3;
	}
	else if (ft_strncmp(line, "WE ", 3) == 0)
	{
		identifier = "WE";
		dest_texture = &map->west_texture;
		loaded_flag = &parse->west_loaded;
		path_start = line + 3;
	}
	else if (ft_strncmp(line, "EA ", 3) == 0)
	{
		identifier = "EA";
		dest_texture = &map->east_texture;
		loaded_flag = &parse->east_loaded;
		path_start = line + 3;
	}
	else
	{
		return (1);
	}
	if (*loaded_flag)
	{
		printf("Error\nDuplicate %s texture definition\n", identifier);
		return (1);
	}
	while (*path_start == ' ' || *path_start == '\t')
		path_start++;
	if (*path_start == '\0')
	{
		printf("Error\nEmpty path for %s texture\n", identifier);
		return (1);
	}
	len = ft_strlen(path_start);
	while (len > 0 && (path_start[len - 1] == ' '
			|| path_start[len - 1] == '\t'))
		len--;
	path_start[len] = '\0';
	*dest_texture = ft_strdup(path_start);
	if (!*dest_texture)
	{
		printf("Error\nMemory allocation failed for %s texture path\n",
			identifier);
		return (1);
	}
	*loaded_flag = 1;
	parse->texture_count ++;
	return (0);
}

int	parse_color_line(char *line, t_map *map, t_parse_data *parse)
{
	char	*color_values;
	int		*dest_color;
	int		*loaded_flag;
	char	*identifier;

	if (!line || !map || !parse)
		return (1);
	while (*line == ' ' || *line == '\t')
		line++;
	if (ft_strncmp(line, "F ", 2) == 0)
	{
		identifier = "Floor";
		dest_color = map->floor_col;
		loaded_flag = &parse->floor_set;
		color_values = line + 2;
	}
	else if (ft_strncmp(line, "C ", 2) == 0)
	{
		identifier = "Ceiling";
		dest_color = map->sky_col;
		loaded_flag = &parse->ceiling_set;
		color_values = line + 2;
	}
	else
		return (1);
	if (*loaded_flag)
	{
		printf("Error\nDuplicate %s color definition\n", identifier);
		return (1);
	}
	while (*color_values == ' ' || *color_values == '\t')
		color_values++;
	if (*color_values == '\0')
	{
		printf("Error\nEmpty RGB values for %s color\n", identifier);
		return (1);
	}
	if (extract_rgb_values(color_values, dest_color) != 0)
	{
		printf("Error\nInvalid RGB format for %s color\n", identifier);
		return (1);
	}
	*loaded_flag = 1;
	return (0);
}

int	validate_config_complete(t_map *map, t_parse_data *parse)
{
	int	error;

	error = 0;
	if (!map->north_texture)
	{
		printf("Error\nMissing North (NO) texture\n");
		error = 1;
	}
	else if (validate_texture_file(map->north_texture, "North") != 0)
		error = 1;
	if (!map->south_texture)
	{
		printf("Error\nMissing South (SO) texture\n");
		error = 1;
	}
	else if (validate_texture_file(map->south_texture, "South") != 0)
		error = 1;
	if (!map->west_texture)
	{
		printf("Error\nMissing West (WE) texture\n");
		error = 1;
	}
	else if (validate_texture_file(map->west_texture, "West") != 0)
		error = 1;
	if (!map->east_texture)
	{
		printf("Error\nMissing East (EA) texture\n");
		error = 1;
	}
	else if (validate_texture_file(map->east_texture, "East") != 0)
		error = 1;
	if (!parse->floor_set)
	{
		printf("Error\nMissing Floor (F) color\n");
		error = 1;
	}
	if (!parse->ceiling_set)
	{
		printf("Error\nMissing Ceiling (C) color\n");
		error = 1;
	}
	return (error);
}

static int	parse_single_rgb_value(char *str, int *pos)
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
		printf("Error\nEmpty RGB component\n");
		return (-1);
	}
	return (value);
}

static int	expect_comma(char *str, int *pos)
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

	pos = 0;
	if (!str || !rgb)
		return (1);
	value = parse_single_rgb_value(str, &pos);
	if (value < 0)
		return (1);
	rgb[0] = value;
	if (expect_comma(str, &pos) != 0)
		return (1);
	value = parse_single_rgb_value(str, &pos);
	if (value < 0)
		return (1);
	rgb[1] = value;
	if (expect_comma(str, &pos) != 0)
		return (1);
	value = parse_single_rgb_value(str, &pos);
	if (value < 0)
		return (1);
	rgb[2] = value;
	while (str[pos] && (str[pos] == ' ' || str[pos] == '\t'))
		pos++;
	if (str[pos] != '\0')
	{
		printf("Error\nUnexpected content after RGB values: '%c'\n", str[pos]);
		return (1);
	}
	return (0);
}
