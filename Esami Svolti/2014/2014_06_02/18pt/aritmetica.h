/*
 * FILE: aritmetica.h
 * SCOPO: Definizione delle costanti e prototipi delle funzioni per
 * risolvere il problema di aritmetica verbale.
 */

#ifndef ARITMETICA_H
#define ARITMETICA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* --- DEFINIZIONI --- */
#define MAX_STR 9  /* Lunghezza massima 8 + 1 terminatore '\0' */
#define NUM_CIFRE 10
#define NUM_LETTERE 26

/* --- STRUTTURA DATI --- */
/* * Non e' strettamente necessaria una struct complessa per i 18 punti,
 * ma usiamo typedef per chiarezza se necessario.
 * Qui useremo array semplici per didattica diretta.
 */

/* --- PROTOTIPI DI FUNZIONE --- */

/* * Funzione: risolvi_verificatore
 * Scopo: Modalita' 1. Legge una mappatura utente e controlla se e' corretta.
 */
void risolvi_verificatore(char *s1, char *s2, char *s3);

/* * Funzione: risolvi_generatore
 * Scopo: Modalita' 2. Trova automaticamente la soluzione con backtracking.
 */
void risolvi_generatore(char *s1, char *s2, char *s3);

#endif