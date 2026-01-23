/* magazzino.h */
#ifndef MAGAZZINO_H
#define MAGAZZINO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODICE 20

/* --- STRUTTURE DATI --- */

/* Rappresenta la coordinata fisica di un pacco */
typedef struct {
    int id_corridoio;
    int id_scaffale; // Indice dello scaffale nel corridoio
    int id_slot;
} Posizione;

/* Il Pacco: contenuto informativo */
typedef struct Pacco {
    char codice[MAX_CODICE];
    // Qui potresti aggiungere peso, mittente, ecc.
} Pacco;

/* Nodo del BST per la RICERCA LOGARITMICA */
typedef struct BSTNode {
    char codice_pacco[MAX_CODICE]; // Chiave di ricerca
    Posizione pos;                 // Valore associato (dove si trova)
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

/* Lo Scaffale: contiene un array di puntatori a Pacchi */
typedef struct Scaffale {
    char codice_scaffale[MAX_CODICE]; // Codice alfanumerico univoco dello scaffale
    Pacco **slots;  // Doppio puntatore: è un array di puntatori a Pacco
    int K;          // Capacità fissa di slot
    int n_occupati; // Quanti slot sono pieni
} Scaffale;

/* Il Corridoio: collezione di Scaffali */
typedef struct Corridoio {
    int id_numerico;
    Scaffale *scaffali; // Array dinamico di Scaffali
    int n_scaffali;     // Dimensione dell'array
} Corridoio;

/* Il Magazzino: collezione di Corridoi e l'indice BST */
typedef struct Magazzino {
    Corridoio *corridoi; // Array dinamico di Corridoi
    int n_corridoi;      // Dimensione array
    int K_slots;         // K globale (tutti gli scaffali hanno stessi slot)
    BSTNode *indice;     // Radice del BST per la ricerca veloce
} Magazzino;

/* --- PROTOTIPI --- */

/* Gestione Memoria e Init */
Magazzino* initMagazzino(const char* filename);
void liberaMagazzino(Magazzino *m);

/* Operazioni Richieste */
Posizione ricercaPacco(Magazzino *m, char *codice); // O(log N)
int inserimentoPacco(Magazzino *m, char *codice, Posizione pos);
int estrazionePacco(Magazzino *m, Posizione pos); // Rimuove pacco
int spostamentoPacco(Magazzino *m, Posizione da, Posizione a);
int compattazioneScaffali(Magazzino *m, int corr_id, int scaff_id_A, int scaff_id_B);

/* Funzioni di Stampa per debug */
void stampaStatoMagazzino(Magazzino *m);

#endif