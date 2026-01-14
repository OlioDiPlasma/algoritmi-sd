/*
 * File: main.c
 * Scopo: Interfaccia utente e test delle funzionalita'.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "anagrafica.h"

int main() {
    Lista anagrafe = NULL; // Lista inizialmente vuota
    int scelta;
    char buffer[100]; // Buffer temporaneo per letture stringhe
    Item tmpItem;
    FILE *fp;

    // Carichiamo subito anag1.txt come esempio iniziale (o test)
    printf("--- Caricamento automatico anag1.txt ---\n");
    fp = fopen("anag1.txt", "r");
    if (fp) {
        while(!feof(fp)) {
            tmpItem = leggiItem(fp);
            if (strcmp(tmpItem.codice, "ERR") != 0)
                inserisciOrdinato(&anagrafe, tmpItem);
        }
        fclose(fp);
    } else {
        printf("Attenzione: anag1.txt non trovato. Crealo!\n");
    }

    do {
        printf("\n=== GESTIONE ANAGRAFICA ===\n");
        printf("1. Inserisci nuovo elemento da tastiera\n");
        printf("2. Importa elementi da file (es. anag2.txt)\n");
        printf("3. Ricerca per codice\n");
        printf("4. Cancella per codice\n");
        printf("5. Cancella ed estrai per intervallo date\n");
        printf("6. Stampa lista su file (output.txt)\n");
        printf("0. Esci\n");
        printf("Scelta: ");
        scanf("%d", &scelta);

        switch(scelta) {
            case 1: {
                printf("Inserisci dati (Codice Nome Cognome gg/mm/aaaa Via Citta CAP):\n");
                // Usiamo stdin per leggere da tastiera
                tmpItem = leggiItem(stdin); 
                inserisciOrdinato(&anagrafe, tmpItem);
                printf("Elemento inserito.\n");
                break;
            }

            case 2: {
                printf("Inserisci nome file (es. anag2.txt): ");
                scanf("%s", buffer);
                fp = fopen(buffer, "r");
                if (fp == NULL) {
                    printf("Errore apertura file.\n");
                } else {
                    int conta = 0;
                    while(1) {
                        tmpItem = leggiItem(fp);
                        if (strcmp(tmpItem.codice, "ERR") == 0) break; // Fine file o errore
                        if (feof(fp)) break;
                        inserisciOrdinato(&anagrafe, tmpItem);
                        conta++;
                    }
                    fclose(fp);
                    printf("Importati %d elementi.\n", conta);
                }
                break;
            }

            case 3: {
                printf("Inserisci codice da cercare: ");
                scanf("%s", buffer);
                Lista trovato = ricercaCodice(anagrafe, buffer);
                if (trovato != NULL) {
                    printf("Trovato:\n");
                    stampaItem(trovato->info, stdout);
                } else {
                    printf("Elemento non presente.\n");
                }
                break;
            }

            case 4: {
                printf("Inserisci codice da cancellare: ");
                scanf("%s", buffer);
                // Passiamo &anagrafe perche' la lista potrebbe cambiare testa
                tmpItem = estraiPerCodice(&anagrafe, buffer);
                if (strlen(tmpItem.codice) > 0) {
                    printf("Elemento rimosso ed estratto:\n");
                    stampaItem(tmpItem, stdout);
                } else {
                    printf("Codice non trovato, nessuna cancellazione.\n");
                }
                break;
            }

            case 5: {
                Data d1, d2;
                printf("Inserisci data inizio (gg/mm/aaaa): ");
                scanf("%d/%d/%d", &d1.g, &d1.m, &d1.a);
                printf("Inserisci data fine (gg/mm/aaaa): ");
                scanf("%d/%d/%d", &d2.g, &d2.m, &d2.a);

                printf("--- Elementi estratti nell'intervallo ---\n");
                int contatore = 0;
                // CICLO CHIAMANTE: continuiamo a chiamare la funzione finche' 
                // ci restituisce qualcosa di valido.
                while(1) {
                    tmpItem = estraiIntervalloDate(&anagrafe, d1, d2);
                    if (strlen(tmpItem.codice) == 0) {
                        // Se torna item vuoto, non c'e' piu' nulla da estrarre
                        break;
                    }
                    stampaItem(tmpItem, stdout);
                    contatore++;
                }
                
                if (contatore == 0) 
                    printf("Nessun elemento trovato in questo intervallo.\n");
                else
                    printf("Totale elementi estratti: %d\n", contatore);
                
                break;
            }

            case 6: {
                stampaListaSuFile(anagrafe, "output.txt");
                break;
            }
        }

    } while (scelta != 0);

    // Pulizia finale della memoria
    liberaLista(anagrafe);
    printf("Memoria liberata. Arrivederci.\n");

    return 0;
}