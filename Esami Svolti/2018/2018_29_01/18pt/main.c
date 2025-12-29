#include "defs.h"

/*
 * MAIN DI TEST
 * Uso: ./main <N> <file_persone> <file_aziende> <file_match>
 * Esempio: ./main 4 persone.txt aziende.txt match_input.txt
 */
int main(int argc, char *argv[]) {
    // Controllo argomenti
    if (argc != 5) {
        fprintf(stderr, "Uso corretto: %s <N> <file_persone> <file_aziende> <file_match>\n", argv[0]);
        return 1;
    }

    // 1. Parsing Input
    int N = atoi(argv[1]);
    if (N <= 0) { fprintf(stderr, "N deve essere positivo.\n"); return 1; }
    
    char *file_persone = argv[2];
    char *file_aziende = argv[3];
    char *file_match = argv[4];

    printf("Inizio elaborazione per N=%d\n", N);

    // 2. Inizializzazione Strutture
    ProblemData *pd = inizializza_dati(N);

    // 3. Lettura File
    // Importante: l'ordine di lettura deve gestire il fatto che i nomi servono
    // per convertire le stringhe in indici. La funzione 'leggi_preferenze' gestisce questo.
    leggi_preferenze(pd, file_persone, file_aziende);
    leggi_matching_input(pd, file_match);

    // 4. Verifica del matching fornito (Punto 2 dell'esercizio)
    int esito = verifica_stabilita(pd);
    if (esito) {
        printf(">>> Il matching fornito nel file e' PERFETTO.\n");
    } else {
        printf(">>> Il matching fornito nel file NON e' perfetto (vedi instabilita' sopra).\n");
    }

    // 5. Ricerca della soluzione ottima (Punto 3 dell'esercizio)
    algoritmo_gale_shapley(pd);

    // 6. Pulizia Memoria (Fondamentale per evitare Memory Leaks!)
    libera_tutto(pd);

    return 0;
}