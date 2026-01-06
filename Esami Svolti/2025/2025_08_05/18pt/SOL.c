#include "SOL.h"

Sol SOLinit(int max_dim) {
    Sol s;
    /* Allocazione per il caso pessimo (tutte le parole prese) */
    s.parole = WORDSinit(max_dim); 
    s.parole.numW = 0; /* All'inizio è vuota */
    s.valore = 0;
    s.valido = FALSO;
    return s;
}

/* Aggiunge una parola in coda (per ricorsione) */
void SOLadd(Sol *s, Word w) {
    int idx = s->parole.numW;
    s->parole.vettW[idx] = w; /* Copia struct (assignment copia i valori statici) */
    s->parole.numW++;
    s->valore += w.valore;
}

/* Rimuove l'ultima parola (backtracking) */
void SOLremove(Sol *s) {
    if (s->parole.numW > 0) {
        int idx = s->parole.numW - 1;
        s->valore -= s->parole.vettW[idx].valore;
        s->parole.numW--;
    }
}

/* Deep copy per salvare la soluzione ottima */
Sol SOLcopy(Sol src) {
    Sol dst;
    int i;
    
    /* Allocazione nuova memoria */
    dst = SOLinit(src.parole.numW); 
    
    /* Copia dati */
    dst.valore = src.valore;
    dst.valido = src.valido;
    dst.parole.numW = src.parole.numW; /* Impostiamo la dim reale */
    
    for(i=0; i<src.parole.numW; i++) {
        dst.parole.vettW[i] = src.parole.vettW[i];
    }
    return dst;
}

void SOLfree(Sol s) {
    WORDSfree(s.parole);
}