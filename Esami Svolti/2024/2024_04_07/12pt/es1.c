#include <stdio.h>
#include <stdlib.h>

// --- LA TUA FUNZIONE ---
int* diffVett(int *a, int na, int *b, int nb, int *nc) {
    int *c = (int*)malloc(na * sizeof(int));
    if (c == NULL) return NULL;

    int k = 0; // Indice per il vettore risultato

    // Ciclo esterno: scorre A
    for (int i = 0; i < na; i++) {
        
        int trovato = 0; // Flag: 0 = non trovato, 1 = trovato
        
        // Ciclo interno: cerca a[i] dentro B
        for (int j = 0; j < nb; j++) {
            if (a[i] == b[j]) {
                trovato = 1;
                break; // Trovato! Smetto di cercare
            }
            // OTTIMIZZAZIONE: vettori ordinati
            if (b[j] > a[i]) {
                break; 
            }
        }

        // Se non l'ho trovato in B, lo aggiungo a C
        if (trovato == 0) {
            c[k] = a[i];
            k++;
        }
    }

    *nc = k; // Salvo la dimensione finale
    
    // Ridimensionamento finale
    if (k > 0) {
         // Nota: qui va bene anche c = realloc... per semplicità
         int *temp = (int*)realloc(c, k * sizeof(int));
         if (temp != NULL) c = temp;
    } else {
         free(c);
         return NULL;
    }
    
    return c;
}

// --- MAIN DI TEST ---
int main() {
    // 1. Definiamo due vettori di prova (ORDINATI e senza ripetizioni interne)
    // Caso di test:
    // A ha: 1, 5, 10, 15, 20
    // B ha: 5, 15, 30
    // Risultato atteso (A - B): 1, 10, 20 (rimuove 5 e 15)
    
    int a[] = {1, 5, 10, 15, 20};
    int b[] = {5, 15, 30};

    // Calcoliamo le dimensioni automaticamente
    int na = sizeof(a) / sizeof(a[0]);
    int nb = sizeof(b) / sizeof(b[0]);
    int nc = 0; // Variabile per ospitare la dimensione del risultato

    printf("Vettore A: ");
    for(int i=0; i<na; i++) printf("%d ", a[i]);
    printf("\n");

    printf("Vettore B: ");
    for(int i=0; i<nb; i++) printf("%d ", b[i]);
    printf("\n--------------------------\n");

    // 2. Chiamata alla funzione
    int *risultato = diffVett(a, na, b, nb, &nc);

    // 3. Stampa del risultato
    printf("Risultato (elementi in A ma non in B): ");
    
    if (risultato == NULL) {
        if (nc == 0) {
            printf("[Vettore vuoto] (Tutti gli elementi di A erano in B)\n");
        } else {
            printf("Errore di allocazione memoria.\n");
        }
    } else {
        // Stampa il vettore generato
        printf("[ ");
        for (int i = 0; i < nc; i++) {
            printf("%d ", risultato[i]);
        }
        printf("]\n");
        printf("Dimensione vettore risultato (nc): %d\n", nc);

        // 4. IMPORTANTE: Liberare la memoria allocata dalla funzione!
        free(risultato);
        printf("\nMemoria liberata correttamente.\n");
    }

    return 0;
}