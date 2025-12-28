#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * FUNZIONE OBIETTIVO: f
 * ============================================================================
 * Scopo:
 * Sovrascrive una porzione della matrice m1 con i valori della matrice m2.
 * L'operazione parte dalle coordinate (x, y) su m1.
 * La funzione gestisce il "clipping": copia solo le celle di m2 che cadono
 * effettivamente all'interno dell'area valida di m1.
 *
 * Parametri:
 * - m1: Doppio puntatore (matrice destinazione).
 * - r1, c1: Dimensioni (righe, colonne) di m1.
 * - m2: Doppio puntatore (matrice sorgente).
 * - r2, c2: Dimensioni (righe, colonne) di m2.
 * - x: Indice di riga iniziale in m1 (offset verticale).
 * - y: Indice di colonna iniziale in m1 (offset orizzontale).
 *
 * Concetti Chiave:
 * - Interpretazione coordinate: Dal diagramma, 'x' è sull'asse verticale (righe)
 * e 'y' sull'asse orizzontale (colonne). Quindi accessi come m1[x][y].
 * - Boundary Check: Non possiamo ciecamente copiare m2 in m1[x+i][y+j].
 * Dobbiamo verificare ad ogni passo di non sbordare da r1 o c1.
 * ============================================================================
 */
void f(int **m1, int r1, int c1, int **m2, int r2, int c2, int x, int y) {

    /* --- 1. ITERAZIONE SULLA MATRICE SORGENTE (M2) --- */
    // Scorriamo tutta la matrice che dobbiamo copiare (m2)
    for (int i = 0; i < r2; i++) {
        for (int j = 0; j < c2; j++) {

            /* --- 2. CALCOLO COORDINATE DI DESTINAZIONE --- */
            // Calcoliamo dove cadrebbe questo valore nella matrice m1.
            // i e j sono gli scostamenti rispetto al punto di ancoraggio (x, y).
            int riga_dest = x + i;
            int col_dest = y + j;

            /* --- 3. CONTROLLO LIMITI (CLIPPING) --- */
            // Dettaglio Sintattico:
            // Verifichiamo 4 condizioni fondamentali collegate da AND logico (&&):
            // 1. riga_dest >= 0: Gestisce il caso in cui x sia negativo.
            // 2. riga_dest < r1: Assicura di non scrivere oltre l'ultima riga di m1.
            // 3. col_dest >= 0: Gestisce il caso in cui y sia negativo.
            // 4. col_dest < c1: Assicura di non scrivere oltre l'ultima colonna di m1.
            if (riga_dest >= 0 && riga_dest < r1 && 
                col_dest >= 0 && col_dest < c1) {

                /* --- 4. ASSEGNAZIONE --- */
                // Dettaglio Sintattico:
                // m1[riga_dest]: Accede al puntatore della riga specifica.
                // [col_dest]: Accede all'intero specifico in quella riga.
                // Stiamo fisicamente sovrascrivendo la memoria di m1 con il valore di m2.
                m1[riga_dest][col_dest] = m2[i][j];
            }
            // Se l'if è falso, siamo "fuori dai bordi" e ignoriamo semplicemente la copia.
        }
    }
}

/* ============================================================================
 * FUNZIONE AUSILIARIA: alloc_matrix
 * ============================================================================
 * Scopo:
 * Alloca dinamicamente una matrice di interi rxc.
 *
 * Parametri:
 * - r: numero di righe.
 * - c: numero di colonne.
 *
 * Ritorna:
 * - Puntatore alla matrice allocata (int**).
 *
 * Gestione Memoria:
 * Utilizziamo due malloc. Una per il vettore dei puntatori alle righe
 * e, per ogni riga, una malloc per gli interi.
 * ============================================================================
 */
int **alloc_matrix(int r, int c) {
    // 1. Allocazione dell'array di puntatori (le righe)
    // sizeof(int*) perché stiamo allocando spazio per indirizzi di memoria.
    int **m = (int **)malloc(r * sizeof(int *));
    if (m == NULL) {
        printf("Errore malloc righe\n");
        exit(1);
    }

    // 2. Allocazione delle colonne per ogni riga
    for (int i = 0; i < r; i++) {
        // sizeof(int) perché qui stiamo allocando spazio per i dati veri (numeri).
        m[i] = (int *)malloc(c * sizeof(int));
        if (m[i] == NULL) {
            printf("Errore malloc colonne\n");
            exit(1);
        }
    }
    return m;
}

/* ============================================================================
 * FUNZIONE AUSILIARIA: free_matrix
 * ============================================================================
 * Scopo:
 * Libera correttamente la memoria per evitare Memory Leaks.
 *
 * Concetti Chiave:
 * L'ordine è inverso rispetto all'allocazione: prima libero i "figli" (le righe),
 * poi libero il "padre" (il vettore di puntatori).
 * ============================================================================
 */
void free_matrix(int **m, int r) {
    for (int i = 0; i < r; i++) {
        free(m[i]); // Libera l'array di interi della riga i
    }
    free(m); // Libera l'array di puntatori
}

/* ============================================================================
 * FUNZIONE AUSILIARIA: print_matrix
 * ============================================================================
 * Scopo:
 * Stampa a video la matrice formattata.
 * ============================================================================
 */
void print_matrix(int **m, int r, int c, char *nome) {
    printf("Matrice %s (%dx%d):\n", nome, r, c);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            printf("%3d ", m[i][j]); // %3d per allineare i numeri
        }
        printf("\n");
    }
    printf("\n");
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo:
 * Verifica il funzionamento della funzione f con dati concreti.
 * Crea una situazione di sovrapposizione parziale per testare i limiti.
 * ============================================================================
 */
int main() {
    /* --- 1. SETUP DATI DI PROVA --- */
    int r1 = 5, c1 = 5;
    int r2 = 3, c2 = 3;
    
    // Allocazione matrici
    int **M1 = alloc_matrix(r1, c1);
    int **M2 = alloc_matrix(r2, c2);

    /* --- 2. INIZIALIZZAZIONE --- */
    // Riempiamo M1 con 0
    for(int i=0; i<r1; i++)
        for(int j=0; j<c1; j++)
            M1[i][j] = 0;

    // Riempiamo M2 con 9 (per vedere bene la sovrascrittura)
    int count = 1;
    for(int i=0; i<r2; i++)
        for(int j=0; j<c2; j++)
            M2[i][j] = 9; // Uso 9 per evidenziare le celle copiate

    printf("--- STATO INIZIALE ---\n");
    print_matrix(M1, r1, c1, "M1");
    print_matrix(M2, r2, c2, "M2");

    /* --- 3. TEST LOGICA --- */
    // Posizioniamo M2 in modo che esca dai bordi di M1.
    // Mettiamo x=3, y=3.
    // M2 è 3x3.
    // Le righe occupate sarebbero: 3, 4, 5. Ma M1 finisce a indice 4. (La riga 5 è fuori)
    // Le colonne occupate sarebbero: 3, 4, 5. Ma M1 finisce a indice 4. (La colonna 5 è fuori)
    int x = 3;
    int y = 3;
    
    printf("--- ESECUZIONE f(M1, ..., M2, ..., x=%d, y=%d) ---\n", x, y);
    f(M1, r1, c1, M2, r2, c2, x, y);

    /* --- 4. VERIFICA RISULTATI --- */
    // Ci aspettiamo che solo un quadrato 2x2 di '9' sia visibile nell'angolo in basso a destra di M1.
    print_matrix(M1, r1, c1, "M1 Dopo Sovrascrittura");

    /* --- 5. PULIZIA DELLA MEMORIA --- */
    // Fondamentale in C: chi sporca, pulisce.
    free_matrix(M1, r1);
    free_matrix(M2, r2);

    return 0;
}