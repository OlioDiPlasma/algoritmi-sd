/* * File: gestione_sci.h
 * Descrizione: Definizioni delle strutture dati e prototipi.
 */

#ifndef GESTIONE_SCI_H
#define GESTIONE_SCI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- DEFINIZIONE STRUTTURE --- */

/* * 1. StoriaUtilizzo (Lista)
 * Dove sta: Dentro ogni sciatore.
 * Scopo: Memorizza quali impianti ha usato lo sciatore e quando.
 * Struttura: Lista concatenata semplice (gli impianti sono pochi).
 */
typedef struct StoriaUtilizzo {
    char skiliftId[11];         // ID impianto (10 char + terminatore)
    int lastTime;               // Ultimo orario di utilizzo
    struct StoriaUtilizzo *next;
} StoriaUtilizzo;

/* * 2. NodoSciatore (BST)
 * Dove sta: Memoria centrale globale (database sciatori).
 * Scopo: Permette ricerca logaritmica O(log N) dello sciatore per ID.
 */
typedef struct NodoSciatore {
    long cardId;                // Chiave di ricerca
    StoriaUtilizzo *storia;     // Lista degli impianti usati da questo tizio
    struct NodoSciatore *left;
    struct NodoSciatore *right;
} NodoSciatore;

/* * 3. NodoStatistica (BST)
 * Dove sta: Dentro ogni Skilift.
 * Scopo: Conta quante volte uno specifico sciatore ha usato QUESTO impianto.
 * Deve essere un BST per garantire inserimento/aggiornamento in O(log N).
 */
typedef struct NodoStatistica {
    long cardId;                // ID sciatore
    int contatore;              // Numero volte abilitato
    struct NodoStatistica *left;
    struct NodoStatistica *right;
} NodoStatistica;

/* * 4. Skilift (Lista)
 * Dove sta: Memoria centrale globale (elenco impianti).
 * Scopo: Contiene la configurazione letta da file.
 */
typedef struct Skilift {
    char id[11];
    int intervalloMin;          // Tempo minimo di attesa
    NodoStatistica *utentiAbilitati; // BST degli utenti di questo impianto
    struct Skilift *next;
} Skilift;


/* --- PROTOTIPI FUNZIONI --- */

/* Carica gli skilift dal file di testo in una lista */
Skilift* carica_skilift(const char *nomeFile);

/* Funzione principale richiesta dal testo */
int authorize(long cardId, char *skiliftId, int time, Skilift *listaImpianti, NodoSciatore **alberoSciatori);

/* Funzioni di utilita' per la memoria (pulizia) */
void libera_tutto(Skilift *listaImpianti, NodoSciatore *alberoSciatori);

#endif