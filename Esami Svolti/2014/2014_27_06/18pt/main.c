#include <stdio.h>
#include <stdlib.h>
#include "automotive.h"

int main(int argc, char *argv[]) {
    /* * VERIFICA INPUT RIGA DI COMANDO
     * Il testo chiede: "ricevere il nome del file produttori sulla riga di comando"
     */
    if (argc != 2) {
        printf("Uso: %s <file_produttori.txt>\n", argv[0]);
        return 1;
    }

    /* --- 1. CARICAMENTO DATI --- */
    printf("--- Caricamento Database ---\n");
    Produttore *database = carica_database(argv[1]);
    if (database == NULL) {
        printf("Errore fatale nel caricamento.\n");
        return 1;
    }
    printf("Database caricato con successo.\n");

    /* --- 2. MENU INTERATTIVO --- */
    int scelta = -1;
    char buffer1[101], buffer2[101];

    while (scelta != 0) {
        printf("\n--- MENU GESTIONE AUTO ---\n");
        printf("1. Cerca Produttore (Mostra Modelli)\n");
        printf("2. Cerca Modello (Mostra Accessori)\n");
        printf("3. Cancella Produttore\n");
        printf("4. Cancella Modello\n");
        printf("5. Cancella Accessorio\n");
        printf("6. Incorpora Produttori (Merge)\n");
        printf("0. Uscita\n");
        printf("Scelta: ");
        scanf("%d", &scelta);

        switch (scelta) {
            case 1:
                printf("Inserisci nome Produttore: ");
                scanf("%s", buffer1);
                stampa_modelli_produttore(database, buffer1);
                break;
            case 2:
                printf("Inserisci nome Modello: ");
                scanf("%s", buffer1);
                stampa_accessori_modello(database, buffer1);
                break;
            case 3:
                printf("Inserisci Produttore da cancellare: ");
                scanf("%s", buffer1);
                database = cancella_produttore(database, buffer1);
                printf("Operazione completata (se esisteva).\n");
                break;
            case 4:
                printf("Inserisci Modello da cancellare: ");
                scanf("%s", buffer1);
                cancella_modello_globale(database, buffer1);
                printf("Operazione completata.\n");
                break;
            case 5:
                printf("Inserisci Accessorio da cancellare: ");
                scanf("%s", buffer1);
                cancella_accessorio_globale(database, buffer1);
                printf("Operazione completata.\n");
                break;
            case 6:
                printf("Produttore PRINCIPALE (che riceve): ");
                scanf("%s", buffer1);
                printf("Produttore DA RIMUOVERE (che viene fuso): ");
                scanf("%s", buffer2);
                database = incorpora_produttori(database, buffer1, buffer2);
                break;
            case 0:
                printf("Uscita e pulizia memoria...\n");
                break;
            default:
                printf("Scelta non valida.\n");
        }
    }

    /* --- 3. PULIZIA MEMORIA --- */
    libera_tutto(database);

    return 0;
}