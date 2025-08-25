/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:11:29 by edforte           #+#    #+#             */
/*   Updated: 2025/08/25 15:43:02 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"


int validate_arguments(int argc)
{

    /*
    ** CONTROLLO 1: NUMERO DI ARGOMENTI
    ** Il programma deve ricevere esattamente 2 argomenti:
    ** argv[0] = nome del programma (./cub3d)
    ** argv[1] = path del file mappa (maps/test.cub)
    */
    if (argc != 2)
    {
        if (argc < 2)
            printf("Error\nNo map file provided\n");
        else
            printf("Error\nToo many arguments\n");
        printf("Usage: ./cub3d <map.cub>\n");
        return (1);
    }
    return (0);
}

int open_and_validate_file(char *filepath)
{
    int fd;
    
    /*
    ** APERTURA DEL FILE
    ** Usiamo open() con O_RDONLY per aprire in sola lettura
    ** Se il file non esiste o non abbiamo permessi, open ritorna -1
    */
    fd = open(filepath, O_RDONLY);
    
    /*
    ** GESTIONE ERRORE
    ** Se l'apertura fallisce, stampiamo un messaggio di errore
    ** Non ci interessa distinguere il tipo di errore specifico
    */
    if (fd == -1)
    {
        printf("Error\nCannot open file: %s\n", filepath);
        return (-1);
    }
    
    /*
    ** SUCCESSO!
    ** Il file esiste ed è apribile, ritorniamo il file descriptor
    ** Sarà responsabilità di read_file_content() gestire la lettura
    */
    return (fd);
}

int validate_texture_file(char *path, char *texture_name)
{
    int len;
    
    /* Verifica che il path esista */
    if (!path)
    {
        printf("Error\n%s texture not defined\n", texture_name);
        return (1);
    }
    
    /* Calcola lunghezza e verifica estensione .xpm */
    len = ft_strlen(path);
    if (len < 5 || ft_strncmp(&path[len - 4], ".xpm", 4) != 0)
    {
        printf("Error\n%s texture must be a .xpm file\n", texture_name);
        return (1);
    }
    
    return (0);
}
