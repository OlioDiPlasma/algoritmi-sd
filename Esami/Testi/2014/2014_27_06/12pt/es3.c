#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * FUNZIONE HELPER RICORSIVA: build_menu_rec
 * ============================================================================
 * SCOPO:
 * Funzione "motore" della ricorsione. Esplora le scelte per la portata corrente
 * (livello) e scende in profondità per la portata successiva.
 *
 * PARAMETRI:
 * - char **data[]: La struttura dati principale (il menu del ristorante).
 * - int n:         Numero totale di portate (profondità massima dell'albero).
 * - int level:     La portata che stiamo scegliendo ORA (indice corrente).
 * - char **current_menu: Vettore di appoggio dove salviamo le scelte fatte
 * finora (es: [0]="pasta", [1]="carne"...).
 *
 * CONCETTI CHIAVE:
 * 1. Backtracking Implicito:
 * Non dobbiamo cancellare esplicitamente la scelta fatta quando torniamo su
 * dalla ricorsione, perché verrà sovrascritta dalla prossima iterazione del ciclo.
 *
 * 2. Navigazione della Struttura:
 * data[level] ci dà il puntatore all'array di stringhe della portata corrente.
 * Scorriamo questo array finché non troviamo NULL (sentinella).
 * ============================================================================
 */
void build_menu_rec(char **data[], int n, int level, char **current_menu) {

    /* --- 1. CASO BASE (Terminazione) --- */
    /* Abbiamo scelto un piatto per tutte le 'n' portate (da 0 a n-1) */
    if (level == n) {
        /* Stampa della combinazione completa trovata */
        printf("(");
        for (int i = 0; i < n; i++) {
            printf("%s", current_menu[i]);
            /* Aggiungi virgola se non è l'ultimo elemento */
            if (i < n - 1) printf(", ");
        }
        printf(")\n");
        return; // Torna al livello precedente
    }

    /* --- 2. PASSO RICORSIVO (Espansione) --- */
    /*
     * Recuperiamo l'elenco dei piatti disponibili per la portata corrente 'level'.
     * dishes_list è un puntatore al primo piatto (char *).
     */
    char **dishes_list = data[level];

    /* Ciclo su tutti i piatti di questa portata finché non trovo NULL (la 'x' nel disegno) */
    int i = 0;
    while (dishes_list[i] != NULL) {

        /* A. SCELTA: Seleziono l'i-esimo piatto per questo livello */
        current_menu[level] = dishes_list[i];

        /* B. RICORSIONE: Chiedo di completare il menu dal livello successivo in poi */
        build_menu_rec(data, n, level + 1, current_menu);

        /* C. BACKTRACKING: Passo al prossimo piatto (i++) */
        i++;
    }
}

/* ============================================================================
 * FUNZIONE WRAPPER: build_menu
 * ============================================================================
 * SCOPO:
 * Interfaccia richiesta dal testo. Prepara la memoria ausiliaria e lancia la ricorsione.
 *
 * PARAMETRI:
 * - char **data[]: L'array di puntatori ai menu delle portate.
 * - int n:         Il numero di portate.
 *
 * GESTIONE MEMORIA:
 * Dobbiamo allocare un array temporaneo `current_selection` per memorizzare
 * il menu che stiamo costruendo man mano che scendiamo nell'albero di ricorsione.
 * Dimensione: n puntatori a stringa.
 * ============================================================================
 */
void build_menu(char **data[], int n) {
    /* Controllo input difensivo */
    if (n <= 0 || data == NULL) return;

    /* --- 1. ALLOCAZIONE BUFFER --- */
    /*
     * Allocazione dinamica del vettore delle scelte.
     * sizeof(char *) perché contiene PUNTATORI alle stringhe originali (non copie).
     */
    char **current_selection = (char **)malloc(n * sizeof(char *));
    
    if (current_selection == NULL) {
        printf("Errore critico: memoria insufficiente.\n");
        return;
    }

    /* --- 2. LANCIO ALGORITMO --- */
    printf("--- Combinazioni Possibili del Menu ---\n");
    build_menu_rec(data, n, 0, current_selection);

    /* --- 3. PULIZIA --- */
    free(current_selection);
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: Ricostruire ESATTAMENTE la struttura dati mostrata nell'immagine
 * e testare la funzione.
 *
 * Concetto Struttura Dati (Char ***):
 * data -> [Ptr1, Ptr2, Ptr3]
 * |     |     |
 * V     V     V
 * [S1,S2,X] ...  [S5,S6,X]
 *
 * Dove X è NULL.
 * ============================================================================
 */
int main() {
    /* --- 1. CREAZIONE DATI (HARDCODED) --- */
    
    /* Portata 0 (Primi) */
    /* Array di stringhe terminato da NULL */
    char *primi[] = {"pasta", "riso", NULL};

    /* Portata 1 (Secondi) */
    char *secondi[] = {"carne", "pesce", "formaggio", NULL};

    /* Portata 2 (Dolci) */
    char *dolci[] = {"gelato", "torta", NULL};

    /*
     * Array principale 'data'.
     * Contiene i puntatori agli array definiti sopra.
     * Nota: data[] è un array di (char**).
     */
    char **menu_completo[] = {primi, secondi, dolci};

    /* Numero di portate */
    int n = 3;

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    /* Passiamo 'menu_completo' che decade a puntatore al primo elemento -> char *** */
    build_menu(menu_completo, n);

    /*
     * Nota sulla memoria nel Main:
     * Qui abbiamo usato stringhe literal ("pasta") e array sullo stack.
     * Non c'è bisogno di free() per i dati del menu in questo specifico main di test.
     * La free() importante è quella dentro build_menu per il buffer temporaneo.
     */

    return 0;
}