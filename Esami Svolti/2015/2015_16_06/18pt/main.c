/*
 * File: main.c
 * Scopo: Testare l'applicazione come richiesto.
 * Compilazione: gcc main.c gestione_sci.c -o ski_app
 * Esecuzione: ./ski_app config.txt
 */

#include "gestione_sci.h"

/* * Funzione ausiliaria per stampare l'albero statistiche (Debug)
 * Visita in ordine (In-Order Traversal) per stampare ID crescenti
 */
void stampa_stats(NodoStatistica *root) {
    if (root == NULL) return;
    stampa_stats(root->left);
    printf("  Sciatore ID: %ld -> %d volte\n", root->cardId, root->contatore);
    stampa_stats(root->right);
}

int main(int argc, char *argv[]) {
    // Controllo argomenti linea di comando
    if (argc < 2) {
        printf("Uso: %s <file_configurazione>\n", argv[0]);
        return 1;
    }

    printf("--- INIZIO PROGRAMMA IMPIANTI SCIISTICI ---\n");

    /* 1. Inizializzazione Strutture */
    NodoSciatore *dbSciatori = NULL; // Radice albero sciatori (vuoto)
    Skilift *listaImpianti = carica_skilift(argv[1]);

    if (listaImpianti == NULL) {
        printf("Errore nel caricamento impianti. Termino.\n");
        return 1;
    }

    printf("Configurazione caricata.\n");

    /* 2. Loop di simulazione Input */
    /* * Il testo dice: "Il sistema riceve da tastiera le letture".
     * Simulo un loop infinito che termina se inserisco "FINE".
     */
    char idImpiantoInput[20];
    long idSciatoreInput;
    int orarioInput;

    printf("Inserisci i dati nel formato: ID_Impianto ID_Sciatore Orario\n");
    printf("Esempio: SeggioviaA 12345 60\n");
    printf("Scrivi 'FINE' al posto dell'ID impianto per terminare e vedere i report.\n\n");

    while (1) {
        printf("> ");
        scanf("%s", idImpiantoInput);

        if (strcmp(idImpiantoInput, "FINE") == 0) {
            break;
        }

        scanf("%ld %d", &idSciatoreInput, &orarioInput);

        // Chiamata alla funzione CORE
        int risultato = authorize(idSciatoreInput, idImpiantoInput, orarioInput, listaImpianti, &dbSciatori);

        if (risultato == 1) {
            printf("[OK] Accesso CONSENTITO.\n");
        } else {
            printf("[NO] Accesso NEGATO (Troppo presto o impianto errato).\n");
        }
    }

    /* 3. Report Finale (Verifica requisiti memoria) */
    printf("\n--- REPORT FINALE STATISTICHE ---\n");
    Skilift *curr = listaImpianti;
    while (curr != NULL) {
        printf("Impianto: %s (Intervallo: %d min)\n", curr->id, curr->intervalloMin);
        printf("Elenco sciatori abilitati:\n");
        stampa_stats(curr->utentiAbilitati); // Stampa BST interno
        printf("-----------------------------\n");
        curr = curr->next;
    }

    /* 4. Pulizia Memoria */
    libera_tutto(listaImpianti, dbSciatori);
    printf("Memoria liberata correttamente. Arrivederci.\n");

    return 0;
}