#include <stdio.h>
#include <math.h>

/* * Funzione di utilità per contare il numero di cifre
 * Necessaria per calcolare 'n' e dividere il numero.
 */
int getLength(long long value) {
    int counter = 0;
    if (value == 0) return 1;
    while (value != 0) {
        counter++;
        value /= 10;
    }
    return counter;
}

/*
 * ALGORITMO DI KARATSUBA
 * Input: x, y (i due numeri da moltiplicare)
 * Output: il prodotto x * y
 */
long long karatsuba(long long x, long long y) {
    // 1. CASO BASE
    // Se i numeri sono piccoli (una sola cifra), la moltiplicazione è diretta.
    // Questo ferma la ricorsione.
    if (x < 10 || y < 10) {
        return x * y;
    }

    // 2. PREPARAZIONE (DIVIDE)
    // Calcoliamo la lunghezza massima dei due numeri
    int n = fmax(getLength(x), getLength(y));
    
    // Calcoliamo la metà della lunghezza per lo split
    int m = n / 2;

    // Calcoliamo il moltiplicatore per dividere i numeri (pow(10, m))
    // Nota: usiamo long long per evitare overflow nel calcolo delle potenze
    long long multiplier = (long long)pow(10, m);

    // 3. SPLITTING (Divisione dei numeri)
    // x = a * 10^m + b
    long long b = x % multiplier; // Parte bassa di x
    long long a = x / multiplier; // Parte alta di x

    // y = c * 10^m + d
    long long d = y % multiplier; // Parte bassa di y
    long long c = y / multiplier; // Parte alta di y

    // 4. CHIAMATE RICORSIVE (IMPERA)
    // Qui avviene la magia: SOLO 3 chiamate invece di 4
    
    // Passo 1: a * c
    long long z0 = karatsuba(a, c); 
    
    // Passo 2: b * d
    long long z1 = karatsuba(b, d); 
    
    // Passo 3: (a + b) * (c + d)
    long long z2 = karatsuba(a + b, c + d); 

    // 5. COMBINAZIONE (COMBINE)
    // Formula: z0 * 10^(2m) + (z2 - z0 - z1) * 10^m + z1
    // (z2 - z0 - z1) corrisponde al termine centrale (ad + bc)
    
    return (z0 * (long long)pow(10, 2 * m)) + 
           ((z2 - z0 - z1) * multiplier) + 
           z1;
}

int main() {
    printf("--- Algoritmo di Karatsuba (Simulazione) ---\n");

    long long x = 12345;
    long long y = 6789;

    printf("Moltiplicando %lld x %lld\n", x, y);
    
    long long result = karatsuba(x, y);
    long long expected = x * y;

    printf("Risultato Karatsuba: %lld\n", result);
    printf("Verifica (x*y):      %lld\n", expected);

    if (result == expected) {
        printf("Test Superato! I risultati coincidono.\n");
    } else {
        printf("Errore: I risultati non coincidono.\n");
    }

    return 0;
}