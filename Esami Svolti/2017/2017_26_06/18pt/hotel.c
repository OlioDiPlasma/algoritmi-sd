#include "hotel.h"
#include <float.h> // Per DBL_MAX, etc.

/* ============================================================================
 * SEZIONE 1: GESTIONE INPUT/OUTPUT E MEMORIA
 * ============================================================================ */

/*
 * Scopo: Leggere il file delle prenotazioni.
 * Input: Nome file.
 * Parametri: Puntatore a intero per restituire il numero di clienti letti.
 * Ritorna: Un vettore dinamico di strutture Cliente.
 *
 * Concetto Chiave (Realloc): Non sapendo a priori quante righe ha il file,
 * non possiamo fare una malloc singola precisa. Usiamo una strategia 
 * di "ridimensionamento dinamico" o, per semplicita' in esame, leggiamo prima
 * contando le righe e poi facciamo una malloc esatta (approccio usato qui).
 */
Cliente* leggiPrenotazioni(char *filename, int *num_clienti) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore apertura file prenotazioni.\n");
        exit(1);
    }

    /* Passo 1: Conta le righe */
    int count = 0;
    char tempBuffer[100];
    while (fgets(tempBuffer, 100, fp) != NULL) {
        count++;
    }
    rewind(fp); // Torna all'inizio del file

    /* Passo 2: Alloca memoria esatta */
    /* sizeof(Cliente) ci da' i byte necessari per una struct. 
       Moltiplichiamo per count. */
    Cliente *vett = (Cliente *)malloc(count * sizeof(Cliente));
    if (vett == NULL) {
        printf("Errore malloc clienti.\n");
        exit(1);
    }

    /* Passo 3: Leggi i dati */
    int i = 0;
    while (fscanf(fp, "%s %d %d", vett[i].id_cliente, &vett[i].arrivo, &vett[i].pernottamenti) == 3) {
        vett[i].hotel_assegnato = -1; // Inizializziamo a "nessuno"
        i++;
    }

    fclose(fp);
    *num_clienti = count;
    return vett;
}

void liberaMemoria(Cliente *c, Hotel *h, int n_hotels) {
    free(c); // Libera il vettore clienti
    // Per gli hotel, dobbiamo liberare il vettore interno di ogni hotel
    for (int i = 0; i < n_hotels; i++) {
        free(h[i].camere_occupate_giornaliere);
    }
    free(h); // Infine libera il vettore di hotel
}

/* ============================================================================
 * SEZIONE 2: VERIFICA (Parte richiesta 1)
 * ============================================================================ */

/*
 * Scopo: Verificare se un file di allocazione proposto e' valido.
 * Nota: Dobbiamo resettare lo stato degli hotel prima di controllare.
 */
int verificaAllocazione(char *filename, Cliente *clienti, int n_clienti, Hotel *hotels, int n_hotels, int X) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return 0;

    /* --- Reset Strutture Hotel --- */
    /* Dobbiamo assicurarci che i contatori siano a zero prima di iniziare */
    for(int i=0; i<n_hotels; i++) {
        for(int j=0; j<X; j++) hotels[i].camere_occupate_giornaliere[j] = 0;
    }

    char id_cli[MAX_STR];
    int id_albergo;
    int valido = 1;

    /* Leggiamo riga per riga: cliente e hotel proposto */
    while(fscanf(fp, "%s %d", id_cli, &id_albergo) == 2) {
        
        /* Validazione base indice hotel */
        if (id_albergo < 0 || id_albergo >= n_hotels) {
            valido = 0; 
            break; 
        }

        /* Dobbiamo trovare il cliente nel nostro vettore per sapere giorni e notti */
        int idx_cliente = -1;
        for(int k=0; k<n_clienti; k++) {
            if(strcmp(clienti[k].id_cliente, id_cli) == 0) {
                idx_cliente = k;
                break;
            }
        }

        if(idx_cliente == -1) { 
            // Cliente nel file allocazione non esiste in prenotazioni
            valido = 0; break; 
        }

        /* --- Verifica Capacita' --- */
        /* Aggiorniamo l'occupazione per i giorni del soggiorno */
        int start = clienti[idx_cliente].arrivo;
        int duration = clienti[idx_cliente].pernottamenti;
        
        // Controllo se sfora i giorni totali X (non dovrebbe succedere se dati corretti, ma per sicurezza)
        if (start + duration > X) { valido = 0; break; }

        for(int d = start; d < start + duration; d++) {
            hotels[id_albergo].camere_occupate_giornaliere[d]++;
            /* QUI IL CONTROLLO CRUCIALE: Ho superato M? */
            if (hotels[id_albergo].camere_occupate_giornaliere[d] > hotels[id_albergo].M) {
                valido = 0; // Capacita' superata
            }
        }
        if(!valido) break;
    }

    fclose(fp);
    
    /* Reset finale per non lasciare sporchi gli hotel per le fasi successive */
    for(int i=0; i<n_hotels; i++) {
        for(int j=0; j<X; j++) hotels[i].camere_occupate_giornaliere[j] = 0;
    }
    
    return valido;
}

/* ============================================================================
 * SEZIONE 3: OTTIMIZZAZIONE (Il cuore da 18 punti)
 * ============================================================================ */

/* Variabili globali "di supporto" alla ricorsione per evitare di passarle sempre */
static double max_f = -DBL_MAX; // Valore massimo trovato finora (inizializzato a -infinito)
static int *best_allocation = NULL; // Vettore per salvare la combinazione vincente

/*
 * Funzione Ausiliaria: Calcola F
 * Implementa la formula matematica dell'immagine.
 */
double calcolaFunzioneF(Hotel *hotels, int n_hotels, int X, int a, int b) {
    double sum_ri = 0.0;
    double r_max = -1.0;
    double r_min = 2.0; // r_i e' max 1, quindi 2 e' sicuro come init

    for (int i = 0; i < n_hotels; i++) {
        /* Calcolo r_i = (Totale Notti Occupate) / (M_i * X) */
        /* Attenzione al cast a double per non fare divisione intera */
        double num = (double)hotels[i].totale_camere_occupate_periodo;
        double den = (double)(hotels[i].M * X);
        double ri = num / den;

        sum_ri += ri;
        if (ri > r_max) r_max = ri;
        if (ri < r_min) r_min = ri;
    }

    /* Gestione caso limite r_max = 0 (hotel vuoti) per evitare divisione per zero */
    double penalty = 0.0;
    if (r_max > 0.00001) {
        penalty = ((r_max - r_min) / r_max);
    }

    /* Formula: f = a * sum(ri) - b * penalty */
    return (a * sum_ri) - (b * penalty);
}

/*
 * Funzione Ricorsiva (Backtracking)
 * livello: indice del cliente che stiamo considerando (0 .. n_clienti-1)
 */
void backtrack(int livello, Cliente *clienti, int n_clienti, Hotel *hotels, int n_hotels, int X, int a, int b) {
    
    /* --- CASO BASE --- */
    /* Abbiamo assegnato tutti i clienti (dal 0 al n-1) */
    if (livello == n_clienti) {
        double current_f = calcolaFunzioneF(hotels, n_hotels, X, a, b);
        
        /* Se abbiamo trovato una soluzione migliore, salviamola */
        if (current_f > max_f) {
            max_f = current_f;
            for(int k=0; k<n_clienti; k++) {
                best_allocation[k] = clienti[k].hotel_assegnato;
            }
        }
        return;
    }

    /* --- PASSO RICORSIVO --- */
    /* Proviamo ad assegnare il cliente 'livello' ad ogni hotel possibile */
    Cliente *curr = &clienti[livello];

    for (int h = 0; h < n_hotels; h++) {
        
        /* PRUNING (Potatura): Controllo se l'hotel 'h' ha posto */
        int puo_ospitare = 1;
        int start = curr->arrivo;
        int end = curr->arrivo + curr->pernottamenti;

        for (int d = start; d < end; d++) {
            if (hotels[h].camere_occupate_giornaliere[d] >= hotels[h].M) {
                puo_ospitare = 0;
                break;
            }
        }

        if (puo_ospitare) {
            /* DO: Assegno */
            curr->hotel_assegnato = h;
            for (int d = start; d < end; d++) {
                hotels[h].camere_occupate_giornaliere[d]++;
            }
            hotels[h].totale_camere_occupate_periodo += curr->pernottamenti;

            /* RECURSE: Vado al prossimo cliente */
            backtrack(livello + 1, clienti, n_clienti, hotels, n_hotels, X, a, b);

            /* UNDO (Backtrack): Tolgo l'assegnazione per provare il prossimo hotel */
            hotels[h].totale_camere_occupate_periodo -= curr->pernottamenti;
            for (int d = start; d < end; d++) {
                hotels[h].camere_occupate_giornaliere[d]--;
            }
            curr->hotel_assegnato = -1;
        }
    }
}

/*
 * Funzione Wrapper: prepara tutto per la ricorsione
 */
void trovaSoluzioneOttima(Cliente *clienti, int n_clienti, Hotel *hotels, int n_hotels, int X, int a, int b, char *outfile) {
    
    /* Allocazione vettore per salvare la soluzione migliore */
    best_allocation = (int *)malloc(n_clienti * sizeof(int));
    if (best_allocation == NULL) exit(1);

    /* Reset strutture hotel per sicurezza */
    for(int i=0; i<n_hotels; i++) {
        hotels[i].totale_camere_occupate_periodo = 0;
        for(int j=0; j<X; j++) hotels[i].camere_occupate_giornaliere[j] = 0;
    }

    /* Avvio Backtracking */
    printf("Avvio ricerca soluzione ottima...\n");
    backtrack(0, clienti, n_clienti, hotels, n_hotels, X, a, b);

    /* Scrittura Risultato */
    FILE *fp = fopen(outfile, "w");
    if (fp == NULL) {
        printf("Errore creazione file output.\n");
        free(best_allocation);
        return;
    }

    if (max_f == -DBL_MAX) {
        printf("Nessuna soluzione valida trovata.\n");
    } else {
        printf("Soluzione ottima trovata! Valore f: %.4f\n", max_f);
        for (int i = 0; i < n_clienti; i++) {
            fprintf(fp, "%s %d\n", clienti[i].id_cliente, best_allocation[i]);
        }
    }

    fclose(fp);
    free(best_allocation);
}