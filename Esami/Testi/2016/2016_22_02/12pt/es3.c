#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * FUNZIONE AUSILIARIA: controllaLampadine
 * ============================================================================
 * SCOPO:
 * Verifica se, data una specifica selezione di interruttori (rappresentata dal
 * vettore 'scelte'), TUTTE le lampadine risultano accese.
 *
 * PARAMETRI:
 * - mat:    La matrice n x m delle connessioni (interruttore -> lampadine).
 * - n:      Numero di interruttori (righe).
 * - m:      Numero di lampadine (colonne).
 * - scelte: Vettore di interi contenente gli INDICI degli interruttori premuti.
 * - k:      Numero di interruttori premuti (dimensione logica di 'scelte').
 *
 * CONCETTI CHIAVE (LOGICA INTERRUTTORI):
 * Il testo dice: "una lampadina è accesa se e solo se è dispari il numero
 * di interruttori premuti che la controllano".
 * Questo è equivalente all'operazione XOR.
 * Algoritmo:
 * 1. Per ogni lampadina (colonna j), contiamo quanti interruttori TRA QUELLI SCELTI
 * hanno un 1 in quella colonna.
 * 2. Se il conteggio è DISPARI, la lampadina è ON. Se PARI, è OFF.
 * 3. Se anche una sola lampadina è OFF, la configurazione fallisce.
 * ============================================================================
 */
int controllaLampadine(int **mat, int n, int m, int *scelte, int k) {
    
    /* Iteriamo su ogni lampadina (colonna) */
    for (int j = 0; j < m; j++) {
        int accensioni = 0;

        /* Iteriamo solo sugli interruttori che abbiamo deciso di premere */
        for (int i = 0; i < k; i++) {
            int interruttore_idx = scelte[i]; // Indice dell'interruttore
            
            /* Accesso alla matrice: riga dell'interruttore, colonna della lampadina */
            if (mat[interruttore_idx][j] == 1) {
                accensioni++;
            }
        }

        /* Se il numero di attivazioni è PARI (accensioni % 2 == 0),
           la lampadina è rimasta SPENTA. Ritorno 0 (Falso). */
        if (accensioni % 2 == 0) {
            return 0; 
        }
    }

    /* Se siamo arrivati qui, tutte le lampadine sono ON. */
    return 1;
}

/* ============================================================================
 * FUNZIONE RICORSIVA (KERNEL): combinazioni
 * ============================================================================
 * SCOPO:
 * Genera le combinazioni di 'k' interruttori e verifica se accendono tutto.
 *
 * PARAMETRI:
 * - mat, n, m: Dimensioni e dati del problema.
 * - k:         Numero di interruttori che stiamo provando a premere in questo tentativo.
 * - start:     Indice da cui partire (per evitare duplicati, es. {0,1} vs {1,0}).
 * - scelte:    Vettore dove memorizziamo gli indici correnti.
 * - count:     Quanti interruttori abbiamo scelto finora nella ricorsione.
 *
 * CONCETTI CHIAVE (BACKTRACKING):
 * Usiamo la ricorsione per generare combinazioni semplici (senza ripetizioni, ordine ininfluente).
 * Se arriviamo a selezionarne 'k', chiamiamo la funzione di controllo.
 * ============================================================================
 */
int combinazioni(int **mat, int n, int m, int k, int start, int *scelte, int count) {
    
    /* --- 1. CASO BASE --- */
    if (count == k) {
        /* Abbiamo selezionato k interruttori. Verifichiamo se funzionano. */
        if (controllaLampadine(mat, n, m, scelte, k)) {
            return 1; // TROVATO! Questa combinazione funziona.
        }
        return 0; // Questa combinazione non funziona, backtrack.
    }

    /* --- 2. PASSO RICORSIVO --- */
    /* Ciclo sugli interruttori disponibili da 'start' a 'n-1' */
    for (int i = start; i < n; i++) {
        
        /* DO: Scegliamo l'interruttore i */
        scelte[count] = i;

        /* RECURSE: Cerchiamo il prossimo (count + 1) partendo da (i + 1) */
        if (combinazioni(mat, n, m, k, i + 1, scelte, count + 1)) {
            return 1; // Se la chiamata ricorsiva ha trovato una soluzione, risaliamo subito (Pruning).
        }
        
        /* UNDO: Non serve codice esplicito qui, sovrascriveremo scelte[count] 
           alla prossima iterazione. */
    }

    return 0; // Nessuna soluzione trovata in questo ramo.
}

/* ============================================================================
 * FUNZIONE WRAPPER: trovaMinimoInterruttori
 * ============================================================================
 * SCOPO:
 * Poiché cerchiamo il numero MINIMO di interruttori, invece di provare tutte le
 * combinazioni possibili a caso, proviamo prima con k=1, poi k=2, poi k=3...
 * La prima soluzione che troviamo sarà garantita essere la minima.
 *
 * GESTIONE MEMORIA:
 * Allochiamo il vettore 'scelte' dinamicamente.
 * ============================================================================
 */
void trovaMinimoInterruttori(int **mat, int n, int m) {
    
    /* Allochiamo un vettore temporaneo per le soluzioni. 
       Al massimo useremo n interruttori. */
    int *scelte = (int *)malloc(n * sizeof(int));
    if (scelte == NULL) {
        printf("Errore malloc\n");
        return;
    }

    int trovato = 0;

    /* --- LOGICA DI RICERCA ITERATIVA --- */
    /* Proviamo dimensioni crescenti k da 1 a n */
    for (int k = 1; k <= n; k++) {
        
        /* Chiamiamo il motore ricorsivo per cercare combinazioni di dimensione k */
        if (combinazioni(mat, n, m, k, 0, scelte, 0)) {
            
            /* --- STAMPA RISULTATO --- */
            printf("Soluzione minima trovata con %d interruttori: ", k);
            for (int i = 0; i < k; i++) {
                printf("%d ", scelte[i]);
            }
            printf("\n");
            
            trovato = 1;
            break; // Usciamo dal ciclo appena troviamo la minima!
        }
    }

    if (!trovato) {
        printf("Nessuna combinazione di interruttori accende tutte le lampadine.\n");
    }

    /* --- PULIZIA MEMORIA --- */
    free(scelte);
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * SCOPO:
 * Ricrea esattamente l'esempio dell'immagine (n=4, m=5) e testa la funzione.
 * ============================================================================
 */
int main() {
    int n = 4; // Interruttori
    int m = 5; // Lampadine

    /* --- 1. ALLOCAZIONE MATRICE DINAMICA --- */
    /* Allocazione vettore di puntatori (righe) */
    int **mat = (int **)malloc(n * sizeof(int *));
    
    /* Allocazione righe */
    for (int i = 0; i < n; i++) {
        mat[i] = (int *)malloc(m * sizeof(int));
    }

    /* --- 2. INIZIALIZZAZIONE DATI (Hardcoded dall'esempio) --- */
    /* R0: 1 1 0 0 1
       R1: 1 0 1 0 0
       R2: 0 1 1 1 0
       R3: 1 0 0 1 0
    */
    // Interruttore 0
    mat[0][0]=1; mat[0][1]=1; mat[0][2]=0; mat[0][3]=0; mat[0][4]=1;
    // Interruttore 1
    mat[1][0]=1; mat[1][1]=0; mat[1][2]=1; mat[1][3]=0; mat[1][4]=0;
    // Interruttore 2
    mat[2][0]=0; mat[2][1]=1; mat[2][2]=1; mat[2][3]=1; mat[2][4]=0;
    // Interruttore 3
    mat[3][0]=1; mat[3][1]=0; mat[3][2]=0; mat[3][3]=1; mat[3][4]=0;

    printf("--- INIZIO TEST: Interruttori e Lampadine ---\n");
    
    /* Chiamata alla funzione principale */
    trovaMinimoInterruttori(mat, n, m);

    printf("--- FINE TEST ---\n");
    printf("Risultato atteso: 3 interruttori (0, 1, 3)\n");

    /* --- 3. DEALLOCAZIONE MEMORIA --- */
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);

    return 0;
}