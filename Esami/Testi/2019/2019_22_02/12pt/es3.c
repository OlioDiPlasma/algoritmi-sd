#include <stdio.h>
#include <stdlib.h>

// RISULTATO DIVERSO DA QUELLO ATTESO PERò SONO ENTRAMBI corretti amen


/*
 * ======================================================================================
 * NOME FUNZIONE: stampaPercorso
 * ======================================================================================
 * SCOPO:
 * Funzione ricorsiva per stampare la sottosequenza ricostruita.
 * Risale la catena dei predecessori salvata negli array `pred`.
 *
 * PARAMETRI:
 * - int currIdx: L'indice del vettore V che stiamo stampando.
 * - int isPeak: Booleano (0 o 1). Indica se currIdx è un Picco (1) o una Valle (0).
 * - int *P_valle: Array dei predecessori per le sequenze che terminano in Valle.
 * - int *P_picco: Array dei predecessori per le sequenze che terminano in Picco.
 * - int *V: Il vettore originale dei dati.
 *
 * LOGICA:
 * Usiamo la ricorsione per andare indietro fino all'inizio (quando pred == -1),
 * poi stampiamo ritornando dalla ricorsione. Questo stampa i numeri nell'ordine corretto.
 * ======================================================================================
 */
void stampaPercorso(int currIdx, int isPeak, int *P_valle, int *P_picco, int *V) {
    /* Caso Base: se non c'è predecessore, siamo all'inizio della catena */
    if (currIdx == -1) return;

    /* Determino chi era il predecessore in base al fatto che io sia Picco o Valle */
    int prevIdx;
    if (isPeak) {
        prevIdx = P_picco[currIdx];
        /* Se io sono un Picco, il mio predecessore era una Valle.
         * Chiamata ricorsiva passando 0 (isPeak = false) */
        stampaPercorso(prevIdx, 0, P_valle, P_picco, V);
    } else {
        prevIdx = P_valle[currIdx];
        /* Se io sono una Valle, il mio predecessore era un Picco (se esiste).
         * Nota: Il primo elemento è sempre una Valle, e avrà prevIdx -1. */
        stampaPercorso(prevIdx, 1, P_valle, P_picco, V);
    }

    /* Stampa del valore corrente (avviene dopo la ricorsione, quindi in ordine) */
    printf("%d ", V[currIdx]);
}

/*
 * ======================================================================================
 * NOME FUNZIONE: trovaSottosequenzaAlternante
 * ======================================================================================
 * SCOPO:
 * Calcola e visualizza la sottosequenza alternante minore-maggiore di lunghezza massima.
 *
 * PARAMETRI:
 * - int *V: Il vettore di interi input.
 * - int N: La dimensione del vettore.
 *
 * CONCETTI CHIAVE (Programmazione Dinamica):
 * Manteniamo due stati per ogni elemento 'i' del vettore:
 * 1. L_valle[i]: Lunghezza max della sequenza che termina in V[i] essendo V[i] una VALLE (basso).
 * 2. L_picco[i]: Lunghezza max della sequenza che termina in V[i] essendo V[i] un PICCO (alto).
 *
 * La regola "Minore-Maggiore" implica che la sequenza inizi con: Y[0] < Y[1].
 * Quindi Y[0] è una Valle, Y[1] è un Picco.
 *
 * Transizioni:
 * - Se V[i] > V[j]: V[i] può essere un PICCO che segue la VALLE V[j].
 * - Se V[i] < V[j]: V[i] può essere una VALLE che segue il PICCO V[j].
 * ======================================================================================
 */
void trovaSottosequenzaAlternante(int *V, int N) {
    if (N == 0) return;

    /* --- 1. ALLOCAZIONE MEMORIA --- */
    /*
     * Allochiamo 4 array di supporto:
     * - L_valle/L_picco: per memorizzare le lunghezze massime.
     * - P_valle/P_picco: per memorizzare gli indici dei predecessori (per la stampa).
     *
     * sizeof(int) * N: spazio necessario per N interi.
     */
    int *L_valle = (int*)malloc(N * sizeof(int));
    int *L_picco = (int*)malloc(N * sizeof(int));
    int *P_valle = (int*)malloc(N * sizeof(int));
    int *P_picco = (int*)malloc(N * sizeof(int));

    /* Controllo allocazione */
    if (!L_valle || !L_picco || !P_valle || !P_picco) {
        printf("Errore malloc\n");
        exit(1);
    }

    /* --- 2. INIZIALIZZAZIONE --- */
    for (int i = 0; i < N; i++) {
        /*
         * Ogni elemento è almeno una sequenza "Valle" di lunghezza 1 (inizio potenziale).
         * Nessun elemento può essere un "Picco" da solo all'inizio (perché Y[0] < Y[1]),
         * quindi un picco deve avere lunghezza almeno 2. Inizializziamo L_picco a 0.
         */
        L_valle[i] = 1;
        L_picco[i] = 0; // O un valore molto basso

        P_valle[i] = -1; // Nessun predecessore
        P_picco[i] = -1;
    }

    /* --- 3. LOGICA DP (Doppio Ciclo) --- */
    /* i: indice corrente che stiamo analizzando (destinazione) */
    for (int i = 1; i < N; i++) {
        /* j: indice precedente che controlliamo (sorgente) */
        for (int j = 0; j < i; j++) {

            /* CASO A: V[i] è MAGGIORE di V[j] (Stiamo salendo / Going Up) */
            /* V[i] può diventare un PICCO estendendo una VALLE che finiva in j */
            if (V[i] > V[j]) {
                /* Controllo se allungare la sequenza conviene */
                if (L_valle[j] + 1 > L_picco[i]) {
                    L_picco[i] = L_valle[j] + 1; // Aggiorno lunghezza
                    P_picco[i] = j;              // Salvo predecessore
                }
            }

            /* CASO B: V[i] è MINORE di V[j] (Stiamo scendendo / Going Down) */
            /* V[i] può diventare una VALLE estendendo un PICCO che finiva in j */
            if (V[i] < V[j]) {
                /*
                 * Attenzione: Posso estendere un picco solo se esiste (L_picco[j] > 0).
                 * Questo garantisce che non estendiamo una sequenza non valida.
                 */
                if (L_picco[j] > 0 && L_picco[j] + 1 > L_valle[i]) {
                    L_valle[i] = L_picco[j] + 1;
                    P_valle[i] = j;
                }
            }
        }
    }

    /* --- 4. RICERCA DEL MASSIMO --- */
    /* Cerchiamo la lunghezza massima tra tutte le sequenze trovate (sia che finiscano in valle o picco) */
    int maxLen = 0;
    int bestIdx = -1;
    int endAsPeak = 0; // Flag per sapere se finiamo con picco o valle

    for (int i = 0; i < N; i++) {
        /* Controllo sequenze che finiscono come PICCO */
        if (L_picco[i] > maxLen) {
            maxLen = L_picco[i];
            bestIdx = i;
            endAsPeak = 1;
        }
        /* Controllo sequenze che finiscono come VALLE */
        if (L_valle[i] > maxLen) {
            maxLen = L_valle[i];
            bestIdx = i;
            endAsPeak = 0;
        }
    }

    /* --- 5. STAMPA RISULTATI --- */
    printf("Lunghezza massima trovata: %d\n", maxLen);
    printf("Sottosequenza: ");
    if (bestIdx != -1) {
        stampaPercorso(bestIdx, endAsPeak, P_valle, P_picco, V);
    }
    printf("\n");

    /* --- 6. GESTIONE MEMORIA (Free) --- */
    free(L_valle);
    free(L_picco);
    free(P_valle);
    free(P_picco);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    /* --- 1. DEFINIZIONE DATI --- */
    /* Usiamo l'esempio dell'immagine */
    int V[] = {8, 9, 6, 4, 5, 7, 3, 2, 4};
    /* Calcolo dimensione: totale byte diviso byte di un elemento */
    int N = sizeof(V) / sizeof(V[0]);

    printf("--- Test Sottosequenza Alternante Minore-Maggiore ---\n");
    printf("Vettore Input: [ ");
    for(int i=0; i<N; i++) printf("%d ", V[i]);
    printf("]\n\n");

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    trovaSottosequenzaAlternante(V, N);

    /* Output Atteso (secondo esempio): Lunghezza 6, Seq: 8 9 6 7 3 4 */

    return 0;
}