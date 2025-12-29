#ifndef DIPENDENTI_H
#define DIPENDENTI_H

#include <stdio.h>

/* DEFINIZIONE DEI RUOLI
   Mappiamo gli indici dell'array competenze per leggibilità:
   0 = Operaio, 1 = Amministrativo, 2 = Tecnico, 3 = Informatico
*/
typedef enum { OPERAIO=0, AMMINISTRATIVO=1, TECNICO=2, INFORMATICO=3, N_RUOLI=4 } Ruolo;

/* QUASI-ADT DIPENDENTE 
   Esponiamo la struct perché l'utente (il main) deve poter leggere 
   i dati facilmente (es. nome, competenze) senza troppe funzioni getter.
*/
typedef struct {
    int matricola;
    char nome[21];
    char cognome[21];
    int competenze[N_RUOLI]; // Vettore di 4 interi
} dipendente_t;

/* --- Funzioni --- */

/* Legge un singolo dipendente dal file aperto fp */
int dipendente_leggi(FILE *fp, dipendente_t *d);

/* Stampa i dettagli di un dipendente */
void dipendente_stampa(dipendente_t d);

/* Converte un carattere ('o','a','t','i') nell'indice intero corrispondente */
int char_to_ruolo(char c);

/* Converte un indice intero (0-3) nel carattere corrispondente */
char ruolo_to_char(int r);

#endif