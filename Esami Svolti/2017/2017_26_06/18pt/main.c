#include "hotel.h"

int main(int argc, char *argv[]) {
    /*
     * Controllo argomenti.
     * argv[0] = nome programma
     * argv[1] = file prenotazioni
     * argv[2] = file allocazione input (da verificare)
     * argv[3] = file output (dove scrivere l'ottimo)
     */
    if (argc != 4) {
        printf("Uso: %s <prenotazioni.txt> <allocazione.txt> <output.txt>\n", argv[0]);
        return 1;
    }

    int N_hotel, X_giorni, a, b;
    int n_clienti;

    /* Lettura dati da standard input (tastiera) come richiesto dal testo */
    /* Esempio input: 
       2 5 10 5
       3 4 
       (dove 2 hotel, 5 giorni, a=10, b=5, hotel0 cap=3, hotel1 cap=4)
    */
    printf("Inserisci N (hotel), X (giorni), a, b: ");
    scanf("%d %d %d %d", &N_hotel, &X_giorni, &a, &b);

    /* Allocazione dinamica strutture Hotel */
    Hotel *hotels = (Hotel *)malloc(N_hotel * sizeof(Hotel));
    
    printf("Inserisci le capacita' M per i %d hotel:\n", N_hotel);
    for (int i = 0; i < N_hotel; i++) {
        hotels[i].id = i;
        scanf("%d", &hotels[i].M);
        /* * Allocazione fondamentale: ogni hotel deve tracciare X giorni.
         * Usiamo calloc per inizializzare tutto a 0 automaticamente.
         */
        hotels[i].camere_occupate_giornaliere = (int *)calloc(X_giorni, sizeof(int));
        hotels[i].totale_camere_occupate_periodo = 0;
    }

    /* 1. Lettura Clienti */
    Cliente *clienti = leggiPrenotazioni(argv[1], &n_clienti);
    printf("Letti %d clienti.\n", n_clienti);

    /* 2. Verifica Allocazione (Primo punto dell'esercizio) */
    printf("--- VERIFICA ALLOCAZIONE PROPOSTA ---\n");
    int esito = verificaAllocazione(argv[2], clienti, n_clienti, hotels, N_hotel, X_giorni);
    if (esito) 
        printf("Il file %s contiene una allocazione VALIDA.\n", argv[2]);
    else 
        printf("Il file %s contiene una allocazione NON VALIDA.\n", argv[2]);

    /* 3. Ricerca Soluzione Ottima (Punto da 18 Punti) */
    printf("\n--- RICERCA OTTIMO ---\n");
    trovaSoluzioneOttima(clienti, n_clienti, hotels, N_hotel, X_giorni, a, b, argv[3]);

    /* Pulizia */
    liberaMemoria(clienti, hotels, N_hotel);
    printf("Fine programma.\n");

    return 0;
}