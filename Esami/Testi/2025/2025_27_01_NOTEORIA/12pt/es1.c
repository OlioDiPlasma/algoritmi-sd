#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * ======================================================================================
 * FUNZIONE: subSeq
 * ======================================================================================
 * Scopo:
 * Verificare se il vettore 'a' è una "sotto-sequenza" del vettore 'b'.
 *
 * Definizione di Sotto-sequenza:
 * Una sequenza 'a' è sotto-sequenza di 'b' se tutti gli elementi di 'a' appaiono in 'b'
 * mantenendo lo stesso ordine relativo, ma non necessariamente consecutivi.
 * Esempio: {1, 3} è sotto-sequenza di {1, 2, 3, 4}.
 *
 * Parametri:
 * - int a[]: Il vettore "piccolo" (la sequenza da cercare).
 * - int na:  Dimensione di 'a'.
 * - int b[]: Il vettore "grande" (dove cercare).
 * - int nb:  Dimensione di 'b'.
 *
 * Concetti Chiave (Algoritmo a Due Puntatori):
 * - Usiamo due indici paralleli: 'i' per il vettore 'a' e 'j' per il vettore 'b'.
 * - L'indice 'j' scorre sempre in avanti (scansioniamo 'b' una volta sola).
 * - L'indice 'i' avanza SOLO se troviamo una corrispondenza (a[i] == b[j]).
 * - Se 'i' arriva alla fine (i == na), abbiamo trovato tutto.
 *
 * Ritorna:
 * - 1 (Vero) se 'a' è sotto-sequenza di 'b'.
 * - 0 (Falso) altrimenti.
 */
int subSeq(int a[], int na, int b[], int nb) {
    
    int i, j; // Dichiarazione indici fuori dal ciclo (stile C89 compatibile)

    /* --- 1. SCANSIONE PARALLELA (THE TWO POINTER APPROACH) --- */
    // Inizializziamo entrambi gli indici a 0.
    // Condizione di permanenza: 
    // - i < na: Non abbiamo ancora trovato tutti gli elementi di 'a'.
    // - j < nb: Non abbiamo ancora finito di esaminare 'b'.
    for (i = 0, j = 0; i < na && j < nb; j++) {
        
        // Confrontiamo l'elemento corrente di 'a' (obiettivo) con quello di 'b' (candidato)
        // Accesso diretto alla memoria: leggiamo i valori agli indirizzi base + offset.
        if (a[i] == b[j]) {
            // MATCH! Abbiamo trovato l'elemento a[i].
            // Possiamo passare a cercare il prossimo elemento di 'a'.
            i++; 
        }
        
        // NOTA: 'j' incrementa sempre (nel for), perché se non c'è match, 
        // l'elemento b[j] è inutile e lo scartiamo.
    }

    /* --- 2. VERIFICA FINALE --- */
    // Se il ciclo è terminato, può essere per due motivi:
    // A. Abbiamo finito 'a' (i == na) -> SUCCESSO.
    // B. Abbiamo finito 'b' ma non 'a' (j == nb) -> FALLIMENTO.
    
    return (i == na);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Scopo:
 * Creare scenari di test per verificare la correttezza della logica.
 */
int main() {
    // SCENARIO 1: Caso positivo classico
    // Cerchiamo {1, 3, 5} in {0, 1, 2, 3, 4, 5}
    int A1[] = {1, 3, 5};
    int B1[] = {0, 1, 2, 3, 4, 5};
    
    printf("Test 1 (Positivo): ");
    if (subSeq(A1, 3, B1, 6)) {
        printf("OK (Trovato)\n");
    } else {
        printf("ERRORE (Non trovato)\n");
    }

    // SCENARIO 2: Caso negativo (Elementi presenti ma ordine sbagliato)
    // Cerchiamo {1, 5, 3} in {0, 1, 2, 3, 4, 5}
    // Il 5 appare DOPO il 3 in B1, quindi non possiamo formare la sequenza 1->5->3
    int A2[] = {1, 5, 3};
    
    printf("Test 2 (Ordine Errato): ");
    if (subSeq(A2, 3, B1, 6)) {
        printf("ERRORE (Falso positivo)\n");
    } else {
        printf("OK (Correttamente non trovato)\n");
    }

    // SCENARIO 3: Caso negativo (Elemento mancante)
    // Cerchiamo {1, 99} 
    int A3[] = {1, 99};
    
    printf("Test 3 (Elemento assente): ");
    if (subSeq(A3, 2, B1, 6)) {
        printf("ERRORE (Falso positivo)\n");
    } else {
        printf("OK (Non trovato)\n");
    }

    return 0;
}