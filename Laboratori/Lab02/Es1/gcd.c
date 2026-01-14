/*
 * File: gcd.c
 * Scopo: Implementazione della logica dell'Algoritmo di Stein (Binary GCD)
 */

#include "gcd.h"
#include <stdio.h>
#include <stdlib.h> // Per abs() se necessario, ma qui lavoriamo con naturali

/* --- INIZIO FUNZIONE GCD --- */
/*
 * Funzione: gcd_ricorsivo
 * Concetti Chiave:
 * 1. Ricorsione: La funzione chiama se stessa riducendo la dimensione del problema.
 * 2. Stack frame: Ogni chiamata occupa memoria finche' non si raggiunge il caso base.
 * 3. Aritmetica: Usiamo divisioni per 2 e sottrazioni come richiesto dalla traccia.
 */
int gcd_ricorsivo(int a, int b) {

    /* --- 1. CONDIZIONI DI TERMINAZIONE (CASI BASE) --- */
    // Se i numeri sono uguali, abbiamo trovato il divisore.
    if (a == b) {
        return a;
    }
    // Se uno dei due e' 0, il MCD e' l'altro (assumendo non entrambi 0).
    if (a == 0) return b;
    if (b == 0) return a;

    /* --- 2. GESTIONE DELLA CONDIZIONE a > b --- */
    // La traccia specifica: "Per tener conto di tale condizione, se non soddisfatta, si scambiano a e b."
    // Possiamo farlo semplicemente richiamando la funzione con parametri invertiti.
    if (b > a) {
        return gcd_ricorsivo(b, a);
    }

    /* --- 3. LOGICA RICORSIVA (CASI DEFINITI NELL'ESERCIZIO) --- */
    
    // Controlliamo la parita' usando l'operatore modulo (%).
    // (n % 2 == 0) significa che n e' pari.

    // CASO A: Entrambi pari
    // gcd(a, b) = 2 * gcd(a/2, b/2)
    if ((a % 2 == 0) && (b % 2 == 0)) {
        // Moltiplichiamo per 2 il risultato della chiamata ricorsiva.
        return 2 * gcd_ricorsivo(a / 2, b / 2);
    }

    // CASO B: a dispari, b pari (Dato dal testo)
    // gcd(a, b) = gcd(a, b/2)
    if ((a % 2 != 0) && (b % 2 == 0)) {
        return gcd_ricorsivo(a, b / 2);
    }

    // CASO C: a pari, b dispari (COMPLETAMENTO RICHIESTO DAL PUNTO 1)
    // Questo era il caso mancante nella definizione dell'immagine.
    // E' simmetrico al caso B.
    if ((a % 2 == 0) && (b % 2 != 0)) {
        return gcd_ricorsivo(a / 2, b);
    }

    // CASO D: Entrambi dispari (Dato dal testo)
    // gcd(a, b) = gcd((a-b)/2, b)
    // Nota: Poiche' a e b sono dispari, (a-b) e' necessariamente pari, 
    // quindi la divisione per 2 e' sicura e intera.
    if ((a % 2 != 0) && (b % 2 != 0)) {
        return gcd_ricorsivo((a - b) / 2, b);
    }

    return 0; // Non dovremmo mai arrivare qui
}