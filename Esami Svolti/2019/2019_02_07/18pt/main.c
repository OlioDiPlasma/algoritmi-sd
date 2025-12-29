#include "stazioni.h"

int main() {
    // 1. Lettura Dati
    // Carichiamo la mappa dal file di testo
    Mappa m = caricaDati("dati.txt");
    if (m.N == 0) return -1; // Uscita se errore lettura

    printf("Dati caricati: %d comuni.\n", m.N);

    // 2. Verifica Proposta (Requisito 1)
    // Usiamo il file proposta.txt che contiene una possibile soluzione
    // Impostiamo distMax a 8 come nell'esempio del testo
    int distMax = 8;
    int esito = verificaProposta("proposta.txt", m, distMax);
    if (esito) printf("-> La proposta e' VALIDA.\n");
    else printf("-> La proposta NON e' valida.\n");

    // 3. Esecuzione Algoritmo Obiettivo 1
    // Trova il minimo numero di stazioni per coprire tutto entro distMax
    risolviObiettivo1(m, distMax);

    // 4. Esecuzione Algoritmo Obiettivo 2
    // Trova dove mettere esattamente 2 stazioni per minimizzare la formula
    // Usiamo numStaz = 2 come nell'esempio
    int numStazObiettivo2 = 2;
    risolviObiettivo2(m, numStazObiettivo2);

    // 5. Pulizia Memoria
    // Fondamentale in C: ciò che allochi con malloc, devi liberare con free
    liberaMappa(m);

    return 0;
}