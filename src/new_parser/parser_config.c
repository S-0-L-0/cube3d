/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_config.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:12:11 by edforte           #+#    #+#             */
/*   Updated: 2025/08/30 18:39:57 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int parse_config_elements(t_parse_data *parse, t_map *map)
{
    int i;
    int ret;
    
    /*
    ** INIZIALIZZAZIONE
    ** Partiamo dalla prima riga del file
    ** Scorreremo fino a trovare l'inizio della mappa
    */
    i = 0;
    parse->map_start_line = -1;
    
    /*
    ** LOOP PRINCIPALE DI PARSING CONFIGURAZIONE
    ** Continua finché:
    ** 1. Ci sono ancora righe da leggere
    ** 2. Non abbiamo ancora trovato la mappa
    */

    while (parse->file_content[i])
    {
        /*
        ** SKIP LINEE VUOTE
        ** Le linee vuote sono permesse tra gli elementi di config
        ** Le saltiamo semplicemente
        */
        
        if (is_empty_line(parse->file_content[i]))
        {
            i++;
            continue;
        }
        
        /*
        ** CONTROLLO INIZIO MAPPA
        ** Se questa linea sembra essere l'inizio della mappa,
        ** salviamo l'indice e usciamo dal parsing della configurazione
        */
        if (is_map_line(parse->file_content[i]))
        {   
            printf("\n");
            parse->map_start_line = i;
            break;  /* Mappa trovata, stop parsing config */
        }
        
        /*
        ** TENTATIVO PARSING TEXTURE
        ** Proviamo a parsare la linea come texture (NO/SO/WE/EA)
        ** Se ha successo (ritorna 0), passiamo alla prossima linea
        */
        ret = parse_texture_line(parse->file_content[i], map, parse);
        if (ret == 0)
        {
            i++;
            continue;
        }
        
        /*
        ** TENTATIVO PARSING COLORE
        ** Se non era una texture, proviamo come colore (F/C)
        ** Se ha successo (ritorna 0), passiamo alla prossima linea
        */
        ret = parse_color_line(parse->file_content[i], map, parse);
        if (ret == 0)
        {
            i++;
            continue;
        }
        
        /*
        ** LINEA NON VALIDA
        ** Se arriviamo qui, la linea non è:
        ** - Una linea vuota (già controllato)
        ** - Una linea di mappa (già controllato)
        ** - Una texture valida (parse_texture_line ha fallito)
        ** - Un colore valido (parse_color_line ha fallito)
        ** 
        ** Quindi è un errore di formato
        */
        printf("Error\nInvalid configuration line: %s\n", 
               parse->file_content[i]);
        return (1);
    }
    
    /*
    ** VERIFICA CHE LA MAPPA SIA STATA TROVATA
    ** Se siamo usciti dal loop senza trovare la mappa
    ** (map_start_line è ancora 0), il file non contiene una mappa
    */
    if (parse->map_start_line == -1)
    {

        printf("Error\nNo map found in file\n");
        return (1);
    }
    
    /*
    ** SUCCESSO
    ** Abbiamo parsato tutta la configurazione
    ** e trovato l'inizio della mappa
    */
    return (0);
}

int parse_texture_line(char *line, t_map *map, t_parse_data *parse)
{
    char    *identifier;
    char    *path_start;
    char    **dest_texture;
    int     *loaded_flag;
    
    /*
    ** CONTROLLO NULL
    ** Sicurezza base per evitare segfault
    */
    
    if (!line || !map || !parse)
        return (1);
    
    /*
    ** SKIP SPAZI INIZIALI
    ** Trova il primo carattere non-spazio
    */
    while (*line == ' ' || *line == '\t')
        line++;
    
    /*
    ** IDENTIFICAZIONE TIPO TEXTURE
    ** Controlliamo quale texture stiamo parsando
    ** e impostiamo i puntatori appropriati
    */
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
        /*
        ** NON È UNA TEXTURE
        ** Ritorna 1 per indicare che questa linea
        ** non è una texture (potrebbe essere un colore)
        */
        return (1);
    }
    
    /*
    ** CONTROLLO DUPLICATI
    ** Se questa texture è già stata definita, è un errore
    */
    
    if (*loaded_flag)
    {
        printf("Error\nDuplicate %s texture definition\n", identifier);
        return (1);
    }
    
    /*
    ** SKIP SPAZI DOPO L'IDENTIFIER
    ** Ci possono essere spazi tra "NO" e il path
    */
    while (*path_start == ' ' || *path_start == '\t')
        path_start++;
    
    /*
    ** VERIFICA PATH NON VUOTO
    ** Dopo l'identifier deve esserci un path
    */
    if (*path_start == '\0')
    {
        printf("Error\nEmpty path for %s texture\n", identifier);
        return (1);
    }
    
    /*
    ** TRIM SPAZI FINALI DAL PATH
    ** Dobbiamo rimuovere eventuali spazi alla fine del path
    ** Troviamo la fine della stringa e torniamo indietro
    */
    int len = ft_strlen(path_start);
    while (len > 0 && (path_start[len - 1] == ' ' || 
           path_start[len - 1] == '\t'))
        len--;
    path_start[len] = '\0';  /* Termina la stringa senza spazi finali */
    
    /*
    ** DUPLICAZIONE PATH
    ** Usiamo ft_strdup per allocare e copiare il path
    ** Se non hai ft_strdup, usa la versione manuale sotto
    */
    *dest_texture = ft_strdup(path_start);
    if (!*dest_texture)
    {
        printf("Error\nMemory allocation failed for %s texture path\n", 
               identifier);
        return (1);
    }
    
    /*
    ** MARCA COME CARICATA
    ** Impostiamo il flag per evitare duplicati
    */
    *loaded_flag = 1;
    parse->texture_count++;
    
    /*
    ** SUCCESSO
    ** La texture è stata parsata correttamente
    */
    return (0);
}

int parse_color_line(char *line, t_map *map, t_parse_data *parse)
{
    char    *color_values;
    int     *dest_color;
    int     *loaded_flag;
    char    *identifier;
    
    /*
    ** CONTROLLO NULL
    */
    if (!line || !map || !parse)
        return (1);
    
    /*
    ** SKIP SPAZI INIZIALI
    */
    while (*line == ' ' || *line == '\t')
        line++;
    
    /*
    ** IDENTIFICAZIONE TIPO COLORE
    ** F = Floor (pavimento)
    ** C = Ceiling (soffitto)
    */
    if (ft_strncmp(line, "F ", 2) == 0)
    {
        identifier = "Floor";
        dest_color = map->floor_color;
        loaded_flag = &parse->floor_set;
        color_values = line + 2;
    }
    else if (ft_strncmp(line, "C ", 2) == 0)
    {
        identifier = "Ceiling";
        dest_color = map->ceiling_color;
        loaded_flag = &parse->ceiling_set;
        color_values = line + 2;
    }
    else
    {
        /*
        ** NON È UN COLORE
        ** Ritorna 1 per indicare che non è una linea di colore
        */
        return (1);
    }
    
    /*
    ** CONTROLLO DUPLICATI
    */
    if (*loaded_flag)
    {
        printf("Error\nDuplicate %s color definition\n", identifier);
        return (1);
    }
    
    /*
    ** SKIP SPAZI DOPO L'IDENTIFIER
    */
    while (*color_values == ' ' || *color_values == '\t')
        color_values++;
    
    /*
    ** VERIFICA VALORI NON VUOTI
    */
    if (*color_values == '\0')
    {
        printf("Error\nEmpty RGB values for %s color\n", identifier);
        return (1);
    }
    
    /*
    ** ESTRAZIONE VALORI RGB
    ** Delega a funzione helper per parsare "R,G,B"
    */
    if (extract_rgb_values(color_values, dest_color) != 0)
    {
        printf("Error\nInvalid RGB format for %s color\n", identifier);
        return (1);
    }
    
    /*
    ** MARCA COME CARICATO
    */
    *loaded_flag = 1;
    
    /*
    ** SUCCESSO
    */
    return (0);
}

int validate_config_complete(t_map *map, t_parse_data *parse)
{
    int error;
    
    /*
    ** INIZIALIZZAZIONE FLAG ERRORE
    ** Useremo questo per tracciare se ci sono errori
    ** Continuiamo a controllare tutto anche se troviamo errori
    ** per dare all'utente una lista completa di cosa manca
    */
    error = 0;
    
    /*
    ** VERIFICA TEXTURE NORTH
    */
    if (!map->north_texture)
    {
        printf("Error\nMissing North (NO) texture\n");
        error = 1;
    }
    else if (validate_texture_file(map->north_texture, "North") != 0)
    {
        error = 1;  /* Errore già stampato da validate_texture_file */
    }
    
    /*
    ** VERIFICA TEXTURE SOUTH
    */
    if (!map->south_texture)
    {
        printf("Error\nMissing South (SO) texture\n");
        error = 1;
    }
    else if (validate_texture_file(map->south_texture, "South") != 0)
    {
        error = 1;
    }
    
    /*
    ** VERIFICA TEXTURE WEST
    */
    if (!map->west_texture)
    {
        printf("Error\nMissing West (WE) texture\n");
        error = 1;
    }
    else if (validate_texture_file(map->west_texture, "West") != 0)
    {
        error = 1;
    }
    
    /*
    ** VERIFICA TEXTURE EAST
    */
    if (!map->east_texture)
    {
        printf("Error\nMissing East (EA) texture\n");
        error = 1;
    }
    else if (validate_texture_file(map->east_texture, "East") != 0)
    {
        error = 1;
    }
    
    /*
    ** VERIFICA COLORE FLOOR
    ** Ora possiamo usare i flag da parse_data
    ** per sapere se il colore è stato definito
    */
    if (!parse->floor_set)
    {
        printf("Error\nMissing Floor (F) color\n");
        error = 1;
    }
    
    /*
    ** VERIFICA COLORE CEILING
    ** Stesso controllo per il ceiling
    */
    if (!parse->ceiling_set)
    {
        printf("Error\nMissing Ceiling (C) color\n");
        error = 1;
    }
    
    /*
    ** RITORNO RISULTATO
    ** Se error è 0, tutto ok
    ** Se error è 1, almeno un elemento manca o è invalido
    */
    return (error);
}

static int parse_single_rgb_value(char *str, int *pos)
{
    int value = 0;
    int has_digit = 0;
    
    /* Skip leading whitespace */
    while (str[*pos] && (str[*pos] == ' ' || str[*pos] == '\t'))
        (*pos)++;
    
    /* Parse digits */
    while (str[*pos] && str[*pos] >= '0' && str[*pos] <= '9')
    {
        value = value * 10 + (str[*pos] - '0');
        has_digit = 1;
        
        /* Early overflow check */
        if (value > 255)
        {
            printf("Error\nRGB value exceeds 255\n");
            return (-1);
        }
        (*pos)++;
    }
    
    /* Skip trailing whitespace */
    while (str[*pos] && (str[*pos] == ' ' || str[*pos] == '\t'))
        (*pos)++;
    
    /* Must have at least one digit */
    if (!has_digit)
    {
        printf("Error\nEmpty RGB component\n");
        return (-1);
    }
    
    return (value);
}

/*
** HELPER: Expect and consume a comma at current position
** Returns 0 on success, 1 on error
*/
static int expect_comma(char *str, int *pos)
{
    if (str[*pos] != ',')
    {
        printf("Error\nExpected comma in RGB format\n");
        return (1);
    }
    (*pos)++;
    return (0);
}

/*
** MAIN FUNCTION: Simplified RGB parser using helper functions
*/

int extract_rgb_values(char *str, int *rgb)
{
    int pos = 0;
    int value;
    
    if (!str || !rgb)
        return (1);
    
    
    /* Parse R value */
    value = parse_single_rgb_value(str, &pos);
    if (value < 0)
        return (1);
    rgb[0] = value;
    
    /* Expect comma */
    if (expect_comma(str, &pos) != 0)
        return (1);
    
    /* Parse G value */
    value = parse_single_rgb_value(str, &pos);
    if (value < 0)
        return (1);
    rgb[1] = value;
    
    /* Expect comma */
    if (expect_comma(str, &pos) != 0)
        return (1);
    
    /* Parse B value */
    value = parse_single_rgb_value(str, &pos);
    if (value < 0)
        return (1);
    rgb[2] = value;
    
    /* Check for trailing content */
    while (str[pos] && (str[pos] == ' ' || str[pos] == '\t'))
        pos++;
    
    if (str[pos] != '\0')
    {
        printf("Error\nUnexpected content after RGB values: '%c'\n", str[pos]);
        return (1);
    }
    
    return (0);
}
