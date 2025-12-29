/* main.c */
#include "catalogo.h"

int main() {
    Negozio myShop;
    
    /* 1. Inizializzazione */
    printf("=== AVVIO PROGRAMMA ===\n");
    init_negozio(&myShop);

    /* 2. Caricamento Dati (Simuliamo inserimento manuale o da file) */
    /* NOTA: Assicurati di aver creato prodotti.txt! */
    carica_da_file(&myShop, "prodotti.txt");

    printf("\n=== TEST PUNTO 2: Ricerca per Codice ===\n");
    ricerca_prodotto_globale(&myShop, "A003"); // Esiste
    ricerca_prodotto_globale(&myShop, "Z999"); // Non esiste

    printf("\n=== TEST PUNTO 3: Ricerca Codice + Categoria ===\n");
    ricerca_prodotto_mirata(&myShop, "Elettronica", "A001"); // Ok
    ricerca_prodotto_mirata(&myShop, "Cucina", "A001");      // Categoria errata

    printf("\n=== TEST PUNTO 4: Stampa Categoria ===\n");
    printf("[Ordinata per ID]\n");
    stampa_categoria(&myShop, "Elettronica", 0);
    printf("[Ordinata per Nome]\n");
    stampa_categoria(&myShop, "Elettronica", 1);

    printf("\n=== TEST PUNTO 5: Ricerca Parziale ===\n");
    // Cerchiamo tutto cio' che inizia con "Mou*" o "Lap*"
    cerca_per_nome_parziale(&myShop, "Lap*");
    
    printf("\n=== TEST PUNTO 6: Ordine ===\n");
    /* Assicurati di aver creato ordine.txt */
    valuta_ordine(&myShop, "ordine.txt");

    /* Verifica aggiornamento scorte dopo l'ordine */
    printf("\nVerifica scorte post-ordine (A001 doveva scendere di 2):\n");
    ricerca_prodotto_mirata(&myShop, "Elettronica", "A001");

    /* Pulizia */
    libera_tutto(&myShop);
    printf("\nMemoria liberata. Uscita.\n");

    return 0;
}