#ifndef SOL_H
#define SOL_H

#include "WORDS.h"

typedef enum { FALSO, VERO } boolean;

/* * Struttura Soluzione
 * Contiene un sottoinsieme delle parole originali.
 */
typedef struct sol_s {
    Words parole; /* Riusiamo la struttura Words per il sottoinsieme */
    int valore;   /* Somma valori */
    boolean valido;
} Sol;

Sol SOLinit(int max_dim);
void SOLadd(Sol *s, Word w);
void SOLremove(Sol *s);
Sol SOLcopy(Sol src);
void SOLfree(Sol s);

#endif