#ifndef DIVISIONI_H
#define DIVISIONI_H

#include <stdio.h>
#include "dipendenti.h"

/* ADT DI PRIMA CLASSE
   L'utente vede solo il puntatore, non i campi interni.
*/
typedef struct divisione_s *divisione_t;

/* --- Gestione Memoria --- */
divisione_t divisione_new();
void divisione_free(divisione_t d);

/* --- I/O --- */
/* Legge una divisione dal file (sigla + 4 righe di requisiti) */
int divisione_leggi(FILE *fp, divisione_t d);
/* Cerca una divisione per sigla in un array di divisioni */
int divisione_cerca(divisione_t *vett, int n_div, char *sigla);
void divisione_stampa_stato(divisione_t d);

/* --- Logica di Business --- */
/* Aggiunge un dipendente alla divisione d nel ruolo specificato */
void divisione_aggiungi_dipendente(divisione_t d, dipendente_t *dip, int ruolo);

/* Rimuove un dipendente (serve per il backtracking) */
void divisione_rimuovi_dipendente(divisione_t d, dipendente_t *dip, int ruolo);

/* Verifica se i vincoli MINIMI sono soddisfatti */
int divisione_check_minimi(divisione_t d);

/* Calcola lo scostamento (costo) della divisione */
float divisione_get_costo(divisione_t d);


char* divisione_get_sigla(divisione_t d);

#endif