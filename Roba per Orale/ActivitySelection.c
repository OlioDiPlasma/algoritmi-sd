#include <stdio.h>
#include <stdlib.h>

// --- STRUTTURA DATI ---
typedef struct {
    int id;     // Identificativo originale dell'attività
    int start;  // Tempo di inizio
    int finish; // Tempo di fine
} Activity;

// --- FUNZIONE DI CONFRONTO PER QSORT ---
// L'algoritmo Greedy RICHIEDE che le attività siano ordinate per tempo di FINE crescente.
int compareActivities(const void *a, const void *b) {
    Activity *activityA = (Activity *)a;
    Activity *activityB = (Activity *)b;
    return (activityA->finish - activityB->finish);
}

// --- ALGORITMO DI SELEZIONE ATTIVITÀ (GREEDY) ---
void activitySelection(Activity activities[], int n) {
    // 1. Ordinamento (Step fondamentale: O(N log N))
    qsort(activities, n, sizeof(Activity), compareActivities);

    printf("--- Attivita' Selezionate (Massimizzazione Numero) ---\n");

    // 2. La prima attività dell'array ordinato è sempre selezionata
    // (Perché è quella che finisce prima in assoluto, lasciando più spazio alle altre)
    int i = 0; // Indice dell'ultima attività selezionata
    printf("Attivita' %d: [%d, %d]\n", activities[i].id, activities[i].start, activities[i].finish);

    int count = 1; // Contatore attività

    // 3. Scansione delle restanti attività (O(N))
    for (int j = 1; j < n; j++) {
        // Se l'attività corrente (j) inizia DOPO o QUANDO finisce l'ultima selezionata (i)
        if (activities[j].start >= activities[i].finish) {
            printf("Attivita' %d: [%d, %d]\n", activities[j].id, activities[j].start, activities[j].finish);
            i = j; // Aggiorna l'indice dell'ultima attività selezionata
            count++;
        }
    }
    
    printf("\nTotale attivita' selezionate: %d su %d\n", count, n);
}

// --- MAIN DI ESEMPIO ---
int main() {
    // Esempio classico
    // A: 5-9
    // B: 1-2
    // C: 3-4
    // D: 0-6
    // E: 5-7
    // F: 8-9
    
    // Nota: L'array verrà ordinato, quindi l'ordine iniziale non conta.
    Activity arr[] = {
        {1, 5, 9},
        {2, 1, 2},
        {3, 3, 4},
        {4, 0, 6},
        {5, 5, 7},
        {6, 8, 9}
    };
    
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Input: %d attivita' disordinate.\n", n);
    activitySelection(arr, n);

    /* * Analisi dell'Esempio dopo sort per Finish Time:
     * 1. {2, 1, 2} -> Selezionata (Finisce a 2)
     * 2. {3, 3, 4} -> Compatibile (3 >= 2)? SI. Selezionata (Finisce a 4)
     * 3. {4, 0, 6} -> Compatibile (0 >= 4)? NO.
     * 4. {5, 5, 7} -> Compatibile (5 >= 4)? SI. Selezionata (Finisce a 7)
     * 5. {6, 8, 9} -> Compatibile (8 >= 7)? SI. Selezionata (Finisce a 9)
     * 6. {1, 5, 9} -> Compatibile (5 >= 9)? NO.
     * Totale: 4
     */

    return 0;
}