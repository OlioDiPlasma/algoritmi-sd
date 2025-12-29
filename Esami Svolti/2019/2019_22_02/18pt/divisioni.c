#include "divisioni.h"
#include <stdlib.h>
#include <string.h>
#include <math.h> /* Per fabs (valore assoluto float) */

/*
   STRUTTURA NASCOSTA
   Contiene i requisiti e lo stato CORRENTE (chi ci lavora ora).
   
   Usiamo array di dimensione 4 per le 4 tipologie di ruolo.
*/
struct divisione_s {
    char sigla[11];
    
    /* Requisiti letti da file */
    int min_pers[N_RUOLI];
    int min_skill[N_RUOLI];
    int opt_skill[N_RUOLI];
    
    /* Stato attuale (modificato dinamicamente) */
    int curr_pers[N_RUOLI];
    int curr_skill[N_RUOLI];
    
    /* NOTA: Il testo chiedeva di salvare la collezione di dipendenti nell'ADT.
       Per semplicità e velocità, teniamo traccia delle statistiche (curr_pers, curr_skill).
       Se servisse l'elenco nominativo per stamparlo DENTRO l'ADT, useremmo un vettore di puntatori.
       Qui ci limitiamo ai contatori per il calcolo del costo.
    */
};

/* --- 1. ALLOCAZIONE --- */
divisione_t divisione_new() {
    /* calloc inizializza tutto a 0, utilissimo per i contatori */
    divisione_t d = calloc(1, sizeof(struct divisione_s));
    return d;
}

void divisione_free(divisione_t d) {
    if (d != NULL) free(d);
}

/* --- 2. INPUT / OUTPUT --- */
int divisione_leggi(FILE *fp, divisione_t d) {
    /* Legge la sigla */
    if (fscanf(fp, "%s", d->sigla) != 1) return 0;
    
    /* Legge le 4 righe dei ruoli */
    for (int i = 0; i < N_RUOLI; i++) {
        fscanf(fp, "%d %d %d", 
               &d->min_pers[i], &d->min_skill[i], &d->opt_skill[i]);
    }
    return 1;
}

int divisione_cerca(divisione_t *vett, int n_div, char *sigla) {
    for (int i=0; i<n_div; i++) {
        if (strcmp(vett[i]->sigla, sigla) == 0) return i;
    }
    return -1;
}

void divisione_stampa_stato(divisione_t d) {
    printf("Div: %s Costo Attuale: %.2f\n", d->sigla, divisione_get_costo(d));
}

/* --- 3. LOGICA CORE --- */

void divisione_aggiungi_dipendente(divisione_t d, dipendente_t *dip, int ruolo) {
    /* Aggiorniamo i contatori della divisione */
    d->curr_pers[ruolo]++;
    d->curr_skill[ruolo] += dip->competenze[ruolo];
}

void divisione_rimuovi_dipendente(divisione_t d, dipendente_t *dip, int ruolo) {
    /* Operazione inversa per il backtracking */
    d->curr_pers[ruolo]--;
    d->curr_skill[ruolo] -= dip->competenze[ruolo];
}

int divisione_check_minimi(divisione_t d) {
    /* Verifica se tutti i requisiti MINIMI sono soddisfatti */
    for (int i = 0; i < N_RUOLI; i++) {
        if (d->curr_pers[i] < d->min_pers[i]) return 0; // Troppe poche persone
        if (d->curr_skill[i] < d->min_skill[i]) return 0; // Competenza totale troppo bassa
    }
    return 1; // Tutto ok
}

float divisione_get_costo(divisione_t d) {
    /* Calcola sommatoria |richiesta_opt - competenza_att| 
       Come definito nella formula di Delta_avg (parte interna)
    */
    float costo = 0.0;
    for (int i = 0; i < N_RUOLI; i++) {
        /* fabs è il valore assoluto per float/double */
        costo += fabs((double)(d->opt_skill[i] - d->curr_skill[i]));
    }
    return costo;
}

char* divisione_get_sigla(divisione_t d) {
    return d->sigla;
}