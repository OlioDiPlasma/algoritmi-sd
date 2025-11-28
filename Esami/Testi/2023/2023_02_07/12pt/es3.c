#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INF 99999

/* * Struttura per mantenere lo stato della soluzione migliore trovata 
 * durante la ricorsione.
 */
typedef struct {
    int min_gruppi; // Il minor numero di gruppi trovato finora
} Soluzione;

/*
 * Funzione ausiliaria per verificare se una città 'citta_corrente' 
 * può essere inserita nel 'gruppo_id'.
 * Controlla se 'citta_corrente' è compatibile con TUTTI i membri già presenti in quel gruppo.
 */
bool e_compatibile(int citta_corrente, int gruppo_id, int N, int *assegnazioni, bool **connessioni) {
    for (int i = 0; i < citta_corrente; i++) {
        // Se la città 'i' appartiene al gruppo in cui vogliamo entrare...
        if (assegnazioni[i] == gruppo_id) {
            // ...verifichiamo che ci sia connessione reciproca (distanza <= 2)
            // Nota: connessioni[u][v] è true se u e v sono mutualmente raggiungibili
            if (!connessioni[citta_corrente][i]) {
                return false; 
            }
        }
    }
    return true;
}

/*
 * FUNZIONE RICORSIVA (Backtracking)
 * citta_idx: la città che stiamo analizzando attualmente (da 0 a N-1)
 * num_gruppi_usati: quanti gruppi abbiamo creato finora nel ramo corrente
 * assegnazioni: array dove assegnazioni[i] = ID del gruppo della città i
 * connessioni: matrice pre-calcolata delle raggiungibilità (<= 1 intermediario)
 * best: puntatore alla struttura che tiene traccia del record mondiale (minimo gruppi)
 */
void trova_raggruppamenti(int citta_idx, int num_gruppi_usati, int N, 
                          int *assegnazioni, bool **connessioni, Soluzione *best) {
    
    // Pruning: Se stiamo già usando più gruppi della soluzione migliore trovata finora,
    // non ha senso continuare questo ramo.
    if (num_gruppi_usati >= best->min_gruppi) {
        return;
    }

    // Caso Base: Abbiamo assegnato un gruppo a tutte le città
    if (citta_idx == N) {
        if (num_gruppi_usati < best->min_gruppi) {
            best->min_gruppi = num_gruppi_usati;
        }
        return;
    }

    // Tentativo 1: Provare ad aggiungere la città corrente a uno dei gruppi esistenti
    for (int g = 0; g < num_gruppi_usati; g++) {
        if (e_compatibile(citta_idx, g, N, assegnazioni, connessioni)) {
            assegnazioni[citta_idx] = g; // Assegna
            trova_raggruppamenti(citta_idx + 1, num_gruppi_usati, N, assegnazioni, connessioni, best);
            assegnazioni[citta_idx] = -1; // Backtrack (annulla assegnazione)
        }
    }

    // Tentativo 2: Creare un NUOVO gruppo per la città corrente
    assegnazioni[citta_idx] = num_gruppi_usati;
    trova_raggruppamenti(citta_idx + 1, num_gruppi_usati + 1, N, assegnazioni, connessioni, best);
    assegnazioni[citta_idx] = -1; // Backtrack
}

/*
 * FUNZIONE PRINCIPALE (Wrapper)
 * Prepara i dati e lancia la ricorsione.
 */
int calcola_minimo_gruppi(int N, int **M) {
    // 1. Matrice di supporto per la raggiungibilità
    // connessioni[i][j] sarà true se dist(i,j) <= 2 e dist(j,i) <= 2
    bool **connessioni = (bool **)malloc(N * sizeof(bool *));
    for (int i = 0; i < N; i++) {
        connessioni[i] = (bool *)calloc(N, sizeof(bool));
    }

    // 2. Pre-calcolo delle connessioni (Dirette o con 1 intermediario)
    // Usiamo una matrice temporanea 'raggiungibile' monodirezionale
    bool **raggiungibile = (bool **)malloc(N * sizeof(bool *));
    for (int i = 0; i < N; i++) raggiungibile[i] = (bool *)calloc(N, sizeof(bool));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) {
                raggiungibile[i][j] = true;
                continue;
            }
            // Controllo diretto
            if (M[i][j] > 0) {
                raggiungibile[i][j] = true;
            } else {
                // Controllo con 1 intermediario (k)
                for (int k = 0; k < N; k++) {
                    if (M[i][k] > 0 && M[k][j] > 0) {
                        raggiungibile[i][j] = true;
                        break;
                    }
                }
            }
        }
    }

    // 3. Verifica "Mutua Raggiungibilità"
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // Devono potersi raggiungere in entrambe le direzioni
            if (raggiungibile[i][j] && raggiungibile[j][i]) {
                connessioni[i][j] = true;
            } else {
                connessioni[i][j] = false;
            }
        }
    }

    // 4. Preparazione alla ricorsione
    int *assegnazioni = (int *)malloc(N * sizeof(int));
    for(int i=0; i<N; i++) assegnazioni[i] = -1;

    Soluzione best;
    best.min_gruppi = N; // Nel caso peggiore, ogni città è un gruppo da sola

    // 5. Avvio Ricorsione
    trova_raggruppamenti(0, 0, N, assegnazioni, connessioni, &best);

    // 6. Pulizia Memoria
    for (int i = 0; i < N; i++) {
        free(connessioni[i]);
        free(raggiungibile[i]);
    }
    free(connessioni);
    free(raggiungibile);
    free(assegnazioni);

    return best.min_gruppi;
}

// Esempio di utilizzo (Main opzionale per test)
int main() {
    int N = 4;
    int **M = (int **)malloc(N * sizeof(int *));
    for(int i=0; i<N; i++) M[i] = (int *)calloc(N, sizeof(int));

    // Esempio: A(0)-B(1)-C(2)-D(3) lineare
    // 0 raggiunge 2 (tramite 1), ma 0 non raggiunge 3 in max 1 passo.
    M[0][1] = 10; M[1][0] = 10;
    M[1][2] = 10; M[2][1] = 10;
    M[2][3] = 10; M[3][2] = 10;

    printf("Minimo numero di gruppi: %d\n", calcola_minimo_gruppi(N, M));
    // Output atteso: 2 (es: {0,1,2} e {3} oppure {0,1} e {2,3})
    
    return 0;
}