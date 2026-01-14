#include "majority.h"
#include <stdio.h> // Serve solo se volessimo mettere print di debug, qui non serve ma lo includo per sicurezza

/* -----------------------------------------------------------
 * FUNZIONE AUSILIARIA (Helper)
 * Scopo: Conta quante volte un numero 'target' appare in un array.
 * Non e' ricorsiva, serve per la fase di "verifica" del Divide et Impera.
 * ----------------------------------------------------------- */
int conta_occorrenze(int *a, int N, int target) {
    int count = 0;
    int i; 
    
    // Scansione lineare del sotto-vettore
    for (i = 0; i < N; i++) {
        // Accedo all'i-esimo elemento usando l'aritmetica dei puntatori o l'indice
        if (a[i] == target) {
            count++;
        }
    }
    return count;
}

/* -----------------------------------------------------------
 * FUNZIONE PRINCIPALE: majority
 * Concetto Chiave: DIVIDE ET IMPERA
 * Dividiamo il problema in due meta'.
 * - Troviamo il candidato maggioritario a sinistra.
 * - Troviamo il candidato maggioritario a destra.
 * - Combiniamo i risultati per vedere chi vince nel vettore intero.
 * ----------------------------------------------------------- */
int majority(int *a, int N) {
    
    /* --- 1. CASO BASE (Condizione di arresto) --- */
    // Se il vettore ha 0 elementi, non c'e' maggioritario.
    if (N == 0) return -1;
    
    // Se il vettore ha 1 elemento, quell'elemento e' sicuramente la maggioranza (1 > 0.5).
    if (N == 1) return a[0];

    /* --- 2. DIVIDE (Fase ricorsiva) --- */
    // Calcoliamo la meta'. Divisione intera.
    int mid = N / 2;

    // Chiamata ricorsiva sulla parte SINISTRA (da a[0] a a[mid-1])
    // Passiamo il puntatore 'a' e la dimensione 'mid'.
    int left_major = majority(a, mid);

    // Chiamata ricorsiva sulla parte DESTRA (da a[mid] alla fine)
    // ARITMETICA DEI PUNTATORI: 'a + mid' sposta l'indirizzo di memoria di 'mid' interi in avanti.
    // La nuova dimensione e' 'N - mid'.
    int right_major = majority(a + mid, N - mid);

    /* --- 3. CONQUER (Combinazione dei risultati) --- */
    
    // Caso A: Entrambe le meta' hanno lo stesso elemento maggioritario.
    // Allora e' sicuramente maggioritario anche nell'unione.
    if (left_major == right_major) {
        return left_major;
    }

    // Caso B: I risultati sono diversi (o uno e' -1). 
    // Dobbiamo contare le occorrenze di entrambi i candidati NELL'INTERO vettore attuale (dimensione N)
    // per vedere se qualcuno dei due supera N/2.

    // Verifichiamo il candidato di sinistra (se valido, cioe' diverso da -1)
    if (left_major != -1) {
        int count_left = conta_occorrenze(a, N, left_major);
        if (count_left > N / 2) {
            return left_major;
        }
    }

    // Verifichiamo il candidato di destra (se valido)
    if (right_major != -1) {
        int count_right = conta_occorrenze(a, N, right_major);
        if (count_right > N / 2) {
            return right_major;
        }
    }

    // Se nessuno dei due candidati vince nel vettore unito, non c'e' maggioranza.
    return -1;
}