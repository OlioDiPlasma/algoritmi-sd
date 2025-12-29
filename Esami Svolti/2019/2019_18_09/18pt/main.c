/*
 * FILE: main.c
 * SCOPO: Driver code per testare il programma.
 */

#include "progetto.h"

int main() {
    // 1. Lettura Mappa
    Mappa *mappa = leggiMappa("mappa.txt");
    if (!mappa) { printf("Errore apertura mappa.txt\n"); return 1; }
    
    printf("Mappa caricata: %d x %d\n", mappa->nr, mappa->nc);

    // 2. Lettura e Verifica Proposta
    Soluzione *proposta = leggiProposta("proposta.txt", mappa->nr, mappa->nc);
    if (!proposta) { printf("Errore apertura proposta.txt\n"); return 1; }

    int esito = verificaAmmissibilita(mappa, proposta);
    
    if (!esito) {
        // Se fallisce, proviamo a generare corretto.txt
        // Nota: Qui invoco la ricerca per le STESSE risorse (numero Z)
        generaCorretto(mappa, proposta);
    }

    // 3. Ottimizzazione (Bonus Didattico)
    // Cerchiamo la soluzione ottima con ESATTAMENTE Z risorse (che è quello che fa generaCorretto)
    // E poi potremmo cercare quella con il MINOR numero di risorse iterando Z da 1 a proposta->n_risorse
    
    // Pulizia finale
    liberaSoluzione(proposta); // Attenzione: liberaSoluzione in questo esempio semplificato non libera la matrice interna se allocata nel main, ma qui è allocata dentro leggiProposta, quindi va bene implementarla robusta nel .c
    liberaMappa(mappa);

    printf("\nProgramma terminato. Memoria liberata.\n");
    return 0;
}