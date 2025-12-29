/* file: main.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "titoli.h"
#include "quotazioni.h"

int main() {
    /* * Inizializzazione Struttura Dati Principale
     * Usiamo una struttura completamente dinamica (BST di BST)
     */
    TITOLI database = titoli_init();
    
    int scelta = -1;
    while (scelta != 0) {
        printf("\n--- MENU GESTIONE TITOLI AZIONARI ---\n");
        printf("1. Acquisisci file transazioni\n");
        printf("2. Cerca titolo (Verifica esistenza)\n");
        printf("3. Cerca quotazione titolo in data specifica\n");
        printf("4. Min/Max quotazione in intervallo date\n");
        printf("5. Min/Max quotazione assoluta (tutto il periodo)\n");
        printf("0. Esci\n");
        printf("Scelta: ");
        scanf("%d", &scelta);

        /* Variabili di supporto per i case */
        char nome_file[50];
        char codice[21];
        QUOTAZIONI q_found;
        Data d1, d2;
        float min, max, val;

        switch(scelta) {
            case 1:
                printf("Nome file (es: transazioni.txt): ");
                scanf("%s", nome_file);
                titoli_leggi_file(&database, nome_file);
                break;

            case 2:
                /* PUNTO 2: Ricerca Titolo (O(log N)) */
                printf("Codice titolo: ");
                scanf("%s", codice);
                q_found = titoli_cerca(database, codice);
                if (q_found != NULL) 
                    printf("Titolo %s TROVATO nel sistema.\n", codice);
                else 
                    printf("Titolo %s NON trovato.\n", codice);
                break;

            case 3:
                /* PUNTO 3: Ricerca Quotazione (O(log M)) */
                printf("Codice titolo: ");
                scanf("%s", codice);
                q_found = titoli_cerca(database, codice);
                if (q_found == NULL) {
                    printf("Titolo non esistente.\n");
                } else {
                    printf("Inserisci data (aaaa/mm/gg): ");
                    scanf("%d/%d/%d", &d1.anno, &d1.mese, &d1.giorno);
                    val = quotazioni_cerca_data(q_found, d1);
                    if (val < 0) printf("Nessuna quotazione in questa data.\n");
                    else printf("Quotazione in data: %.2f\n", val);
                }
                break;

            case 4:
                /* PUNTO 4: Min/Max Intervallo */
                printf("Codice titolo: ");
                scanf("%s", codice);
                q_found = titoli_cerca(database, codice);
                if (q_found == NULL) {
                    printf("Titolo non esistente.\n");
                } else {
                    printf("Data inizio (aaaa/mm/gg): ");
                    scanf("%d/%d/%d", &d1.anno, &d1.mese, &d1.giorno);
                    printf("Data fine (aaaa/mm/gg): ");
                    scanf("%d/%d/%d", &d2.anno, &d2.mese, &d2.giorno);
                    
                    quotazioni_minmax_intervallo(q_found, d1, d2, &min, &max);
                    if (max == -1.0) printf("Nessuna quotazione nell'intervallo.\n");
                    else printf("Min: %.2f, Max: %.2f\n", min, max);
                }
                break;

            case 5:
                /* PUNTO 5: Min/Max Assoluto */
                /* Usiamo la stessa funzione ma passando un intervallo invertito come flag */
                printf("Codice titolo: ");
                scanf("%s", codice);
                q_found = titoli_cerca(database, codice);
                if (q_found == NULL) {
                    printf("Titolo non esistente.\n");
                } else {
                    /* Passiamo d2 < d1 per indicare "tutto" alla nostra logica */
                    d1.anno = 2000; d2.anno = 1900; 
                    quotazioni_minmax_intervallo(q_found, d1, d2, &min, &max);
                    if (max == -1.0) printf("Nessuna quotazione registrata.\n");
                    else printf("Min Assoluto: %.2f, Max Assoluto: %.2f\n", min, max);
                }
                break;
            
            case 0:
                printf("Uscita e deallocazione memoria...\n");
                titoli_free(database);
                break;
            default:
                printf("Scelta non valida.\n");
        }
    }
    return 0;
}