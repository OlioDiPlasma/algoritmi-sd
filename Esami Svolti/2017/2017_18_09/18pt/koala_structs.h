/* * File: koala_structs.h
 * Scopo: Definizione delle strutture dati e costanti globali.
 * Qui definiamo il "modello" del nostro problema.
 */

#ifndef KOALA_STRUCTS_H
#define KOALA_STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 21 // Lunghezza massima stringhe (20 + terminatore)

/* --- STRUTTURE DATI --- */

/* Rappresenta un Koala */
typedef struct {
    char id[MAX_STR];       // Es: "K7"
    int family_idx;         // Indice della famiglia nel vettore globale delle famiglie
    int n_habitats;         // Numero di alberi su cui può vivere
    int *habitats;          // Vettore dinamico degli indici degli alberi ammissibili
    int assigned_tree;      // Per l'ottimizzazione: albero attualmente assegnato (-1 se nessuno)
} Koala;

/* Rappresenta una Famiglia */
typedef struct {
    char id[MAX_STR];       // Es: "F2"
    // Non serve memorizzare la lista dei koala qui per l'algoritmo, 
    // basta sapere per ogni Koala a quale famiglia appartiene.
} Family;

/* * Gestione Nemici:
 * Useremo una Matrice di Adiacenza (S x S) allocata dinamicamente nel main.
 * enemies[i][j] = 1 se la famiglia i e j sono nemiche, 0 altrimenti.
 */

/* --- PROTOTIPI FUNZIONI --- */

/* Funzioni di utilità per trovare indici basati su stringhe */
int get_koala_index(Koala *koalas, int n, char *id);
int get_family_index(Family *families, int s, char *id);

/* Funzioni principali richieste */
int verify_solution(char *filename, Koala *koalas, int N, int S, int T, int m, int **enemies_mat);
void solve_optimization(Koala *koalas, int N, int S, int T, int m, int **enemies_mat);

#endif