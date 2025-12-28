#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * FUNZIONE: matMax
 * ============================================================================
 * SCOPO:
 * Scansiona una matrice e visualizza le coordinate (riga, colonna) di tutti
 * gli elementi che sono "massimi locali". Un elemento è un massimo locale se
 * il suo valore è STRETTAMENTE MAGGIORE di tutti gli elementi adiacenti
 * (inclusi i diagonali).
 *
 * PARAMETRI:
 * - m:   int ** (Doppio puntatore)
 * La matrice di interi da analizzare.
 * - r:   int
 * Numero di righe della matrice.
 * - c:   int
 * Numero di colonne della matrice.
 *
 * CONCETTI CHIAVE:
 * 1. Vicinato a 8 connessioni: Per ogni cella [i][j], dobbiamo controllare i
 * vicini con offset da -1 a +1 sia su riga che su colonna.
 * 2. Controllo dei Limiti (Bounds Checking): Prima di accedere a un vicino,
 * dobbiamo verificare che le sue coordinate siano valide (>= 0 e < dimensione).
 * Altrimenti rischiamo un Segmentation Fault.
 * ============================================================================
 */
void matMax(int **m, int r, int c) {

    /* --- 1. SCANSIONE DELLA MATRICE --- */
    /* Iteriamo su ogni cella della matrice per valutarla come candidato massimo. */
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {

            int valoreCorrente = m[i][j];
            int isLocalMax = 1; // Flag: assumiamo sia un massimo finché non proviamo il contrario

            /* --- 2. CONTROLLO DEI VICINI --- */
            /* Per evitare di scrivere 8 if separati, usiamo due cicli annidati
               che generano gli offset (spostamenti) relativi alla cella corrente.
               dr = delta riga, dc = delta colonna. Vanno da -1 a +1. */
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {

                    /* Caso base: saltiamo la cella stessa (offset 0,0) */
                    if (dr == 0 && dc == 0) continue;

                    /* Calcoliamo le coordinate reali del vicino */
                    int ni = i + dr; // Neighbor i
                    int nj = j + dc; // Neighbor j

                    /* --- 3. VERIFICA VALIDITÀ INDICI (Safety Check) --- */
                    /* Controlliamo se il vicino esiste o se siamo fuori dai bordi. */
                    if (ni >= 0 && ni < r && nj >= 0 && nj < c) {

                        /* --- 4. CONFRONTO VALORI --- */
                        /* Dettaglio Sintattico:
                           Accediamo a m[ni][nj]. Se il vicino è maggiore o UGUALE
                           al nostro valore corrente, allora il nostro valore NON è
                           strettamente maggiore di TUTTI i vicini. */
                        if (m[ni][nj] >= valoreCorrente) {
                            isLocalMax = 0; // Il candidato ha fallito
                        }
                    }
                }
                /* Ottimizzazione: Se abbiamo già scoperto che non è un massimo,
                   inutile controllare gli altri vicini. Usciamo dai cicli interni. */
                if (isLocalMax == 0) break;
            }

            /* --- 5. VISUALIZZAZIONE RISULTATO --- */
            /* Se il flag è rimasto 1, significa che ha vinto tutti i confronti. */
            if (isLocalMax == 1) {
                printf("(%d, %d) ", i, j);
            }
        }
    }
    printf("\n"); // A capo finale per pulizia
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * SCOPO:
 * Creare la matrice dell'esempio fornito nel testo e verificare il funzionamento.
 * ============================================================================
 */
int main() {
    /* Parametri dell'esempio */
    int R = 3;
    int C = 4;

    printf("--- INIZIO TEST: Ricerca Massimi Locali ---\n");

    /* --- 1. ALLOCAZIONE DINAMICA (Gestione Memoria) --- */
    /* Spiegazione:
       La matrice è int**.
       1. Allochiamo un array di R puntatori (le righe).
       2. Per ogni puntatore, allochiamo un array di C interi (le colonne). */
    
    int **matrice = (int **)malloc(R * sizeof(int *));
    if (matrice == NULL) {
        perror("Errore malloc righe");
        return -1;
    }

    for (int i = 0; i < R; i++) {
        matrice[i] = (int *)malloc(C * sizeof(int));
        if (matrice[i] == NULL) {
            perror("Errore malloc colonne");
            return -1; // Qui bisognerebbe liberare le righe precedenti
        }
    }

    /* --- 2. INIZIALIZZAZIONE DATI (Hardcoded dall'esempio) --- */
    /*
       Matrice:
       5 2 3 1
       4 1 6 4
       3 0 5 2
    */
    // Riga 0
    matrice[0][0] = 5; matrice[0][1] = 2; matrice[0][2] = 3; matrice[0][3] = 1;
    // Riga 1
    matrice[1][0] = 4; matrice[1][1] = 1; matrice[1][2] = 6; matrice[1][3] = 4;
    // Riga 2
    matrice[2][0] = 3; matrice[2][1] = 0; matrice[2][2] = 5; matrice[2][3] = 2;

    /* Stampa di controllo */
    printf("Matrice Input:\n");
    for(int i=0; i<R; i++) {
        for(int j=0; j<C; j++) {
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }

    /* --- 3. ESECUZIONE --- */
    printf("\nRisultato atteso: (0, 0) e (1, 2)\n");
    printf("Risultato ottenuto: ");
    matMax(matrice, R, C);

    /* --- 4. DEALLOCAZIONE (Free) --- */
    /* Regola: Liberare in ordine inverso all'allocazione.
       Prima il contenuto delle righe, poi il contenitore delle righe. */
    for (int i = 0; i < R; i++) {
        free(matrice[i]); // Libera l'array di interi
    }
    free(matrice); // Libera l'array di puntatori
    
    printf("\nMemoria liberata.\n");
    return 0;
}