#ifndef ATTIVITA_H
#define ATTIVITA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- 1. DEFINIZIONE STRUTTURE DATI --- */
/* Definiamo la struct per la singola attività. 
   La dimensione del nome è 21 (20 caratteri + terminatore '\0') come da specifica[cite: 61].
   Includiamo i vincoli direttamente nella struct per facilitare i controlli.
*/
typedef struct {
    char nome[21];
    int inizio;
    int durata;
    int fine;       // Calcolato come inizio + durata
    int valore;     // Il profitto
    char vincoli[2][21]; // Array di stringhe per memorizzare i nomi delle attività da cui dipende [cite: 62]
    int nVin;       // Numero effettivo di vincoli (0, 1 o 2)
} Attivita;

/* Wrapper per l'ADT (Abstract Data Type) di prima classe.
   Nascondiamo i dettagli implementativi dietro un puntatore.
   Vedi specifica: typedef struct activities *ACT; [cite: 70]
*/
struct activities {
    Attivita *vettAtt; // Vettore dinamico di attività
    int nAtt;          // Numero di attività
};

typedef struct activities *ACT;

/* --- 2. PROTOTIPI DELLE FUNZIONI --- */

/* Legge le attività da file */
ACT activityRead(FILE *f);

/* Verifica se una selezione è valida (compatibilità temporale e precedenze) */
int checkSelection(ACT a, char **selected, int nsel);

/* Trova la selezione ottima che massimizza il profitto */
void bestSelection(ACT a, char ***bestNomi, int *pNsel, int *maxProfitto);

/* Funzione di utilità per liberare la memoria dell'ADT */
void activityFree(ACT a);

#endif