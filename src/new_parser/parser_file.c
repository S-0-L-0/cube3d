/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:12:07 by edforte           #+#    #+#             */
/*   Updated: 2025/08/23 16:10:18 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int read_file_content(int fd, t_parse_data *parse)
{
    char    *line;
    int     line_count;
    int     len;
    
    /*
    ** ALLOCAZIONE ARRAY FISSO
    ** Allochiamo spazio per 1000 puntatori a char*
    ** Questo è più che sufficiente per qualsiasi mappa ragionevole
    ** L'ultimo slot sarà riservato per il NULL terminator
    */
    parse->file_content = (char **)malloc(sizeof(char *) * 1000);
    if (!parse->file_content)
    {
        printf("Error\nMemory allocation failed\n");
        return (1);
    }
    
    /*
    ** INIZIALIZZAZIONE CONTATORE
    ** line_count tiene traccia di quante righe abbiamo letto
    ** Serve anche come indice per l'array file_content
    */
    line_count = 0;
    
    /*
    ** LETTURA DEL FILE RIGA PER RIGA
    ** Usiamo get_next_line per leggere una riga alla volta
    ** Continuiamo finché GNL non ritorna NULL (fine file)
    ** o finché non raggiungiamo il limite di 999 righe
    */
    line = get_next_line(fd);
    while (line != NULL && line_count < 999)
    {
        /*
        ** RIMOZIONE NEWLINE FINALE
        ** GNL include il \n alla fine della riga (tranne l'ultima)
        ** Lo rimuoviamo per semplificare il parsing successivo
        */
        len = 0;
        while (line[len])
            len++;
        
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';
        
        /*
        ** SALVATAGGIO RIGA NELL'ARRAY
        ** Salviamo il puntatore alla riga nell'array
        ** GNL ha già allocato la memoria per la stringa
        */
        parse->file_content[line_count] = line;
        line_count++;
        
        /*
        ** LETTURA PROSSIMA RIGA
        ** Continuiamo a leggere finché c'è contenuto
        */
        line = get_next_line(fd);
    }
    
    /*
    ** CONTROLLO OVERFLOW
    ** Se line non è NULL significa che c'erano ancora righe
    ** ma abbiamo raggiunto il limite massimo di 999
    */
    if (line != NULL)
    {
        printf("Error\nFile too large (maximum 999 lines)\n");
        free(line);  /* Libera l'ultima riga non salvata */
        
        /* Libera tutte le righe già lette */
        while (line_count > 0)
        {
            line_count--;
            free(parse->file_content[line_count]);
        }
        free(parse->file_content);
        parse->file_content = NULL;
        return (1);
    }
    
    /*
    ** TERMINATORE NULL
    ** Aggiungiamo NULL alla fine dell'array
    ** Questo ci permette di iterare sull'array fino a trovare NULL
    ** come si fa con le stringhe che terminano con '\0'
    */
    parse->file_content[line_count] = NULL;
    
    /*
    ** SUCCESSO!
    ** Il file è stato letto completamente in memoria
    ** parse->file_content contiene tutte le righe
    ** parse->file_content[line_count] = NULL marca la fine
    */
    return (0);
}

int is_empty_line(char *line)
{
    int i;
    
    /*
    ** CONTROLLO NULL
    ** Se la linea è NULL, la consideriamo vuota
    ** Questo può succedere se ci sono problemi di allocazione
    */
    if (!line)
        return (1);
    
    /*
    ** SCANSIONE DELLA LINEA
    ** Scorriamo ogni carattere della linea
    ** Se troviamo un carattere che non è whitespace, non è vuota
    */
    i = 0;
    while (line[i])
    {
        /*
        ** CONTROLLO CARATTERE NON-WHITESPACE
        ** Spazio ' ' = 32 in ASCII
        ** Tab '\t' = 9 in ASCII
        ** Newline '\n' = 10 in ASCII (non dovrebbe esserci se l'abbiamo rimosso)
        ** Se troviamo qualsiasi altro carattere, la linea non è vuota
        */
        if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
        {
            /* Trovato un carattere significativo, linea non vuota */
            return (0);
        }
        i++;
    }
    
    /*
    ** LINEA VUOTA
    ** Abbiamo controllato tutti i caratteri e sono tutti whitespace
    ** oppure la stringa è vuota (solo '\0')
    */
    return (1);
}

int is_map_line(char *line)
{
    int i;
    int has_map_char;
    
    /*
    ** CONTROLLO NULL
    ** Se la linea è NULL, non può essere una linea di mappa
    ** Ritorniamo 0 (false) - non è una linea di mappa
    */
    if (!line)
        return (0);
    
    /*
    ** INIZIALIZZAZIONE FLAG
    ** has_map_char: indica se abbiamo trovato almeno un carattere di mappa
    ** Serve per distinguere linee di soli spazi da linee di mappa con spazi
    ** Una linea di soli spazi/tab NON è considerata inizio mappa
    */
    has_map_char = 0;
    i = 0;
    
    /*
    ** SCANSIONE COMPLETA DELLA LINEA
    ** Dobbiamo controllare TUTTI i caratteri della linea per verificare:
    ** 1. Che TUTTI i caratteri siano validi per una mappa
    ** 2. Che ci sia ALMENO UN carattere significativo di mappa
    ** 
    ** Se anche UN SOLO carattere non è valido → NON è una linea di mappa
    */
    while (line[i])
    {
        /*
        ** VERIFICA CARATTERE PER CARATTERE
        ** I caratteri validi in una linea di mappa sono SOLO:
        ** - '0': spazio percorribile dal player
        ** - '1': muro non attraversabile
        ** - 'N': player spawn facing North
        ** - 'S': player spawn facing South
        ** - 'E': player spawn facing East
        ** - 'W': player spawn facing West
        ** - ' ': spazio vuoto nella mappa (buco/void)
        ** - '\t': tab (trattato come spazio)
        ** 
        ** QUALSIASI altro carattere rende la linea NON-mappa
        */
        if (line[i] != '0' && line[i] != '1' && 
            line[i] != ' ' && line[i] != '\t' &&
            line[i] != 'N' && line[i] != 'S' && 
            line[i] != 'E' && line[i] != 'W')
        {
            /*
            ** CARATTERE NON VALIDO TROVATO
            ** Questa linea contiene un carattere che non appartiene alla mappa
            ** Potrebbe essere:
            ** - Una linea di texture (contiene '.', '/', lettere del path)
            ** - Una linea di colore (contiene 'F', 'C', numeri, virgole)
            ** - Una linea con errori (contiene caratteri random)
            ** 
            ** In ogni caso, NON è una linea di mappa valida
            ** Ritorniamo 0 (false) immediatamente
            */
            return (0);
        }
        
        /*
        ** CONTROLLO PRESENZA CARATTERI SIGNIFICATIVI
        ** Una linea per essere considerata "inizio mappa" deve contenere
        ** almeno un carattere che non sia solo whitespace.
        ** 
        ** Esempio:
        ** "     " → has_map_char resta 0 → NON è inizio mappa
        ** "  1  " → has_map_char diventa 1 → È inizio mappa
        ** "0000" → has_map_char diventa 1 → È inizio mappa
        */
        if (line[i] == '0' || line[i] == '1' ||
            line[i] == 'N' || line[i] == 'S' ||
            line[i] == 'E' || line[i] == 'W')
        {
            has_map_char = 1;  /* Trovato almeno un carattere di mappa */
        }
        
        i++;
    }
    
    /*
    ** DECISIONE FINALE
    ** A questo punto abbiamo scansionato TUTTA la linea
    ** 
    ** Se siamo arrivati qui significa che:
    ** - TUTTI i caratteri sono validi per una mappa (0,1,N,S,E,W,spazi)
    ** 
    ** Ma dobbiamo ancora verificare:
    ** - C'è almeno UN carattere significativo? (has_map_char)
    ** 
    ** RITORNIAMO:
    ** - 1 (true) se la linea può essere parte della mappa
    ** - 0 (false) se è solo whitespace
    */
    return (has_map_char);
}
