/*
 * FILE: main.c
 * SCOPO: Punto di ingresso. Gestione I/O e menu.
 */

#include "aritmetica.h"

int main() {
    /* * --- 1. ALLOCAZIONE VARIABILI ---
     * Usiamo array statici perche' la dimensione max e' nota e piccola (8 caratteri).
     * Malloc non e' strettamente necessaria per stringhe cosi' corte, 
     * ma se volessimo essere dinamici useremmo:
     * char *s1 = (char*)malloc(MAX_STR * sizeof(char));
     */
    char s1[MAX_STR], s2[MAX_STR], s3[MAX_STR];
    int scelta;

    /* --- 2. LETTURA INPUT (DA TASTIERA) --- */
    /* * Il testo chiede lettura da tastiera. 
     * Se usi input.txt, lancia con "./main < input.txt"
     */
    printf("Inserisci la prima stringa (max 8 char): ");
    scanf("%s", s1);
    printf("Inserisci la seconda stringa (max 8 char): ");
    scanf("%s", s2);
    printf("Inserisci la terza stringa (somma): ");
    scanf("%s", s3);

    /* Normalizzazione in maiuscolo (sicurezza) */
    for(int i=0; s1[i]; i++) s1[i] = toupper(s1[i]);
    for(int i=0; s2[i]; i++) s2[i] = toupper(s2[i]);
    for(int i=0; s3[i]; i++) s3[i] = toupper(s3[i]);

    /* --- 3. MENU SCELTA --- */
    printf("\nScegli modalita':\n");
    printf("1. Verificatore (tu dai i numeri, io controllo)\n");
    printf("2. Generatore (io calcolo la soluzione)\n");
    printf("Scelta: ");
    scanf("%d", &scelta);

    /* --- 4. CHIAMATA FUNZIONI --- */
    switch(scelta) {
        case 1:
            risolvi_verificatore(s1, s2, s3);
            break;
        case 2:
            risolvi_generatore(s1, s2, s3);
            break;
        default:
            printf("Scelta non valida.\n");
    }

    return 0;
}