#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Per INT_MAX

/* 
Testo Esercizio: "cambia riga 0, riga 2 e colonna 1" (Totale: 3 mosse).

Tuo Programma: "cambia riga 1, colonna 0, colonna 2" (Totale: 3 mosse).

Sono entrambe corrette! In molti problemi di ottimizzazione esistono più minimi globali.
 Il tuo codice ha esplorato l'albero delle decisioni e ha trovato una delle soluzioni migliori possibili. 
 Siccome 3 è uguale a 3, il programma ha fatto il suo dovere perfettamente.
*/

/*
 * ======================================================================================
 * STRUTTURA DATI DI SUPPORTO
 * ======================================================================================
 * Usiamo una struct per raggruppare i risultati e renderli più facili da gestire
 * tra le chiamate ricorsive. Non è strettamente necessaria, ma è buona norma
 * per mantenere pulita la firma delle funzioni.
 */
typedef struct {
    int *righeDaInvertire;   // Array di boolean (0 o 1): indice i=1 significa "inverti riga i"
    int *colonneDaInvertire; // Array di boolean (0 o 1): indice j=1 significa "inverti colonna j"
    int numMosse;            // Totale inversioni (somma degli 1 nei due array)
} Soluzione;

/*
 * ======================================================================================
 * NOME FUNZIONE: calcolaMosseColonne
 * ======================================================================================
 * SCOPO:
 * Data una configurazione fissa di inversioni sulle righe, verifica se è possibile
 * rendere la matrice tutta bianca agendo sulle colonne e calcola quante mosse servono.
 *
 * PARAMETRI:
 * - mat:       (int **) La matrice originale (sola lettura).
 * - N:         (int) Dimensione matrice.
 * - rigaFlip:  (int *) Array che dice quale riga abbiamo deciso di invertire.
 * - colFlip:   (int *) Array di output dove scriveremo quali colonne vanno invertite.
 *
 * RITORNA:
 * - int: Il numero di colonne da invertire, oppure -1 se la configurazione è impossibile.
 *
 * CONCETTI CHIAVE:
 * L'operatore XOR (^). In C, `a ^ 1` inverte il bit (0 diventa 1, 1 diventa 0).
 * `a ^ 0` lascia il valore invariato.
 * ValoreFinale = ValoreIniziale ^ InversioneRiga ^ InversioneColonna.
 * Noi qui calcoliamo il valore dopo l'inversione riga: Temp = Init ^ Riga.
 * Se Temp è 0, dobbiamo invertire la colonna per avere 1.
 * ======================================================================================
 */
int calcolaMosseColonne(int **mat, int N, int *rigaFlip, int *colFlip) {
    int mosseCol = 0;

    /* --- 1. ITERAZIONE SULLE COLONNE --- */
    for (int j = 0; j < N; j++) {
        
        // Calcoliamo il valore virtuale della prima cella della colonna j
        // dopo aver applicato l'eventuale inversione della riga 0.
        int primoValore = mat[0][j] ^ rigaFlip[0];
        
        // Decidiamo se questa colonna va invertita.
        // Se primoValore è 0 (nero), per diventare bianco (1) DOBBIAMO invertire la colonna.
        // Se primoValore è 1 (bianco), NON DOBBIAMO invertire la colonna.
        int serveInversione = (primoValore == 0) ? 1 : 0;
        
        /* --- 2. VERIFICA COERENZA COLONNA --- */
        // Controlliamo se questa decisione è valida per TUTTE le righe di questa colonna.
        for (int i = 1; i < N; i++) {
            int valoreCella = mat[i][j] ^ rigaFlip[i]; // Valore dopo flip riga
            
            // Se applichiamo 'serveInversione', otteniamo 1?
            if ((valoreCella ^ serveInversione) != 1) {
                // Se arriviamo qui, significa che nella stessa colonna abbiamo
                // celle che richiedono azioni opposte. Configurazione impossibile.
                return -1; 
            }
        }

        // Se siamo qui, la colonna è coerente. Salviamo la decisione.
        colFlip[j] = serveInversione;
        mosseCol += serveInversione;
    }

    return mosseCol;
}

/*
 * ======================================================================================
 * NOME FUNZIONE: backtracking
 * ======================================================================================
 * SCOPO:
 * Esplora ricorsivamente tutte le combinazioni di inversione delle righe.
 * È il cuore del processo decisionale.
 *
 * PARAMETRI:
 * - mat:           Matrice input.
 * - N:             Dimensione.
 * - rigaCorrente:  Indice della riga che stiamo decidendo se invertire o no.
 * - currentRFlip:  Array stato attuale inversioni righe.
 * - currentMoves:  Numero inversioni righe fatte finora.
 * - bestSol:       Puntatore alla struttura che contiene la miglior soluzione trovata finora.
 *
 * CONCETTI CHIAVE:
 * Ricorsione e Backtracking. Proviamo una strada, scendiamo in profondità,
 * poi torniamo su annullando la modifica per provare l'altra strada.
 * ======================================================================================
 */
void backtracking(int **mat, int N, int rigaCorrente, int *currentRFlip, int currentMoves, Soluzione *bestSol) {
    
    /* --- 1. CASO BASE: DECISIONI PRESE PER TUTTE LE RIGHE --- */
    if (rigaCorrente == N) {
        // Abbiamo deciso per tutte le righe. Ora verifichiamo le colonne.
        
        // Alloco un array temporaneo per le colonne (nello stack o heap, qui heap per coerenza didattica)
        int *tempCFlip = (int *)malloc(N * sizeof(int));
        if (tempCFlip == NULL) exit(1);

        int mosseCol = calcolaMosseColonne(mat, N, currentRFlip, tempCFlip);

        // Se la configurazione è valida (mosseCol != -1) E il totale è migliore del record attuale
        if (mosseCol != -1) {
            int totaleMosse = currentMoves + mosseCol;
            
            if (totaleMosse < bestSol->numMosse) {
                // Abbiamo trovato un nuovo minimo! Aggiorniamo la soluzione migliore.
                bestSol->numMosse = totaleMosse;
                // Copiamo gli array nella struttura 'bestSol'
                for(int i=0; i<N; i++) {
                    bestSol->righeDaInvertire[i] = currentRFlip[i];
                    bestSol->colonneDaInvertire[i] = tempCFlip[i];
                }
            }
        }

        free(tempCFlip); // Importante: evitare memory leak nel caso base
        return;
    }

    /* --- 2. RAMO A: NON INVERTIAMO LA RIGA CORRENTE --- */
    currentRFlip[rigaCorrente] = 0;
    backtracking(mat, N, rigaCorrente + 1, currentRFlip, currentMoves, bestSol);

    /* --- 3. RAMO B: INVERTIAMO LA RIGA CORRENTE --- */
    currentRFlip[rigaCorrente] = 1;
    // Incrementiamo currentMoves di 1 perché abbiamo fatto un'inversione
    backtracking(mat, N, rigaCorrente + 1, currentRFlip, currentMoves + 1, bestSol);
    
    // Non serve "annullare" esplicitamente currentRFlip qui perché verrà 
    // sovrascritta alla prossima chiamata o iterazione, ma concettualmente 
    // il backtracking finisce qui per questo nodo.
}

/*
 * ======================================================================================
 * NOME FUNZIONE: trovaMinimiComandi
 * ======================================================================================
 * SCOPO:
 * Funzione Wrapper (involucro). Prepara la memoria e lancia la ricorsione.
 * Stampa a video i comandi risultanti.
 *
 * PARAMETRI:
 * - mat: (int **) Matrice quadrata da risolvere.
 * - N:   (int) Dimensione.
 *
 * GESTIONE MEMORIA:
 * Alloca gli array di supporto per la ricorsione e per la soluzione migliore.
 * Li libera alla fine.
 * ======================================================================================
 */
void trovaMinimiComandi(int **mat, int N) {
    /* --- 1. ALLOCAZIONE STRUTTURE DATI --- */
    Soluzione bestSol;
    bestSol.numMosse = INT_MAX; // Inizializziamo a "infinito"
    bestSol.righeDaInvertire = (int *)calloc(N, sizeof(int));
    bestSol.colonneDaInvertire = (int *)calloc(N, sizeof(int));

    int *currentRFlip = (int *)calloc(N, sizeof(int));

    // Controllo allocazioni
    if (!bestSol.righeDaInvertire || !bestSol.colonneDaInvertire || !currentRFlip) {
        printf("Errore malloc\n");
        exit(1);
    }

    /* --- 2. AVVIO LOGICA --- */
    backtracking(mat, N, 0, currentRFlip, 0, &bestSol);

    /* --- 3. STAMPA RISULTATI --- */
    if (bestSol.numMosse == INT_MAX) {
        printf("Impossibile rendere la matrice tutta bianca.\n");
    } else {
        printf("Soluzione Minima trovata in %d comandi:\n", bestSol.numMosse);
        
        // Stampa Righe
        for (int i = 0; i < N; i++) {
            if (bestSol.righeDaInvertire[i]) {
                printf("- Cambia riga %d\n", i);
            }
        }
        // Stampa Colonne
        for (int j = 0; j < N; j++) {
            if (bestSol.colonneDaInvertire[j]) {
                printf("- Cambia colonna %d\n", j);
            }
        }
    }

    /* --- 4. PULIZIA MEMORIA --- */
    free(bestSol.righeDaInvertire);
    free(bestSol.colonneDaInvertire);
    free(currentRFlip);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Scopo: Creare la matrice dell'esempio e testare la funzione.
 * ======================================================================================
 */
int main() {
    int N = 3;

    /* --- 1. ALLOCAZIONE MATRICE (Puntatori doppi) --- */
    // Spiegazione: int **mat è un puntatore a un array di puntatori (le righe).
    int **matrice = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        matrice[i] = (int *)malloc(N * sizeof(int));
    }

    /* --- 2. POPOLAMENTO DATI (Dall'immagine dell'esercizio) --- */
    // Esempio:
    // 0 1 0
    // 1 0 1
    // 0 1 0
    int dati[3][3] = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 0}
    };

    printf("--- Matrice Iniziale ---\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrice[i][j] = dati[i][j];
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }
    printf("------------------------\n");

    /* --- 3. CHIAMATA FUNZIONE --- */
    trovaMinimiComandi(matrice, N);

    /* --- 4. LIBERAZIONE MEMORIA MATRICE --- */
    for (int i = 0; i < N; i++) {
        free(matrice[i]); // Libero ogni riga
    }
    free(matrice); // Libero l'array di righe

    return 0;
}