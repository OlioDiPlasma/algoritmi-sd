#include <stdlib.h>
#include <stdio.h>

/* =============================================================
 * PROTOTIPI
 * ============================================================= */
int muovi(int mossa, int x, int y, int *dx, int *dy, int **scacc, int N);
void stampa(int **scacc, int N);
int **malloc2d(int N);
void free2d(int **scacc, int N);

/* =============================================================
 * MAIN
 * ============================================================= */
int main(void) {
    int dx[8], dy[8], **scacc;
    int x, y, N;

    // Definizione dei possibili movimenti del cavallo
    dx[0] =  2; dy[0] =  1;
    dx[1] =  1; dy[1] =  2;
    dx[2] = -1; dy[2] =  2;
    dx[3] = -2; dy[3] =  1;
    dx[4] = -2; dy[4] = -1;
    dx[5] = -1; dy[5] = -2;
    dx[6] =  1; dy[6] = -2;
    dx[7] =  2; dy[7] = -1;

    // Legge dimensione della scacchiera
    printf("Inserisci dimensione della scacchiera: ");
    scanf("%d", &N);

    // Allocazione dinamica di una matrice NxN inizializzata a zero
    scacc = malloc2d(N);

    // Posizione iniziale del cavallo
    printf("Inserisci posizione di partenza (riga colonna): ");
    scanf("%d %d", &x, &y);

    scacc[x][y] = 1;  // il cavallo parte dalla prima mossa

    // Avvio della ricerca ricorsiva
    if (muovi(2, x, y, dx, dy, scacc, N) == 1) {
        printf("Percorso completo del cavallo:\n");
        stampa(scacc, N);
    } else {
        printf("Soluzione non trovata\n");
    }

    // Liberazione memoria
    free2d(scacc, N);

    return 0;
}

/* =============================================================
 * muovi(mossa, x, y, dx, dy, scacc, N)
 * -------------------------------------------------------------
 * Funzione ricorsiva per provare a completare il percorso
 * del cavallo.
 *
 * mossa = numero corrente della mossa (1..N*N)
 * (x, y) = posizione corrente del cavallo
 * dx, dy = array dei movimenti del cavallo
 * scacc = matrice NxN che memorizza le mosse
 * N = dimensione della scacchiera
 *
 * Ritorna 1 se trova un percorso completo, 0 altrimenti
 * ============================================================= */
int muovi(int mossa, int x, int y, int *dx, int *dy, int **scacc, int N) {
    int i, newx, newy;

    // Caso base: tutte le caselle visitate → percorso completo
    if (mossa == N*N + 1)
        return 1;

    // Provo tutte e 8 le possibili mosse del cavallo
    for (i = 0; i < 8; i++) {
        newx = x + dx[i];
        newy = y + dy[i];

        // Verifica se la nuova posizione è valida
        if (newx >= 0 && newx < N && newy >= 0 && newy < N) {
            if (scacc[newx][newy] == 0) {  // se non è ancora visitata
                scacc[newx][newy] = mossa;  // piazza la mossa corrente

                // Chiamata ricorsiva alla prossima mossa
                if (muovi(mossa + 1, newx, newy, dx, dy, scacc, N) == 1)
                    return 1;  // soluzione trovata, termina

                // BACKTRACKING: rimuovo la mossa e provo un'altra direzione
                scacc[newx][newy] = 0;
            }
        }
    }

    return 0;  // nessuna mossa valida → torna indietro
}

/* =============================================================
 * stampa(scacc, N)
 * -------------------------------------------------------------
 * Stampa la scacchiera con i numeri delle mosse del cavallo
 * ============================================================= */
void stampa(int **scacc, int N) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
            printf("%2d ", scacc[i][j]);
        printf("\n");
    }
}

/* =============================================================
 * malloc2d(N)
 * -------------------------------------------------------------
 * Alloca dinamicamente una matrice NxN e inizializza a zero
 * ============================================================= */
int **malloc2d(int N) {
    int i;
    int **m = malloc(N * sizeof(int *));
    for (i = 0; i < N; i++) {
        m[i] = calloc(N, sizeof(int));  // inizializzazione a zero
    }
    return m;
}

/* =============================================================
 * free2d(scacc, N)
 * -------------------------------------------------------------
 * Libera la memoria di una matrice NxN allocata dinamicamente
 * ============================================================= */
void free2d(int **scacc, int N) {
    int i;
    for (i = 0; i < N; i++)
        free(scacc[i]);
    free(scacc);
}
