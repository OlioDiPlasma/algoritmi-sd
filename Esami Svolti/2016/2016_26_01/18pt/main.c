#include "strutture.h"

/*
 * Main
 * Argomenti attesi da riga di comando:
 * argv[0]: nome eseguibile
 * argv[1]: nome file input (es. citta.txt)
 * argv[2]: K (numero di sedi ATO)
 */
int main(int argc, char *argv[]) {
    Citta *elenco_citta = NULL;
    int **matrice_distanze = NULL;
    int n, k;

    /* Verifica argomenti */
    if (argc != 3) {
        printf("Uso corretto: %s <nome_file> <K>\n", argv[0]);
        return 1;
    }

    /* Parsing di K */
    k = atoi(argv[2]);
    if (k <= 0) {
        printf("Errore: K deve essere maggiore di 0.\n");
        return 1;
    }

    /* * 1. LETTURA FILE E GENERAZIONE STRUTTURA DATI 
     * Passo l'indirizzo di 'elenco_citta' (&elenco_citta) perche'
     * la funzione deve allocare la memoria e modificare il puntatore.
     */
    printf("Lettura del file in corso...\n");
    n = leggiFile(argv[1], &elenco_citta);
    printf("Lette %d citta'.\n", n);

    if (k > n) {
        printf("Errore: K non puo' essere maggiore di N.\n");
        free(elenco_citta); // Pulizia prima di uscire
        return 1;
    }

    /* * 2. CALCOLO MATRICE DISTANZE
     * Complessita' O(N^2)
     */
    printf("Generazione matrice distanze...\n");
    matrice_distanze = creaMatriceDistanze(elenco_citta, n);

    /* * 3. ALGORITMO RICORSIVO (OTTIMIZZAZIONE)
     * Trova il sottoinsieme di K citta' che minimizza SD.
     */
    printf("Calcolo della soluzione ottima per K=%d...\n", k);
    trovaSoluzioneOttima(n, k, elenco_citta, matrice_distanze);

    /*
     * --- GESTIONE MEMORIA FINALE ---
     * E' fondamentale liberare tutto cio' che abbiamo allocato con malloc.
     */
    liberaMatrice(matrice_distanze, n);
    free(elenco_citta);

    printf("\nProgramma terminato correttamente.\n");
    return 0;
}