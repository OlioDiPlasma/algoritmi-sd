/* ==========================================
   SEZIONE 1: INCLUSIONI E STRUTTURE
   ========================================== */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* * Struttura di supporto per la Coda della BFS.
 * Memorizza le coordinate (r, c) di una cella e la
 * distanza (dist) accumulata per arrivarci.
 */
typedef struct {
    int r;      // Coordinata riga
    int c;      // Coordinata colonna
    int dist;   // Lunghezza del percorso fin qui
} Node;

/* ==========================================
   SEZIONE 2: LA FUNZIONE RICHIESTA
   ========================================== */

/*
 * Scopo: Calcola la lunghezza del percorso minimo tra (r0,c0) e (r1,c1)
 * muovendosi solo su caselle '0' (libere) in orizzontale/verticale.
 *
 * Parametri:
 * - char **area: La matrice NxM (array di puntatori).
 * - int N, M: Dimensioni della matrice (Righe, Colonne).
 * - int r0, c0: Coordinate di partenza.
 * - int r1, c1: Coordinate di arrivo.
 *
 * Concetti Chiave:
 * 1. BFS (Breadth-First Search): Usiamo una coda per esplorare i vicini livello per livello.
 * 2. Matrice Visited: Fondamentale per non girare in tondo. Usiamo un array linearizzato
 * per semplicità di allocazione (visited[r * M + c]).
 * 3. Validazione Coordinate: Controlliamo sempre di non uscire dai bordi (0 <= r < N).
 */
int minPath(char **area, int N, int M, int r0, int c0, int r1, int c1) {
    
    /* --- 1. CONTROLLI INIZIALI --- */
    // Se le coordinate sono fuori dai bordi o le caselle di partenza/arrivo sono occupate ('1')
    if (r0 < 0 || r0 >= N || c0 < 0 || c0 >= M || 
        r1 < 0 || r1 >= N || c1 < 0 || c1 >= M ||
        area[r0][c0] == '1' || area[r1][c1] == '1') {
        return 0; // O -1, a seconda di come si vuole indicare l'errore. Qui ritorno 0.
    }

    // Se partenza e arrivo coincidono, la lunghezza è 1 (la casella stessa)
    if (r0 == r1 && c0 == c1) {
        return 1;
    }

    /* --- 2. GESTIONE MEMORIA (Allocazione Coda e Visited) --- */
    
    // Allocazione Coda: nel caso peggiore potremmo dover accodare tutte le celle.
    // Usiamo malloc con sizeof(Node).
    Node *queue = (Node *)malloc(N * M * sizeof(Node));
    if (queue == NULL) return -1; // Errore allocazione

    // Allocazione array Visited (appiattito 1D per comodità): 
    // Usiamo calloc per inizializzare tutto a 0 (false).
    // Indice = r * M + c.
    bool *visited = (bool *)calloc(N * M, sizeof(bool));
    if (visited == NULL) {
        free(queue);
        return -1;
    }

    /* --- 3. INIZIALIZZAZIONE BFS --- */
    int head = 0; // Indice di estrazione (testa della coda)
    int tail = 0; // Indice di inserimento (coda della coda)

    // Inserisco il nodo di partenza
    queue[tail].r = r0;
    queue[tail].c = c0;
    queue[tail].dist = 1; // La lunghezza conta il numero di caselle, inclusa la partenza
    tail++;

    // Marco la partenza come visitata
    visited[r0 * M + c0] = true;

    // Vettori direzionali per esplorare: Su, Giù, Sinistra, Destra
    // dr = delta righe, dc = delta colonne
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    int result = 0; // Variabile per il risultato (0 significa nessun percorso trovato)

    /* --- 4. CICLO BFS (IL CUORE DELL'ALGORITMO) --- */
    while (head < tail) {
        // Dequeue: Estraggo l'elemento corrente
        Node current = queue[head];
        head++;

        // Controllo se sono arrivato a destinazione
        if (current.r == r1 && current.c == c1) {
            result = current.dist;
            break; // Trovato! Esco dal ciclo.
        }

        // Esploro i 4 vicini adiacenti
        for (int i = 0; i < 4; i++) {
            int nr = current.r + dr[i]; // Nuova riga
            int nc = current.c + dc[i]; // Nuova colonna

            // Verifico se il vicino è valido:
            // 1. È dentro i bordi?
            // 2. È una casella libera ('0')?
            // 3. NON è stato ancora visitato?
            if (nr >= 0 && nr < N && nc >= 0 && nc < M && 
                area[nr][nc] == '0' && 
                !visited[nr * M + nc]) {
                
                // Enqueue: Aggiungo il vicino alla coda
                queue[tail].r = nr;
                queue[tail].c = nc;
                queue[tail].dist = current.dist + 1; // Incremento la distanza
                tail++;

                // Marco subito come visitato per evitare di ri-aggiungerlo
                visited[nr * M + nc] = true;
            }
        }
    }

    /* --- 5. PULIZIA DELLA MEMORIA --- */
    // Fondamentale: liberare tutto ciò che abbiamo allocato con malloc/calloc
    free(queue);
    free(visited);

    return result; // Ritorna la distanza trovata o 0 se non c'è percorso
}

/* ==========================================
   SEZIONE 3: MAIN DI TEST (Da non incollare se non richiesto)
   ========================================== */
int main() {
    // Creiamo la matrice dell'esempio 4x5
    // 01000
    // 00010 (NB: L'esempio nell'immagine ha delle 'x' che sono il percorso, assumiamo fossero '0')
    // 01000 
    // 00000
    
    int N = 4, M = 5;
    
    // Allocazione dinamica della matrice di puntatori (per simulare char **area)
    char **area = (char **)malloc(N * sizeof(char *));
    for (int i = 0; i < N; i++) {
        area[i] = (char *)malloc(M * sizeof(char));
    }

    // Popolamento manuale simile all'esempio (dove 'x' sono celle libere usate dal percorso)
    // Riga 0: 01000
    area[0][0]='0'; area[0][1]='1'; area[0][2]='0'; area[0][3]='0'; area[0][4]='0';
    // Riga 1: 00010 (Start 1,0)
    area[1][0]='0'; area[1][1]='0'; area[1][2]='0'; area[1][3]='1'; area[1][4]='0';
    // Riga 2: 01000 (End 2,3)
    area[2][0]='0'; area[2][1]='1'; area[2][2]='0'; area[2][3]='0'; area[2][4]='0';
    // Riga 3: 00000
    area[3][0]='0'; area[3][1]='0'; area[3][2]='0'; area[3][3]='0'; area[3][4]='0';

    int r0 = 1, c0 = 0; // Start
    int r1 = 2, c1 = 3; // End

    printf("Calcolo percorso minimo da (%d,%d) a (%d,%d)...\n", r0, c0, r1, c1);
    
    int passi = minPath(area, N, M, r0, c0, r1, c1);
    
    printf("Lunghezza percorso minimo: %d\n", passi);
    // Risultato atteso dall'immagine: 5

    // Free della matrice di test
    for(int i=0; i<N; i++) free(area[i]);
    free(area);

    return 0;
}