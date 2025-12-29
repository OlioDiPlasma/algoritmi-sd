#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

/*
    Compilazione (da terminale/VSCode):
    gcc main.c graph.c st.c -o esame -lm
    
    Esecuzione:
    ./esame grafo.txt
*/

int main(int argc, char **argv) {
    /* Controllo argomenti riga di comando.
       argv[0] è il nome del programma, argv[1] è il primo argomento.
    */
    if (argc < 2) {
        printf("Errore: inserire il nome del file grafo.\nUso: %s <file_grafo>\n", argv[0]);
        return 1;
    }

    /* --- 1. CARICAMENTO --- */
    Graph G = GRAPHload(argv[1]);
    if (G == NULL) {
        printf("Errore apertura file grafo o memoria insufficiente.\n");
        return 1;
    }
    printf("Grafo caricato correttamente.\n");

    /* --- 2. VERIFICA SOLUZIONI PROPOSTE --- */
    /* I file sol1.txt e sol2.txt devono esistere nella cartella */
    GRAPHcheckSolutions(G, "sol1.txt", "sol2.txt");

    /* --- 3. OTTIMIZZAZIONE --- */
    GRAPHfindBestConnection(G);

    /* --- 4. PULIZIA MEMORIA --- */
    GRAPHfree(G);

    return 0;
}