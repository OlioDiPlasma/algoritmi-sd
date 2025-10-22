#include <stdio.h>

void countdown(int n) {
    /*
     * Paragrafo 2: La Condizione di Terminazione (o Caso Base)
     * Se n è 0 o meno, non facciamo nulla e la funzione termina.
     * Questo impedisce una ricorsione infinita.
     */
    if (n <= 0) {
        return; // Stop
    }

    /*
     * Azione: Stampa il numero corrente
     */
    printf("%d\n", n);

    /*
     * Paragrafo 1: La Chiamata Ricorsiva
     * La funzione chiama sé stessa, ma con un problema "più semplice"
     * (n-1 invece di n).
     */
    countdown(n - 1);
}

int main() {
    printf("Conto alla rovescia da 3:\n");
    countdown(3);
    return 0;
}