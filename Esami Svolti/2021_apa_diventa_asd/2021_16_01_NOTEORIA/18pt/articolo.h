#ifndef ARTICOLO_H
#define ARTICOLO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- DEFINIZIONE STRUTTURA ---
 * Usiamo una struct "quasi ADT" (campi visibili) come richiesto
 * per l'entità singola.
 */
typedef struct {
    char *titolo;    // Stringa allocata dinamicamente
    char *relatore;  // Stringa allocata dinamicamente
    int slot;        // Durata in slot temporali [cite: 50]
    char *argomento; // Stringa allocata dinamicamente
    int id;          // Utile per debug e indicizzazione
} Articolo;

/* Funzione per leggere un singolo articolo da file */
Articolo leggi_articolo(FILE *fp, int id);

/* Funzione per liberare la memoria delle stringhe */
void free_articolo(Articolo a);

#endif