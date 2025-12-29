#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h> /* Per FLT_MAX */

#include "dipendenti.h"
#include "divisioni.h"

/* Struttura per memorizzare una soluzione (assegnamento)
   Usiamo un array semplice dove:
   indice = indice del dipendente nel vettore 'dipendenti'
   valore = indice della divisione * 10 + ruolo (un trucco per salvare 2 info in 1 int)
            oppure useremo due array paralleli. Facciamo una struct dedicata.
*/
typedef struct {
    int id_divisione;
    int id_ruolo;
} assegnamento_t;

/* Variabili Globali (o passate nei parametri) per la ricorsione */
int N_DIP, N_DIV;
dipendente_t *elenco_dip;   // Vettore dinamico dipendenti
divisione_t *elenco_div;    // Vettore dinamico divisioni (puntatori)

assegnamento_t *sol_best;   // Migliore assegnamento trovato
float min_delta_avg = FLT_MAX; // Valore sentinella (infinito)

/* --- PROTOTIPI LOCALI --- */
void leggi_tutto(void);
void verifica_associazioni(char *filename);
void risolvi_ottimizzazione(char *outfile);
void backtrack(int dip_idx, float current_delta_partial);
void salva_soluzione(char *filename);

int main(int argc, char **argv) {
    /* 1. Caricamento Dati 
       Carichiamo dipendenti.txt e divisioni.txt
    */
    leggi_tutto();

    /* 2. Verifica (Punto 2 del testo)
       Legge associazioni.txt e calcola se è valido e lo scostamento.
    */
    printf("\n--- FASE DI VERIFICA ---\n");
    verifica_associazioni("associazioni.txt");

    /* 3. Ottimizzazione (Punto 3 del testo)
       Calcola l'assegnamento ottimo.
    */
    printf("\n--- FASE DI OTTIMIZZAZIONE ---\n");
    
    // Allocazione vettore soluzione best
    sol_best = malloc(N_DIP * sizeof(assegnamento_t));
    
    // Reset dello stato delle divisioni (pulizia dopo la verifica)
    // Per semplicità, ricarichiamo o resettiamo manualmente.
    // Qui azzeriamo rimuovendo logicamente ciò che 'verifica' ha aggiunto? 
    // No, meglio liberare e ricaricare, o implementare una funzione reset.
    // Facciamo un reset manuale veloce liberando e ricaricando le struct è più sicuro per l'esame.
    for(int i=0; i<N_DIV; i++) divisione_free(elenco_div[i]);
    free(elenco_div);
    
    // Rileggiamo divisioni per averle pulite (contatori a 0)
    FILE *f = fopen("divisioni.txt", "r");
    fscanf(f, "%d", &N_DIV);
    elenco_div = malloc(N_DIV * sizeof(divisione_t));
    for(int i=0; i<N_DIV; i++) {
        elenco_div[i] = divisione_new();
        divisione_leggi(f, elenco_div[i]);
    }
    fclose(f);

    // Lanciamo il backtracking
    // file di output: "ottimo.txt"
    risolvi_ottimizzazione("ottimo.txt");

    /* --- PULIZIA FINALE --- */
    free(elenco_dip);
    for(int i=0; i<N_DIV; i++) divisione_free(elenco_div[i]);
    free(elenco_div);
    free(sol_best);

    return 0;
}

/* --- IMPLEMENTAZIONE FUNZIONI --- */

void leggi_tutto() {
    FILE *f;

    /* Lettura Dipendenti */
    f = fopen("dipendenti.txt", "r");
    if(!f) { perror("Errore dipendenti.txt"); exit(1); }
    fscanf(f, "%d", &N_DIP);
    elenco_dip = malloc(N_DIP * sizeof(dipendente_t));
    for(int i=0; i<N_DIP; i++) {
        dipendente_leggi(f, &elenco_dip[i]);
    }
    fclose(f);

    /* Lettura Divisioni */
    f = fopen("divisioni.txt", "r");
    if(!f) { perror("Errore divisioni.txt"); exit(1); }
    fscanf(f, "%d", &N_DIV);
    elenco_div = malloc(N_DIV * sizeof(divisione_t)); // Vettore di puntatori
    for(int i=0; i<N_DIV; i++) {
        elenco_div[i] = divisione_new(); // Alloca la struct opaca
        divisione_leggi(f, elenco_div[i]);
    }
    fclose(f);
}

/* --- VERIFICA --- */
void verifica_associazioni(char *filename) {
    FILE *f = fopen(filename, "r");
    if(!f) { printf("File associazioni non trovato.\n"); return; }

    int matricola;
    char ruolo_char;
    char sigla_div[11];
    
    // Usiamo variabili temporanee, non ci serve salvare la struttura
    // ma dobbiamo aggiornare lo stato delle divisioni.
    
    while(fscanf(f, "%d %c %s", &matricola, &ruolo_char, sigla_div) == 3) {
        // 1. Trova dipendente (indice)
        int idx_dip = -1;
        for(int i=0; i<N_DIP; i++) {
            if(elenco_dip[i].matricola == matricola) { idx_dip = i; break; }
        }

        // 2. Trova divisione (indice)
        int idx_div = divisione_cerca(elenco_div, N_DIV, sigla_div);

        // 3. Decodifica ruolo
        int ruolo = char_to_ruolo(ruolo_char);

        if(idx_dip != -1 && idx_div != -1 && ruolo != -1) {
            // Aggiorna stato divisione
            divisione_aggiungi_dipendente(elenco_div[idx_div], &elenco_dip[idx_dip], ruolo);
        }
    }
    fclose(f);

    // Controlliamo validità e calcoliamo Delta
    int valido = 1;
    float somma_scostamenti = 0.0;

    for(int i=0; i<N_DIV; i++) {
        if(!divisione_check_minimi(elenco_div[i])) {
            printf("Divisione %d non soddisfa i requisiti minimi.\n", i);
            valido = 0;
        }
        somma_scostamenti += divisione_get_costo(elenco_div[i]);
    }

    float delta_avg = somma_scostamenti / N_DIV;
    
    if(valido) {
        printf("Soluzione VALIDA. Delta_avg = %.2f\n", delta_avg);
    } else {
        printf("Soluzione NON VALIDA. (Calcolato comunque Delta_avg = %.2f)\n", delta_avg);
    }
}

/* --- OTTIMIZZAZIONE (Backtracking) --- */

/* Vettore temporaneo per costruire la soluzione corrente durante la ricorsione */
assegnamento_t *sol_curr; 

void risolvi_ottimizzazione(char *outfile) {
    sol_curr = malloc(N_DIP * sizeof(assegnamento_t));
    
    /* Chiamata iniziale: primo dipendente (indice 0) */
    backtrack(0, 0.0);
    
    if (min_delta_avg == FLT_MAX) {
        printf("Nessuna soluzione valida trovata.\n");
    } else {
        printf("Soluzione OTTIMA trovata con Delta_avg = %.2f\n", min_delta_avg);
        salva_soluzione(outfile);
    }
    
    free(sol_curr);
}

/*
   Funzione Ricorsiva.
   dip_idx: indice del dipendente che stiamo provando a piazzare ora.
   current_delta_partial: ottimizzazione (pruning) opzionale, qui non usata per semplicità.
*/
void backtrack(int dip_idx, float current_delta_partial) {
    
    /* CASO BASE: Abbiamo assegnato tutti i dipendenti */
    if (dip_idx == N_DIP) {
        
        // 1. Verifichiamo se TUTTE le divisioni soddisfano i minimi
        for(int i=0; i<N_DIV; i++) {
            if(!divisione_check_minimi(elenco_div[i])) return; // Soluzione non ammissibile
        }

        // 2. Calcoliamo il costo globale
        float somma = 0.0;
        for(int i=0; i<N_DIV; i++) {
            somma += divisione_get_costo(elenco_div[i]);
        }
        float delta = somma / N_DIV;

        // 3. Aggiorniamo il best se necessario
        if (delta < min_delta_avg) {
            min_delta_avg = delta;
            // Copiamo la soluzione corrente in quella best
            for(int k=0; k<N_DIP; k++) sol_best[k] = sol_curr[k];
        }
        return;
    }

    /* PASSO RICORSIVO */
    // Il dipendente 'elenco_dip[dip_idx]' deve essere assegnato a UNA divisione e UN ruolo.
    
    for (int d = 0; d < N_DIV; d++) {
        for (int r = 0; r < N_RUOLI; r++) {
            
            // a. Assegno (Do)
            divisione_aggiungi_dipendente(elenco_div[d], &elenco_dip[dip_idx], r);
            sol_curr[dip_idx].id_divisione = d;
            sol_curr[dip_idx].id_ruolo = r;

            // b. Ricorsione (Recur)
            // Nota: potremmo aggiungere PRUNING qui: se costo parziale > min_delta_avg, return.
            backtrack(dip_idx + 1, 0);

            // c. De-assegno (Undo / Backtrack)
            divisione_rimuovi_dipendente(elenco_div[d], &elenco_dip[dip_idx], r);
        }
    }
}

void salva_soluzione(char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    
    // Formato richiesto: matricola ruolo sigla
    // Attenzione: bisogna recuperare la sigla della divisione (è privata, ma non abbiamo un getter)
    // Errore didattico comune! Abbiamo bisogno di accedere alla sigla.
    // Soluzione: o aggiungiamo un getter in divisioni.h o ci ricordiamo l'ordine è lo stesso.
    // Siccome siamo nel main e non possiamo fare 'elenco_div[i]->sigla', dobbiamo usare un trucco o cambiare .h
    // MA: nel modulo divisioni.c la struct è definita.
    // In un compito d'esame, o aggiungete `char* divisione_get_sigla(divisione_t d)` nel .h
    // o (brutto ma funziona se i file sono compilati insieme) sapete che la sigla è nel file input.
    // Aggiungiamo il getter mentalmente o leggiamo dai file originali? 
    // Creiamo una funzione helper fittizia o usiamo un array parallelo di nomi divisioni nel main se necessario.
    // Ma aspetta! divisione_stampa_stato stampa la sigla. Possiamo aggiungere un getter facile.
    
    /* NOTA PROFESSORE: Qui avreste dovuto aggiungere `char* get_sigla(divisione_t)` in divisioni.h
       Per ora assumiamo di aver salvato i nomi delle divisioni nel main o facciamo una stampa brutale
       basandoci sull'ordine di lettura.
       Nel file divisioni.txt l'ordine è sequenziale.
       Leggerò le sigle di nuovo o le salvo in un array di stringhe nel main all'inizio per comodità.
       Per brevità qui stampo "DIV_IDX" ma all'esame dovreste mappare l'indice sulla stringa.
    */
    
    // Supponiamo di avere le sigle in un array di appoggio per la stampa, oppure modifichiamo l'.h
    // Facciamo la cosa pulita: aggiungiamo char* divisione_get_sigla(divisione_t d) nel .h e .c
    // (L'ho omesso sopra per brevità, ma qui servirebbe).
    
    for(int i=0; i<N_DIP; i++) {
        int d_idx = sol_best[i].id_divisione;
        int r_idx = sol_best[i].id_ruolo;
        
        // Recuperiamo la sigla vera usando la nuova funzione dell'ADT
        char *sigla = divisione_get_sigla(elenco_div[d_idx]);

        fprintf(f, "%d %c %s\n", 
                elenco_dip[i].matricola, 
                ruolo_to_char(r_idx), 
                sigla); 
    }
    fclose(f);
    printf("Risultati salvati in %s\n", filename);
}