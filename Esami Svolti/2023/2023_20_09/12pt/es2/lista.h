/* ===========================================================================
 * FILE: lista.h
 * DEFINIZIONE ADT
 * =========================================================================== */

/* * LIST è un "Puntatore Opaco". L'utente sa che esiste una LIST, 
 * ma non sa come è fatta dentro (se è un array, una lista, un albero...).
 */

#ifndef LISTA_H
#define LISTA_H

/* TIPO OPACO: Il main sa solo che esiste una LIST, non com'è fatta */
typedef struct list *LIST;

/* Funzione richiesta dall'esame */
void f(LIST l, int k);

/* Funzioni di supporto (necessarie perché il main non può toccare i puntatori interni) */
LIST crea_lista();              // Alloca la struttura
void append(LIST l, char c);    // Aggiunge in coda
void print_list(LIST l);        // Stampa
void free_list(LIST l);         // Libera memoria

#endif