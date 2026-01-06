#include "attivita.h"

int main() {
    /* --- 1. APERTURA FILE E LETTURA --- */
    FILE *f = fopen("input.txt", "r");
    if (f == NULL) {
        perror("Errore apertura file");
        return 1;
    }

    printf("--- LETTURA ATTIVITA' ---\n");
    ACT a = activityRead(f);
    fclose(f);

    if (a == NULL) {
        printf("Errore lettura attività.\n");
        return 1;
    }
    printf("Lette %d attivita'.\n", a->nAtt);

    /* --- 2. TEST CHECK SELECTION (Esempio manuale) --- */
    // Proviamo a verificare la soluzione dell'esempio PDF (Act1 e Act4)
    printf("\n--- TEST CHECK SELECTION (Act1, Act4) ---\n");
    char **testSel = malloc(2 * sizeof(char*));
    testSel[0] = "Act1";
    testSel[1] = "Act4";
    if (checkSelection(a, testSel, 2)) {
        printf("Selezione Valida! (Corretto)\n");
    } else {
        printf("Selezione Invalida! (Errore)\n");
    }
    free(testSel); // Libero solo il vettore di puntatori, non le stringhe literal

    /* --- 3. RICERCA OTTIMA --- */
    printf("\n--- RICERCA BEST SELECTION ---\n");
    char **bestNomi = NULL; // Questo verrà allocato dentro la funzione
    int nSel = 0;
    int maxProfit = 0;

    /* Passiamo l'indirizzo di bestNomi (&bestNomi) perché la funzione deve
       modificare il puntatore stesso (allocando memoria e facendolo puntare lì).
       Ecco perché nel prototipo abbiamo char ***bestNomi (triplo puntatore!).
    */
    bestSelection(a, &bestNomi, &nSel, &maxProfit);

    printf("Massimo Profitto Trovato: %d\n", maxProfit);
    printf("Attivita' Selezionate (%d):\n", nSel);
    for (int i = 0; i < nSel; i++) {
        printf("- %s\n", bestNomi[i]);
    }

    /* --- 4. CLEANUP MEMORIA --- */
    // Libero la matrice di stringhe restituita da bestSelection
    for (int i = 0; i < a->nAtt; i++) {
        free(bestNomi[i]);
    }
    free(bestNomi);
    
    activityFree(a);

    return 0;
}