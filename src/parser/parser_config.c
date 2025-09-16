/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_config.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:12:11 by edforte           #+#    #+#             */
/*   Updated: 2025/09/16 14:25:15 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	process_config_line(t_parse_data *parse, t_map *map, int line_index)
{
	int	ret;

	if (is_empty_line(parse->file_content[line_index]))
		return (0);
	
	if (is_map_line(parse->file_content[line_index]))
	{
		printf("\n");
		parse->map_start_line = line_index;
		return (0);
	}
	
	ret = parse_texture_line(parse->file_content[line_index], map, parse);
	if (ret == 0)
		return (0);
	
	ret = parse_color_line(parse->file_content[line_index], map, parse);
	if (ret == 0)
		return (0);
	
	printf("Error\nInvalid configuration line: %s\n",
		parse->file_content[line_index]);
	return (1);
}

int	parse_config_elements(t_parse_data *parse, t_map *map)
{
	int	i;
	int	ret;

	i = 0;
	parse->map_start_line = -1;
	while (parse->file_content[i])
	{
		ret = process_config_line(parse, map, i);
		if (ret == 1)
			return (1);
		if (parse->map_start_line != -1)
			break ;
		i++;
	}
	if (parse->map_start_line == -1)
	{
		printf("Error\nNo map found in file\n");
		return (1);
	}
	return (0);
}

int	get_texture_type_index(char *line)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		return (0);
	else if (ft_strncmp(line, "SO ", 3) == 0)
		return (1);
	else if (ft_strncmp(line, "WE ", 3) == 0)
		return (2);
	else if (ft_strncmp(line, "EA ", 3) == 0)
		return (3);
	return (-1);
}

char	*get_texture_identifier(int type_index)
{
	char *identifiers[4];

	identifiers[0] = "NO";
	identifiers[1] = "SO";
	identifiers[2] = "WE";
	identifiers[3] = "EA";
	if (type_index >= 0 && type_index < 4)
		return (identifiers[type_index]);
	return (NULL);
}

char	**get_texture_destination(int type_index, t_map *map)
{
	if (type_index == 0)
		return (&map->north_texture);
	else if (type_index == 1)
		return (&map->south_texture);
	else if (type_index == 2)
		return (&map->west_texture);
	else if (type_index == 3)
		return (&map->east_texture);
	return (NULL);
}

int	*get_loaded_flag(int type_index, t_parse_data *parse)
{
	if (type_index == 0)
		return (&parse->north_loaded);
	else if (type_index == 1)
		return (&parse->south_loaded);
	else if (type_index == 2)
		return (&parse->west_loaded);
	else if (type_index == 3)
		return (&parse->east_loaded);
	return (NULL);
}

char	*extract_and_clean_path(char *path_start, char *identifier)
{
	int		len;
	char	*result;

	while (*path_start == ' ' || *path_start == '\t')
		path_start++;
	if (*path_start == '\0')
	{
		printf("Error\nEmpty path for %s texture\n", identifier);
		return (NULL);
	}
	len = ft_strlen(path_start);
	while (len > 0 && (path_start[len - 1] == ' '
			|| path_start[len - 1] == '\t'))
		len--;
	path_start[len] = '\0';
	result = ft_strdup(path_start);
	if (!result)
	{
		printf("Error\nMemory allocation failed for %s texture path\n",
			identifier);
		return (NULL);
	}
	return (result);
}

int	store_texture_path(char *line, int type_index, t_map *map, t_parse_data *parse)
{
	char	*path_start;
	char	*cleaned_path;
	char	**dest_texture;
	int		*loaded_flag;

	loaded_flag = get_loaded_flag(type_index, parse);
	if (*loaded_flag)
	{
		printf("Error\nDuplicate %s texture definition\n", 
			get_texture_identifier(type_index));
		return (1);
	}
	path_start = line + 3;
	cleaned_path = extract_and_clean_path(path_start, 
		get_texture_identifier(type_index));
	if (!cleaned_path)
		return (1);
	dest_texture = get_texture_destination(type_index, map);
	*dest_texture = cleaned_path;
	*loaded_flag = 1;
	parse->texture_count++;
	return (0);
}

int	parse_texture_line(char *line, t_map *map, t_parse_data *parse)
{
	int		type_index;

	if (!line || !map || !parse)
		return (1);
	while (*line == ' ' || *line == '\t')
		line++;
	
	type_index = get_texture_type_index(line);
	if (type_index == -1)
		return (1);
	
	return (store_texture_path(line, type_index, map, parse));
}

int	get_color_type_index(char *line)
{
	if (ft_strncmp(line, "F ", 2) == 0)
		return (0);
	else if (ft_strncmp(line, "C ", 2) == 0)
		return (1);
	return (-1);
}

char	*get_color_identifier(int type_index)
{
	char *identifiers[2] = {"Floor", "Ceiling"};
	
	if (type_index >= 0 && type_index < 2)
		return (identifiers[type_index]);
	return (NULL);
}

int	*get_color_destination(int type_index, t_map *map)
{
	if (type_index == 0)
		return (map->floor_col);
	else if (type_index == 1)
		return (map->sky_col);
	return (NULL);
}

int	*get_color_loaded_flag(int type_index, t_parse_data *parse)
{
	if (type_index == 0)
		return (&parse->floor_set);
	else if (type_index == 1)
		return (&parse->ceiling_set);
	return (NULL);
}

int	check_duplicate_color(int type_index, t_parse_data *parse)
{
	int	*loaded_flag;

	loaded_flag = get_color_loaded_flag(type_index, parse);
	if (*loaded_flag)
	{
		printf("Error\nDuplicate %s color definition\n", 
			get_color_identifier(type_index));
		return (1);
	}
	return (0);
}

int	process_color_values(char *color_values, int type_index, t_map *map, t_parse_data *parse)
{
	int	*dest_color;
	int	*loaded_flag;

	while (*color_values == ' ' || *color_values == '\t')
		color_values++;
	if (*color_values == '\0')
	{
		printf("Error\nEmpty RGB values for %s color\n", 
			get_color_identifier(type_index));
		return (1);
	}
	dest_color = get_color_destination(type_index, map);
	if (extract_rgb_values(color_values, dest_color) != 0)
	{
		printf("Error\nInvalid RGB format for %s color\n", 
			get_color_identifier(type_index));
		return (1);
	}
	loaded_flag = get_color_loaded_flag(type_index, parse);
	*loaded_flag = 1;
	return (0);
}

int	parse_color_line(char *line, t_map *map, t_parse_data *parse)
{
	int		type_index;
	char	*color_values;

	if (!line || !map || !parse)
		return (1);
	while (*line == ' ' || *line == '\t')
		line++;
	
	type_index = get_color_type_index(line);
	if (type_index == -1)
		return (1);
	
	if (check_duplicate_color(type_index, parse) == 1)
		return (1);
	
	color_values = line + 2;
	return (process_color_values(color_values, type_index, map, parse));
}
int	validate_config_vertical(int error, t_map *map)
{
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
	return (error);
}
int	validate_config_horizontal(int error, t_map *map)
{
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
	return (error);
}
int	validate_config_complete(t_map *map, t_parse_data *parse)
{
	int	error;

	error = 0;
	if (validate_config_vertical(error, map))
		return (1);
	if (validate_config_horizontal(error, map))
		return (1);
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
		printf("Error\nEmpty RGB component\n");
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
