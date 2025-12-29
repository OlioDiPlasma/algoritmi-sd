#ifndef DEFINES_H
#define DEFINES_H

/* --- DEFINIZIONI COSTANTI --- */
#define NERO 1
#define BIANCO 0

// Definiamo le direzioni per comodità negli array di spiazzamento
// 0: Nord, 1: Est, 2: Sud, 3: Ovest
typedef enum {
    NORD = 0,
    EST = 1,
    SUD = 2,
    OVEST = 3,
    NESSUNA = -1
} Direzione;

/* --- STRUTTURA GRIGLIA --- */
/* Raggruppiamo i dati della griglia per passarli facilmente alle funzioni */
typedef struct {
    int **celle;    // Doppio puntatore per la matrice dinamica
    int nr, nc;     // Numero righe e colonne
    int celle_bianche_totali; // Quante celle dobbiamo visitare per vincere
} Griglia;

#endif