#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ============================================================================
 * FUNZIONI AUSILIARIE (Richieste dall'esercizio)
 * ============================================================================
 */

// Controlla se abbiamo raggiunto o superato la metà della somma totale.
// Se sum >= goal, fermiamo questo ramo di ricorsione perché aggiungere
// altri numeri positivi ci allontanerebbe solo di più dall'obiettivo.
int checkTerminal(int n, int start, int sum, int goal) {
    return sum >= goal;
}

// Verifica se la soluzione corrente è migliore della migliore trovata finora.
void checkBestSol(int n, int *sol, int *bestSol, int sum, int *bestSumP, int goal) {
    // Calcoliamo la distanza dall'obiettivo (S/2)
    int currentDiff = abs(sum - goal);
    int bestDiff = abs(*bestSumP - goal);

    // Se la nuova somma è più vicina al goal, aggiorniamo
    if (currentDiff < bestDiff) {
        *bestSumP = sum;
        // Copiamo il vettore delle soluzioni (maschera di bit)
        for (int i = 0; i < n; i++) {
            bestSol[i] = sol[i];
        }
    }
}

// Costruisce i vettori finali V1 e V2 basandosi sulla maschera bestSol
int buildSol(int *V, int N, int *bestSol, int **V1p, int **V2p) {
    int n1 = 0;
    int i1 = 0, i2 = 0;

    // 1. Contiamo quanti elementi vanno nel primo vettore
    for (int i = 0; i < N; i++) {
        if (bestSol[i]) n1++;
    }

    // 2. Allocazione dinamica dei vettori risultato
    // Dereferenziamo V1p e V2p per modificare i puntatori nel chiamante
    *V1p = (int *)malloc(n1 * sizeof(int));
    *V2p = (int *)malloc((N - n1) * sizeof(int));

    // 3. Distribuzione dei valori
    for (int i = 0; i < N; i++) {
        if (bestSol[i]) {
            (*V1p)[i1++] = V[i]; // Se bestSol[i] è 1, va in V1
        } else {
            (*V2p)[i2++] = V[i]; // Se bestSol[i] è 0, va in V2
        }
    }

    return n1; // Ritorniamo la dimensione di V1
}

/* ============================================================================
 * FUNZIONE RICORSIVA (CORE)
 * ============================================================================
 */
void bestSolR(int *val, int *sol, int *bestSol, int sum, int *bestSumP, int goal, int n, int start) {
    int i;

    // Caso Base / Terminale: Abbiamo raggiunto o superato il goal?
    if (checkTerminal(n, start, sum, goal)) {
        checkBestSol(n, sol, bestSol, sum, bestSumP, goal);
        return;
    }

    // A ogni passo controlliamo se la soluzione corrente (anche parziale) è comunque ottima
    // Nota: questo controllo extra è utile perché il checkTerminal scatta solo se superiamo il goal.
    // Se finiamo i numeri senza superare il goal, dobbiamo comunque salvare il risultato.
    checkBestSol(n, sol, bestSol, sum, bestSumP, goal);

    // Ciclo per il Power Set (Combinazioni Semplici)
    // <1> Sostituito con 'start' per evitare permutazioni e duplicati
    for (i = start; i < n; i++) {
        
        sol[i] = 1; // Prendo l'elemento i-esimo

        // Chiamata Ricorsiva
        // <2> Parametri aggiornati: sum + val[i] e start diventa i + 1
        bestSolR(val, sol, bestSol, sum + val[i], bestSumP, goal, n, i + 1);

        sol[i] = 0; // Backtracking: non prendo l'elemento i-esimo
    }
}

/* ============================================================================
 * FUNZIONE WRAPPER (PRINCIPALE)
 * ============================================================================
 */
// <3> Firma completata come richiesto
int bestSplit(int *V, int N, int **V1p, int **V2p) {
    // Allocazione vettori di supporto (maschere di bit)
    int *sol = (int *)calloc(N, sizeof(int));
    int *bestSol = (int *)calloc(N, sizeof(int));
    
    int bestSum = 0;
    int goal = 0;
    int n1;

    // Calcolo Somma Totale
    for (int i = 0; i < N; i++) {
        goal += V[i];
    }
    
    // Il goal è la metà esatta della somma
    goal /= 2;

    // Avvio della ricorsione
    bestSolR(V, sol, bestSol, 0, &bestSum, goal, N, 0);

    // Costruzione dei vettori risultato
    n1 = buildSol(V, N, bestSol, V1p, V2p);

    // Pulizia memoria ausiliaria
    free(sol);
    free(bestSol);

    return n1;
}

/* ============================================================================
 * MAIN DI PROVA
 * ============================================================================
 */
int main() {
    // Esempio: Vettore disordinato
    int V[] = {1, 5, 10, 6}; 
    int N = 4;
    
    // Somma Totale = 22. Goal = 11.
    // Miglior split ideale: {5, 6} (Somma 11) vs {1, 10} (Somma 11) -> Diff 0
    // Oppure {10, 1} (Somma 11) etc.
    
    int *V1, *V2; // Puntatori che verranno allocati dentro bestSplit
    int N1, N2;

    printf("Vettore originale: ");
    for(int i=0; i<N; i++) printf("%d ", V[i]);
    printf("\n----------------------------\n");

    // Chiamata alla funzione
    N1 = bestSplit(V, N, &V1, &V2);
    N2 = N - N1;

    // Calcolo somme per verifica
    int s1 = 0, s2 = 0;
    
    printf("Vettore V1 (size %d): [ ", N1);
    for (int i = 0; i < N1; i++) {
        printf("%d ", V1[i]);
        s1 += V1[i];
    }
    printf("] -> Somma S1: %d\n", s1);

    printf("Vettore V2 (size %d): [ ", N2);
    for (int i = 0; i < N2; i++) {
        printf("%d ", V2[i]);
        s2 += V2[i];
    }
    printf("] -> Somma S2: %d\n", s2);

    printf("Differenza |S1 - S2| = %d\n", abs(s1 - s2));

    // Pulizia finale
    free(V1);
    free(V2);

    return 0;
}