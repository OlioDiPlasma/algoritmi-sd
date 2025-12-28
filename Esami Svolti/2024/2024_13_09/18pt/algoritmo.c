#include "strutture.h"
#include <float.h>

// Variabili statiche per evitare di passarle in ogni chiamata ricorsiva
static int *best_sol;
static float best_avg;
static int *curr_sol;

// Funzione ricorsiva (Backtracking)
void solve(int k, int *clienti, int n_clienti, DISTMATR d, SEDI s, int MAXD, int MINS) {
    
    // CASO TERMINALE: Ho assegnato tutti i clienti
    if (k == n_clienti) {
        // Calcolo statistiche della soluzione corrente
        int *counts = calloc(d->N, sizeof(int));
        float costo_totale = 0;

        for (int i = 0; i < n_clienti; i++) {
            int citta_reale = clienti[i];
            int sede_scelta = curr_sol[i];
            costo_totale += d->mat[citta_reale][sede_scelta];
            counts[sede_scelta]++;
        }

        // Verifica vincolo MINS (Ogni sede attiva deve avere almeno MINS clienti assegnati)
        int valido = 1;
        for (int i = 0; i < d->N; i++) {
            if (s->is_sede[i] && counts[i] < MINS) {
                valido = 0;
                break;
            }
        }

        // Se valido e migliora la media, aggiorno best_sol
        if (valido) {
            float media = costo_totale / n_clienti;
            if (media < best_avg) {
                best_avg = media;
                for (int i = 0; i < n_clienti; i++) best_sol[i] = curr_sol[i];
            }
        }
        
        free(counts);
        return;
    }

    // PASSO RICORSIVO
    int citta_corrente = clienti[k];

    // Provo ad assegnare la città corrente a tutte le sedi disponibili
    for (int i = 0; i < d->N; i++) {
        if (s->is_sede[i]) {
            // PRUNING: Controllo subito la distanza MAXD
            if (d->mat[citta_corrente][i] <= MAXD) {
                curr_sol[k] = i; // Assegno al cliente k la sede i
                solve(k + 1, clienti, n_clienti, d, s, MAXD, MINS);
            }
        }
    }
}

// Wrapper principale (Domanda 7)
SERVIZI bestPart(ELENCO e, DISTMATR d, SEDI s, int MAXD, int MINS) {
    int N = e->N;
    
    // Identifico chi sono i clienti (città che non sono sedi)
    int *clienti = malloc(N * sizeof(int));
    int n_clienti = 0;
    for (int i = 0; i < N; i++) {
        if (!s->is_sede[i]) {
            clienti[n_clienti++] = i;
        }
    }

    // Preparo le strutture per il backtracking
    best_sol = malloc(n_clienti * sizeof(int));
    curr_sol = malloc(n_clienti * sizeof(int));
    best_avg = FLT_MAX;

    // Avvio la ricorsione
    solve(0, clienti, n_clienti, d, s, MAXD, MINS);

    // Costruisco il risultato finale da ritornare
    SERVIZI res = malloc(sizeof(Servizi_t));
    res->N = N;
    res->assegnazioni = malloc(N * sizeof(int));
    res->media = best_avg;

    // Riempio il vettore assegnazioni finale
    // Le sedi si servono da sole, i clienti usano best_sol
    for (int i = 0; i < N; i++) res->assegnazioni[i] = i; // Default

    if (best_avg < FLT_MAX) {
        for (int i = 0; i < n_clienti; i++) {
            res->assegnazioni[clienti[i]] = best_sol[i];
        }
    } else {
        res->media = -1; // Nessuna soluzione trovata
    }

    free(clienti);
    free(best_sol);
    free(curr_sol);
    return res;
}