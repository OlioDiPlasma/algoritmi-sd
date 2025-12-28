#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * ======================================================================================
 * DEFINIZIONE STRUTTURA: Point
 * ======================================================================================
 * Scopo:
 * Rappresentare una singola coordinata (riga, colonna) all'interno della griglia.
 * Ci serve per memorizzare le caselle nella nostra "coda" di elaborazione.
 */
typedef struct {
    int r; // Indice di riga
    int c; // Indice di colonna
} Point;

/*
 * ======================================================================================
 * FUNZIONE: minPath
 * ======================================================================================
 * Scopo:
 * Calcola la lunghezza del percorso minimo tra due punti (start e end) in una griglia
 * rappresentata da una matrice di caratteri, dove '0' è libero e '1' è muro.
 * Utilizza l'algoritmo BFS (Breadth-First Search).
 *
 * Parametri:
 * - char **area: Puntatore doppio che rappresenta la matrice (griglia) NxM.
 * - int N: Numero totale di righe della griglia.
 * - int M: Numero totale di colonne della griglia.
 * - int r0, c0: Coordinate della casella di partenza (riga, colonna).
 * - int r1, c1: Coordinate della casella di arrivo (riga, colonna).
 *
 * Concetti Chiave:
 * - BFS (Ricerca in Ampiezza): È l'algoritmo standard per trovare il percorso più breve
 * in un grafo non pesato (come una griglia). Esplora i vicini "a cipolla", livello per livello.
 * - Matrice delle Distanze: Useremo una matrice parallela di interi (int **dist) per
 * tenere traccia di due cose:
 * 1. Se una cella è stata visitata (se valore > 0).
 * 2. Quanti passi servono per arrivarci (il valore stesso).
 * - Coda (Queue): La BFS necessita di una struttura FIFO (First In, First Out). Useremo
 * un array dinamico come coda semplice.
 *
 * Ritorna:
 * - int: La lunghezza del percorso (numero di caselle). Ritorna -1 se non esiste un percorso.
 */
int minPath(char **area, int N, int M, int r0, int c0, int r1, int c1) {

    /* --- 1. VALIDAZIONE INIZIALE --- */
    // Controllo se partenza o arrivo sono fuori dai bordi o sono muri ('1')
    // Nota sintattica: area[r][c] accede al valore char nella matrice.
    if (r0 < 0 || r0 >= N || c0 < 0 || c0 >= M || area[r0][c0] == '1') return -1;
    if (r1 < 0 || r1 >= N || c1 < 0 || c1 >= M || area[r1][c1] == '1') return -1;

    // Caso base: partenza e arrivo coincidono.
    // Il problema dice che la lunghezza è il numero di caselle, quindi 1 casella.
    if (r0 == r1 && c0 == c1) return 1;

    /* --- 2. ALLOCAZIONE MATRICE DISTANZE (MEMORY MANAGEMENT CRITICO) --- */
    // Dobbiamo creare una matrice di interi grande quanto 'area' per salvare i passi.
    // Usiamo 'calloc' invece di 'malloc' perché inizializza tutto a 0.
    // 0 significherà "non visitato".

    // Allocazione dell'array di puntatori alle righe
    int **dist = (int **)calloc(N, sizeof(int *));
    if (dist == NULL) {
        perror("Errore malloc righe"); // Stampa errore se la RAM è piena
        exit(1);
    }

    // Allocazione di ogni singola riga
    for (int i = 0; i < N; i++) {
        dist[i] = (int *)calloc(M, sizeof(int));
        // Controllo paranoico dell'allocazione
        if (dist[i] == NULL) {
            // Se fallisce qui, dovremmo liberare tutto ciò che abbiamo allocato prima
            // Per semplicità didattica, usciamo, ma in produzione si fa cleanup.
            exit(1);
        }
    }

    /* --- 3. ALLOCAZIONE CODA BFS --- */
    // La dimensione massima della coda è pari a tutte le celle della matrice (N*M).
    Point *queue = (Point *)malloc(sizeof(Point) * (N * M));
    int head = 0; // Indice di lettura (estrazione)
    int tail = 0; // Indice di scrittura (inserimento)

    /* --- 4. INIZIALIZZAZIONE BFS --- */
    // Inseriamo il punto di partenza nella coda
    queue[tail].r = r0;
    queue[tail].c = c0;
    tail++; // Incrementiamo tail dopo l'inserimento

    // Impostiamo la distanza iniziale.
    // Il testo dice che la lunghezza è il numero di caselle.
    // Quindi partire da (r0,c0) conta come 1 passo.
    dist[r0][c0] = 1;

    // Array di supporto per esplorare le 4 direzioni (Su, Giù, Sinistra, Destra)
    // dr = delta righe, dc = delta colonne
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    int result = -1; // Valore di default se non troviamo il percorso

    /* --- 5. CICLO PRINCIPALE (LOGICA CORE) --- */
    // Continuiamo finché la coda non è vuota (head < tail)
    while (head < tail) {
        // A. DEQUEUE (Estrazione)
        // Leggiamo l'elemento corrente e avanziamo l'indice head
        Point current = queue[head];
        head++;

        // B. CHECK ARRIVO
        if (current.r == r1 && current.c == c1) {
            result = dist[current.r][current.c];
            break; // Trovato! Usciamo dal ciclo while
        }

        // C. ESPLORAZIONE VICINI
        for (int i = 0; i < 4; i++) {
            // Calcoliamo le coordinate del vicino potenziale
            int nr = current.r + dr[i];
            int nc = current.c + dc[i];

            // Verifichiamo se il vicino è valido:
            // 1. È dentro i bordi (0 <= nr < N ...)
            // 2. Non è un muro (area[nr][nc] == '0')
            // 3. Non è stato ancora visitato (dist[nr][nc] == 0)
            if (nr >= 0 && nr < N && nc >= 0 && nc < M &&
                area[nr][nc] == '0' && dist[nr][nc] == 0) {

                // Aggiorniamo la distanza: passo precedente + 1
                dist[nr][nc] = dist[current.r][current.c] + 1;

                // ENQUEUE (Inserimento)
                queue[tail].r = nr;
                queue[tail].c = nc;
                tail++;
            }
        }
    }

    /* --- 6. GESTIONE MEMORIA (CLEANUP) --- */
    // È FONDAMENTALE liberare la memoria per evitare Memory Leaks.
    // L'ordine è inverso rispetto all'allocazione.

    // 1. Liberiamo l'array della coda
    free(queue);

    // 2. Liberiamo ogni riga della matrice distanze
    for (int i = 0; i < N; i++) {
        free(dist[i]);
    }

    // 3. Liberiamo l'array di puntatori alle righe
    free(dist);

    return result;
}

/*
 * ======================================================================================
 * FUNZIONE: main (TEST HARNESS)
 * ======================================================================================
 * Scopo:
 * Creare un ambiente di prova controllato per verificare che la funzione minPath
 * funzioni correttamente con i dati dell'esempio.
 */
int main() {
    // Definizione delle dimensioni basata sull'esempio (4 righe, 5 colonne)
    int N = 4;
    int M = 5;

    /* --- 1. CREAZIONE DATI DI PROVA --- */
    // Simuliamo la matrice dell'esempio.
    // Dobbiamo usare allocazione dinamica per simulare esattamente
    // come verrebbe passata la matrice in un contesto reale (char **).

    char **area = (char **)malloc(N * sizeof(char *));
    for (int i = 0; i < N; i++) {
        area[i] = (char *)malloc(M * sizeof(char));
    }

    // Popoliamo la matrice con i dati dell'immagine
    // Riga 0: 01000
    char row0[] = {'0', '1', '0', '0', '0'};
    // Riga 1: 00010 (notare: nell'esempio c'è il percorso 'x', noi mettiamo '0' liberi)
    char row1[] = {'0', '0', '0', '1', '0'};
    // Riga 2: 01000
    char row2[] = {'0', '1', '0', '0', '0'};
    // Riga 3: 00000
    char row3[] = {'0', '0', '0', '0', '0'};

    // Copiamo i valori nella matrice dinamica
    for(int j=0; j<M; j++) {
        area[0][j] = row0[j];
        area[1][j] = row1[j];
        area[2][j] = row2[j];
        area[3][j] = row3[j];
    }

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    // Coordinate dell'esempio: Start(1,0) -> End(2,3)
    // Il percorso atteso è lungo 5 caselle.
    printf("Calcolo percorso da (1,0) a (2,3)...\n");
    int lunghezza = minPath(area, N, M, 1, 0, 2, 3);

    /* --- 3. STAMPA RISULTATI --- */
    printf("Lunghezza percorso minimo: %d\n", lunghezza);
    if (lunghezza == 5) {
        printf("TEST SUPERATO: Il risultato corrisponde all'esempio.\n");
    } else {
        printf("TEST FALLITO: Atteso 5, ottenuto %d.\n", lunghezza);
    }

    /* --- 4. CLEANUP FINALE --- */
    // Liberiamo la matrice 'area' creata nel main
    for (int i = 0; i < N; i++) {
        free(area[i]);
    }
    free(area);

    return 0;
}