#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 11 // 10 caratteri + terminatore '\0'

/*
 * STRUTTURA DATI GLOBALE
 * Per mantenere il codice pulito e passare meno parametri, raggruppiamo tutto qui.
 * Usiamo vettori di stringhe per i nomi e matrici di interi per le preferenze.
 * Gli interi rappresentano gli INDICI nei vettori dei nomi (0..N-1).
 */
typedef struct {
    int N;
    
    // Array di stringhe per i nomi (es. nomi_persone[0] = "p0")
    char **nomi_persone;
    char **nomi_aziende;

    // Matrici di preferenze:
    // pref_persone[i][j] = indice dell'azienda che è alla posizione j-esima per la persona i
    int **pref_persone;
    
    // pref_aziende[i][j] = indice della persona che è alla posizione j-esima per l'azienda i
    int **pref_aziende;

    // Matrice di "ranking" inverso per le aziende (OTTIMIZZAZIONE PER VERIFICA O(1))
    // rank_aziende[a][p] = quanto piace la persona p all'azienda a? (0 = massimo, N-1 = minimo)
    int **rank_aziende; 

    // Vettori per il matching corrente (Input da file)
    int *match_p_input; // match_p_input[indice_persona] = indice_azienda_assegnata

} ProblemData;

// --- PROTOTIPI ---

// Inizializzazione e Caricamento
ProblemData* inizializza_dati(int N);
void leggi_preferenze(ProblemData *pd, char *file_persone, char *file_aziende);
void leggi_matching_input(ProblemData *pd, char *file_match);
int trova_indice(char **nomi, int N, char *target);

// Funzioni Richieste
int verifica_stabilita(ProblemData *pd); // Ritorna 1 se perfetto, 0 se instabile
void algoritmo_gale_shapley(ProblemData *pd); // Algoritmo completo

// Gestione Memoria
void libera_tutto(ProblemData *pd);

#endif