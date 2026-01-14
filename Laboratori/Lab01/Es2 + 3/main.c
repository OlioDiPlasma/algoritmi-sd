#include "trasporti.h"

int main() {
    /* Variabili del main 
       n_corse: conterrà il numero reale di elementi letti.
       corse: Puntatore al vettore dinamico (nello Heap).
    */
    int n_corse = 0;
    Corsa* corse = NULL;
    comando_e cmd;
    int fine = 0;
    
    /* Gestione Memoria: Caricamento iniziale
       Se fallisce, terminiamo subito.
    */
    corse = leggiFile("corse.txt", &n_corse);
    if (corse == NULL) return -1;

    printf("File caricato correttamente. %d corse in memoria.\n", n_corse);

    /* Ciclo principale del menu */
    while (!fine) {
        cmd = leggiComando();

        switch (cmd) {
            case r_date:
                elencaPerDate(corse, n_corse);
                break;
            case r_partenza:
                elencaPerPartenza(corse, n_corse);
                break;
            case r_capolinea:
                elencaPerCapolinea(corse, n_corse);
                break;
            case r_ritardo:
                elencaRitardiInDate(corse, n_corse);
                break;
            case r_ritardo_tot:
                ritardoTotaleCodice(corse, n_corse);
                break;
            
            /* --- ESERCIZIO 3: Ordinamenti --- */
            case r_ordinamento_data:
                ordinaDataOra(corse, n_corse);
                break;
            case r_ordinamento_codice:
                ordinaCodice(corse, n_corse);
                break;
            case r_ordinamento_staz_part:
                ordinaPartenza(corse, n_corse);
                break;
            case r_ordinamento_staz_arr:
                ordinaArrivo(corse, n_corse);
                break;

            /* --- ESERCIZIO 3: Ricerche --- */
            case r_ricerca_codice:
                /* NOTA: La ricerca dicotomica richiede che l'array sia ordinato
                   per la chiave di ricerca. Per robustezza, lo ordiniamo qui
                   prima di cercare, o avvisiamo l'utente. Qui forziamo l'ordinamento.
                */
                printf("[Info] Riordino per codice prima della ricerca...\n");
                ordinaCodice(corse, n_corse); 
                ricercaDicotomicaCodice(corse, n_corse);
                break;
            case r_ricerca_partenza:
                ricercaLinearePartenza(corse, n_corse);
                break;

            case r_stampa:
                stampaTutto(corse, n_corse);
                break;
            case r_fine:
                fine = 1;
                break;
            case r_err:
            default:
                printf("Comando non valido.\n");
                break;
        }
    }

    /* Gestione Memoria: PULIZIA FINALE
       Fondamentale in C. Ogni malloc deve avere una free corrispondente.
       Se non lo fai -> Memory Leak.
    */
    free(corse);
    printf("Memoria liberata. Arrivederci.\n");

    return 0;
}