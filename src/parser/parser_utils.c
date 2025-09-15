/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edforte <edforte@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:11:51 by edforte           #+#    #+#             */
/*   Updated: 2025/08/25 10:30:59 by edforte          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

char **ft_split_whitespace(char *str)
{
    char    **result;
    int     word_count;
    int     i;
    int     j;
    int     start;
    
    /*
    ** CONTROLLO NULL
    */
    if (!str)
        return (NULL);
    
    /*
    ** CONTA LE PAROLE
    ** Una parola è una sequenza di caratteri non-whitespace
    ** separata da spazi o tab
    */
    word_count = 0;
    i = 0;
    while (str[i])
    {
        /*
        ** SKIP WHITESPACE
        ** Salta tutti gli spazi e tab consecutivi
        */
        while (str[i] && (str[i] == ' ' || str[i] == '\t'))
            i++;
        
        /*
        ** SE TROVIAMO UN CARATTERE NON-WHITESPACE
        ** Abbiamo trovato l'inizio di una parola
        */
        if (str[i])
        {
            word_count++;
            /*
            ** SKIP LA PAROLA
            ** Continua finché non trovi whitespace
            */
            while (str[i] && str[i] != ' ' && str[i] != '\t')
                i++;
        }
    }
    
    /*
    ** ALLOCAZIONE ARRAY
    ** +1 per il NULL terminator
    */
    result = (char **)malloc(sizeof(char *) * (word_count + 1));
    if (!result)
    {
        printf("Error\nMemory allocation failed in split_whitespace\n");
        return (NULL);
    }
    
    /*
    ** ESTRAZIONE PAROLE
    ** Seconda passata per estrarre e copiare le parole
    */
    i = 0;
    j = 0;  /* Indice nell'array result */
    while (str[i] && j < word_count)
    {
        /*
        ** SKIP WHITESPACE INIZIALE
        */
        while (str[i] && (str[i] == ' ' || str[i] == '\t'))
            i++;
        
        /*
        ** TROVA INIZIO E FINE PAROLA
        */
        if (str[i])
        {
            start = i;
            /*
            ** TROVA FINE PAROLA
            */
            while (str[i] && str[i] != ' ' && str[i] != '\t')
                i++;
            
            /*
            ** ALLOCA E COPIA LA PAROLA
            ** Lunghezza = i - start
            */
            result[j] = (char *)malloc(sizeof(char) * (i - start + 1));
            if (!result[j])
            {
                printf("Error\nMemory allocation failed for word\n");
                /*
                ** CLEANUP PARZIALE
                */
                while (j > 0)
                {
                    j--;
                    free(result[j]);
                }
                free(result);
                return (NULL);
            }
            
            /*
            ** COPIA CARATTERE PER CARATTERE
            */
            int k = 0;
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
    
    /*
    ** NULL TERMINATOR
    */
    result[j] = NULL;
    
    /*
    ** SUCCESSO
    ** Ritorna l'array di stringhe
    */
    return (result);
}

char *trim_spaces(char *str)
{
    int     start;
    int     end;
    int     len;
    char    *trimmed;
    int     i;
    
    /*
    ** CONTROLLO NULL
    */
    if (!str)
        return (NULL);
    
    /*
    ** TROVA PRIMO CARATTERE NON-WHITESPACE
    ** Parti dall'inizio e cerca il primo carattere
    ** che non sia spazio o tab
    */
    start = 0;
    while (str[start] && (str[start] == ' ' || str[start] == '\t'))
        start++;
    
    /*
    ** SE LA STRINGA È SOLO WHITESPACE
    ** Ritorna una stringa vuota
    */
    if (!str[start])
    {
        trimmed = (char *)malloc(sizeof(char) * 1);
        if (!trimmed)
            return (NULL);
        trimmed[0] = '\0';
        return (trimmed);
    }
    
    /*
    ** TROVA ULTIMO CARATTERE NON-WHITESPACE
    ** Parti dalla fine e torna indietro
    */
    len = ft_strlen(str);
    end = len - 1;
    while (end > start && (str[end] == ' ' || str[end] == '\t'))
        end--;
    
    /*
    ** CALCOLA LUNGHEZZA RISULTANTE
    ** +1 per includere l'ultimo carattere
    ** +1 per il terminatore
    */
    len = end - start + 1;
    
    /*
    ** ALLOCA NUOVA STRINGA
    */
    trimmed = (char *)malloc(sizeof(char) * (len + 1));
    if (!trimmed)
    {
        printf("Error\nMemory allocation failed in trim_spaces\n");
        return (NULL);
    }
    
    /*
    ** COPIA LA PARTE TRIMMED
    */
    i = 0;
    while (i < len)
    {
        trimmed[i] = str[start + i];
        i++;
    }
    trimmed[i] = '\0';
    
    /*
    ** SUCCESSO
    ** Ritorna la stringa senza spazi iniziali/finali
    */
    return (trimmed);
}

int **allocate_2d_int_array(int height, int width)
{
    int **array;
    int i;
    
    /*
    ** CONTROLLO DIMENSIONI VALIDE
    ** Height e width devono essere positivi
    */
    if (height <= 0 || width <= 0)
    {
        printf("Error\nInvalid dimensions for 2D array\n");
        return (NULL);
    }
    
    /*
    ** ALLOCAZIONE ARRAY DI PUNTATORI
    ** Prima allochiamo l'array di puntatori alle righe
    */
    array = (int **)malloc(sizeof(int *) * height);
    if (!array)
    {
        printf("Error\nMemory allocation failed for 2D array\n");
        return (NULL);
    }
    
    /*
    ** ALLOCAZIONE RIGHE
    ** Ogni riga è un array di int
    */
    i = 0;
    while (i < height)
    {
        array[i] = (int *)malloc(sizeof(int) * width);
        if (!array[i])
        {
            /*
            ** ERRORE: CLEANUP PARZIALE
            ** Libera le righe già allocate
            */
            printf("Error\nMemory allocation failed for row %d\n", i);
            while (i > 0)
            {
                i--;
                free(array[i]);
            }
            free(array);
            return (NULL);
        }
        
        /*
        ** INIZIALIZZAZIONE A ZERO (OPZIONALE)
        ** Utile per evitare valori random
        */
        int j = 0;
        while (j < width)
        {
            array[i][j] = 0;
            j++;
        }
        
        i++;
    }
    
    /*
    ** SUCCESSO
    ** Ritorna l'array 2D allocato e inizializzato
    */
    return (array);
}

void free_2d_int_array(int **array, int height)
{
    int i;
    
    /*
    ** CONTROLLO NULL
    ** Se l'array è già NULL, non c'è niente da liberare
    */
    if (!array)
        return;
    
    /*
    ** LIBERA OGNI RIGA
    ** Partiamo dalla prima riga e liberiamo tutte
    */
    i = 0;
    while (i < height)
    {
        /*
        ** CONTROLLO NULL PER OGNI RIGA
        ** Una riga potrebbe essere NULL se l'allocazione
        ** è fallita parzialmente
        */
        if (array[i])
        {
            free(array[i]);
            array[i] = NULL;  /* Buona pratica: setta a NULL dopo free */
        }
        i++;
    }
    
    /*
    ** LIBERA L'ARRAY DI PUNTATORI
    ** Dopo aver liberato tutte le righe,
    ** liberiamo l'array principale
    */
    free(array);
    
    /*
    ** NOTA: Non possiamo settare array = NULL qui
    ** perché è passato per valore, non per riferimento
    ** Il chiamante dovrebbe farlo dopo questa funzione
    */
}

void free_split(char **split)
{
    int i;
    
    /*
    ** CONTROLLO NULL
    ** Se split è NULL, non c'è niente da liberare
    */
    if (!split)
        return;
    
    /*
    ** LIBERA OGNI STRINGA
    ** L'array è terminato da NULL, quindi continuiamo
    ** finché non lo troviamo
    */
    i = 0;
    while (split[i])
    {
        /*
        ** LIBERA LA STRINGA
        ** Ogni elemento è una stringa allocata dinamicamente
        */
        free(split[i]);
        split[i] = NULL;  /* Buona pratica per evitare double-free */
        i++;
    }
    
    /*
    ** LIBERA L'ARRAY DI PUNTATORI
    ** Dopo aver liberato tutte le stringhe,
    ** liberiamo l'array stesso
    */
    free(split);
    
    /*
    ** NOTA: Questa funzione è generica e funziona per:
    ** - Output di ft_split()
    ** - Output di ft_split_whitespace()
    ** - Qualsiasi array di stringhe NULL-terminated
    */
}


