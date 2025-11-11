#include <stdio.h> // Libreria standard per input/output (printf, scanf)
#define MAX 32     // Definisce una costante 'MAX' con valore 32 (dimensione massima array)
#include <math.h>  // Libreria matematica (non usata in questo codice)
#define DBG 0      // Definisce una costante 'DBG' (Debug) a 0. Usata per la compilazione condizionale.

// Prototipo (dichiarazione) della funzione 'show'
void show(int x[], int l, int r);

// Funzione principale del programma
int main() {
    int i, n;     // 'i' è un contatore per i cicli, 'n' è la dimensione dell'array
    int x[MAX]; // Alloca un array 'x' di 'MAX' (32) interi sullo stack

    // Chiede all'utente la dimensione (n deve essere potenza di 2 per la logica ricorsiva)
    printf("Input n = 2^k: ");
    // Legge il valore intero inserito dall'utente e lo salva nella variabile 'n'
    scanf("%d", &n);

    // Chiede all'utente di inserire gli elementi dell'array
    printf("Input  array \n");
    // Ciclo 'for' che si ripete 'n' volte (da 0 a n-1)
    for(i=0; i<n; i++) {
        printf("x[%d] = ", i); // Stampa il prompt per l'elemento i-esimo
        scanf("%d", &x[i]); // Legge l'intero e lo salva in x[i]
    }

// Questo è un blocco di compilazione condizionale
#if DBG // Controlla se 'DBG' è definito e DIVERSO da 0
    // Poiché 'DBG' è 0, tutto questo blocco di codice
    // viene ignorato dal compilatore e non sarà eseguito.
    printf("The array is: ");
    for(i=0; i<n; i++)
        printf("%d", x[i]);
    printf("\n");
#endif // Fine del blocco condizionale '#if DBG'

    // Chiama la funzione ricorsiva 'show' sull'intero array
    // 'x' è l'array
    // '0' è l'indice sinistro (l) di partenza
    // 'n-1' è l'indice destro (r) di partenza
    show(x, 0, n-1); 
    
    return 0; // Indica al sistema operativo che il programma è terminato con successo
}

/**
 * @brief Funzione ricorsiva "Divide et Impera" che stampa i sotto-array
 * secondo un ordine specifico (Stampa-Sinistra, Ricorri-Sinistra,
 * Stampa-Destra, Ricorri-Destra).
 * @param x L'array di dati
 * @param l Indice sinistro (left) del segmento corrente
 * @param r Indice destro (right) del segmento corrente
 */
void show(int x[], int l, int r) {
    int i, c; // 'i' per il ciclo, 'c' per l'indice centrale (midpoint)

    // --- Caso Base della Ricorsione ---
    // Se 'l' (sinistra) è uguale o maggiore di 'r' (destra),
    // significa che il segmento è vuoto (l > r) o ha 1 solo elemento (l == r).
    // In entrambi i casi, la divisione non è più possibile, quindi ci fermiamo.
    if(l >= r)
        return; // Interrompi questa chiamata ricorsiva

    // --- Passo Ricorsivo (Divide et Impera) ---
    
    // 1. DIVIDE: Calcola l'indice centrale 'c' del segmento [l, r]
    c = (r+l)/2; 

    // 2. ELABORA (Stampa la metà SINISTRA)
    printf("xs = "); // Stampa l'etichetta per la metà sinistra
    // Cicla dall'inizio 'l' fino al centro 'c' (incluso)
    for (i=l; i <= c; i++)
        printf("%d", x[i]); // Stampa l'elemento
    printf("\n"); // Va a capo

    // 3. RICORSIONE (Sulla metà SINISTRA)
    // Chiama se stessa passando il sotto-segmento [l, c]
    show(x, l, c); 

    // 4. ELABORA (Stampa la metà DESTRA)
    printf("xd = "); // Stampa l'etichetta per la metà destra
    // Cicla dal centro+1 ('c+1') fino alla fine 'r' (incluso)
    for (i=c+1; i <= r; i++)
        printf("%d", x[i]); // Stampa l'elemento
    printf("\n"); // Va a capo

    // 5. RICORSIONE (Sulla metà DESTRA)
    // Chiama se stessa passando il sotto-segmento [c+1, r]
    show(x, c+1, r);

    // Ritorna (la funzione è 'void', non restituisce valori)
    return;
}