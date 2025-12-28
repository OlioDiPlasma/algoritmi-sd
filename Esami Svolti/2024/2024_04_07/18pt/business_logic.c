#include "pronto_soccorso.h"

/* --- PARTE 1: VERIFICA SEDI (Domanda 6) --- */

/*
 * Scopo: Verifica se un insieme di sedi rispetta i vincoli (copertura e carico minimo).
 * Parametri: distanze, sedi scelte, distanza max (MAXD), carico min (MINS).
 * Return: 1 se valida, 0 altrimenti.
 */
int checkSedi(DISTMATR dist, SEDI sedi, int MAXD, int MINS) {
    // Mappatura rapida: isSede[i] = 1 se la città i è una sede
    int *isSede = (int *)calloc(dist.n, sizeof(int));
    for (int k = 0; k < sedi.m; k++) isSede[sedi.elencoSedi[k]] = 1;

    // 1. Vincolo Copertura: Ogni città non-sede deve avere un PS entro MAXD
    for (int i = 0; i < dist.n; i++) {
        if (!isSede[i]) { 
            int coperta = 0;
            for (int k = 0; k < sedi.m; k++) {
                // Controllo distanza tra città i e sede k-esima
                if (dist.mat[i][sedi.elencoSedi[k]] <= MAXD) {
                    coperta = 1;
                    break;
                }
            }
            if (!coperta) { free(isSede); return 0; }
        }
    }

    // 2. Vincolo Carico Minimo: Ogni sede deve poter servire almeno MINS città
    for (int k = 0; k < sedi.m; k++) {
        int idSede = sedi.elencoSedi[k];
        int count = 0;
        for (int i = 0; i < dist.n; i++) {
            // Conta città NON sede che sono nel raggio MAXD
            if (!isSede[i] && dist.mat[i][idSede] <= MAXD) {
                count++;
            }
        }
        if (count < MINS) { free(isSede); return 0; }
    }

    free(isSede);
    return 1;
}

/* --- PARTE 2: OTTIMIZZAZIONE (Domanda 7) --- */

/*
 * Funzione ausiliaria per checkPart
 * Verifica se l'assegnazione finale rispetta il vincolo MINS per ogni sede.
 */
int checkPartValid(int *sol, int n, SEDI sedi, int MINS) {
    // Array per contare quanti pazienti vanno a ogni città-sede
    int *counts = (int *)calloc(n, sizeof(int));

    for (int i = 0; i < n; i++) {
        int idSedeAssegnata = sol[i];
        // Se la città i non è essa stessa la sede, conta come carico
        if (i != idSedeAssegnata) {
            counts[idSedeAssegnata]++;
        }
    }

    int valid = 1;
    for (int k = 0; k < sedi.m; k++) {
        if (counts[sedi.elencoSedi[k]] < MINS) {
            valid = 0;
            break;
        }
    }
    free(counts);
    return valid;
}

/*
 * Funzione Ricorsiva (Backtracking)
 * currDistTot: accumulatore distanza per calcolo media
 */
void bestPartR(int pos, int *currSol, SERVIZI *bestSol, DISTMATR dist, 
               SEDI sedi, int MAXD, int MINS, int currDistTot) {

    // PRUNING: Se la media parziale supera già il best, inutile continuare
    // Nota: approssimiamo il divisore con (dist.n - sedi.m) per stima
    int numServiti = dist.n - sedi.m;
    if (bestSol->mediaDist != FLT_MAX && numServiti > 0) {
        if ((float)currDistTot / numServiti >= bestSol->mediaDist) return;
    }

    // CASO TERMINALE: Ho deciso l'assegnazione per tutte le N città
    if (pos >= dist.n) {
        if (checkPartValid(currSol, dist.n, sedi, MINS)) {
            // Calcolo media: somma distanze / numero città servite
            float media = (float)currDistTot / numServiti;
            
            if (media < bestSol->mediaDist) {
                bestSol->mediaDist = media;
                // Copia la soluzione nel risultato finale
                for (int i = 0; i < dist.n; i++) 
                    bestSol->assegnazioni[i] = currSol[i];
            }
        }
        return;
    }

    // PASSO RICORSIVO
    // Verifico se la città attuale 'pos' è una sede
    int isSede = 0;
    for (int k = 0; k < sedi.m; k++) {
        if (sedi.elencoSedi[k] == pos) isSede = 1;
    }

    if (isSede) {
        // Se 'pos' è una sede, si serve da sola (distanza 0)
        currSol[pos] = pos;
        bestPartR(pos + 1, currSol, bestSol, dist, sedi, MAXD, MINS, currDistTot);
    } else {
        // Se 'pos' è un utente, prova ad assegnarlo a ogni sede disponibile
        for (int k = 0; k < sedi.m; k++) {
            int idSede = sedi.elencoSedi[k];
            int d = dist.mat[pos][idSede];

            // Assegno solo se entro il raggio d'azione
            if (d <= MAXD) {
                currSol[pos] = idSede;
                // Ricorsione aggiungendo la distanza d
                bestPartR(pos + 1, currSol, bestSol, dist, sedi, MAXD, MINS, currDistTot + d);
            }
        }
    }
}

// Wrapper function (Interfaccia pubblica)
SERVIZI bestPart(ELENCO elenco, DISTMATR dist, SEDI sedi, int MAXD, int MINS) {
    SERVIZI best;
    best.assegnazioni = (int *)malloc(elenco.n * sizeof(int));
    best.mediaDist = FLT_MAX; // Inizializzo a infinito

    int *currSol = (int *)malloc(elenco.n * sizeof(int));

    bestPartR(0, currSol, &best, dist, sedi, MAXD, MINS, 0);

    free(currSol);
    return best;
}