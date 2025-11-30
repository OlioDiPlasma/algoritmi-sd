/* * --- LE "GUARDIE" (Include Guards) ---
 * #ifndef e #define servono a evitare che questo file venga incluso 
 * due volte nello stesso progetto, cosa che causerebbe errori di "ridefinizione".
 * È lo standard per ogni file .h in C.
 */
#ifndef MODELLO_H
#define MODELLO_H

/* Librerie standard necessarie per i tipi usati qui sotto */
#include <stdio.h>  // Per i tipi FILE* (se servissero)
#include <stdlib.h> // Per size_t o gestione memoria
#include <string.h> // Per manipolazione stringhe

/* * --- COSTANTI (Macro) ---
 * MAX_N: Dimensione massima della griglia. Anche se nel file l'input è più piccolo,
 * noi prenotiamo sempre spazio per una 20x20. Questo evita di dover fare malloc complesse 
 * per la griglia. Si spreca un po' di RAM per guadagnare semplicità.
 */
#define MAX_N 20

/* * TOKEN_LEN: Lunghezza massima di una stringa nella cella + terminatore.
 * Se le stringhe sono di 2 caratteri (es. "55"), serve spazio per 3 char:
 * '5', '5', '\0' (il terminatore nullo obbligatorio in C).
 */
#define TOKEN_LEN 3

/* * --- STRUCT COORD ---
 * Una semplice "scatola" per tenere insieme riga e colonna.
 * Utile per passare coordinate tra funzioni senza passare due variabili separate.
 */
typedef struct {
    int r, c;
} Coord;

/* * --- STRUCT BONUS ---
 * Rappresenta una singola regola bonus (es: sequenza "A1 B2" vale 10 punti).
 */
typedef struct {
    int id;         // Identificativo univoco
    int valore;     // Punti che vale il bonus
    int len_seq;    // Quanto è lunga la sequenza (es. 2, 3, 4...)
    
    /* * --- PUNTO CRITICO: DOPPIO PUNTATORE ---
     * char **sequenza:
     * Non sappiamo a priori quanto è lunga la sequenza, né cosa contiene.
     * È un puntatore a un array di puntatori (un array di stringhe).
     * Richiede DOPPIA malloc:
     * 1. malloc per l'array di puntatori (char*)
     * 2. malloc per ogni singola stringa (char)
     */
    char **sequenza; 
} Bonus;

/* * --- STRUCT GAMEDATA ---
 * È il contenitore principale di tutto il gioco.
 */
typedef struct {
    int N; // Dimensione reale della griglia letta dal file (es. 5)

    /* * --- PUNTO CRITICO: MATRICE STATICA ---
     * char grid[...][...][...]:
     * Qui NON serve malloc. Quando crei un oggetto GameData, questa memoria
     * (20 * 20 * 3 byte) viene creata automaticamente dentro la struct.
     * È una matrice 3D perché ogni cella [r][c] contiene una stringa (array di char).
     */
    char grid[MAX_N][MAX_N][TOKEN_LEN];

    int num_bonus; // Numero totale di bonus disponibili
    
    /* * --- ARRAY DINAMICO ---
     * Bonus *lista_bonus:
     * Qui invece serve malloc. È solo un puntatore che attende di sapere
     * dove si trova l'array dei bonus in memoria heap.
     */
    Bonus *lista_bonus;

    int L; // Lunghezza del buffer obiettivo (parametro del gioco)
} GameData;

/* * --- PROTOTIPI DI FUNZIONE ---
 * Dicono al compilatore: "Esistono queste funzioni, fidati. 
 * Il codice vero e proprio lo troverai nel file .c".
 */
GameData* carica_dati(int L, const char* file_grid, const char* file_bonus);
void libera_dati(GameData *game);

#endif // Chiusura dell'#ifndef iniziale