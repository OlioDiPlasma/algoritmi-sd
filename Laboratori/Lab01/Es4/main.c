#include "trasporti.h"

int main() {
    int n_corse = 0;
    
    /* 1. Vettore PRINCIPALE (i dati veri, nell'ordine del file) */
    Corsa* corse = leggiFile("corse.txt", &n_corse);
    if (corse == NULL) return -1;

    /* 2. ALLOCAZIONE VETTORI DI PUNTATORI (Viste logiche) */
    /* Spiegazione malloc: sizeof(Corsa*) è 4 o 8 byte (puntatore), non l'intera struct */
    Corsa **P_date = malloc(n_corse * sizeof(Corsa*));
    Corsa **P_codice = malloc(n_corse * sizeof(Corsa*));
    Corsa **P_partenza = malloc(n_corse * sizeof(Corsa*));
    Corsa **P_arrivo = malloc(n_corse * sizeof(Corsa*));

    /* 3. INIZIALIZZAZIONE E ORDINAMENTO */
    /* Tutti puntano inizialmente ai dati in ordine 0..N */
    inizializzaPuntatori(P_date, corse, n_corse);
    inizializzaPuntatori(P_codice, corse, n_corse);
    inizializzaPuntatori(P_partenza, corse, n_corse);
    inizializzaPuntatori(P_arrivo, corse, n_corse);

    /* Ordiniamo subito le "viste". I dati in 'corse' NON si muovono! */
    ordinaVettorePuntatori(P_date, n_corse, ORD_DATA);
    ordinaVettorePuntatori(P_codice, n_corse, ORD_CODICE);
    ordinaVettorePuntatori(P_partenza, n_corse, ORD_PARTENZA);
    ordinaVettorePuntatori(P_arrivo, n_corse, ORD_ARRIVO);

    printf("File caricato e 4 ordinamenti generati in memoria.\n");

    comando_e cmd;
    int fine = 0;
    
    /* Per il menu, teniamo traccia di quale ordinamento l'utente vuole 'vedere' */
    /* Default: vediamo quello ordinato per Date */
    Corsa **vista_corrente = P_date; 
    char nome_vista[20] = "Date";

    while (!fine) {
        printf("\n[Vista Attuale: %s]\n", nome_vista);
        cmd = leggiComando();

switch (cmd) {
            case r_stampa:
                /* Stampa usando la vista (lente) attualmente selezionata */
                stampaDaPuntatori(vista_corrente, n_corse);
                break;

            case r_stampa_file:
                /* PROVA DEL NOVE: Stampa il vettore fisico. 
                   Deve restare SEMPRE nell'ordine del file txt! */
                printf("--- Ordine Originale (Fisico) ---\n");
                for(int i=0; i<n_corse; i++) stampaCorsa(&corse[i]);
                break;
            
            /* --- CAMBIO VISTE (Cambio solo la lente, non i dati) --- */
            case r_date:
                vista_corrente = P_date;
                strcpy(nome_vista, "Date");
                printf("Vista commutata su: Date\n");
                break;
            case r_partenza:
                vista_corrente = P_partenza;
                strcpy(nome_vista, "Stazione Partenza");
                printf("Vista commutata su: Partenza\n");
                break;
            case r_capolinea: // O r_arrivo se l'hai chiamato così
                vista_corrente = P_arrivo;
                strcpy(nome_vista, "Stazione Arrivo");
                printf("Vista commutata su: Arrivo\n");
                break;
            case r_ricerca_codice:
                /* La ricerca dicotomica DEVE usare il vettore ordinato per codice */
                /* Nota: passiamo P_codice, non vista_corrente, per sicurezza */
                ricercaDicotomicaSuPuntatori(P_codice, n_corse);
                break;
            
            case r_fine:
                fine = 1;
                break;
            default:
                printf("Comando non valido.\n");
                break;
        }
    }

    /* 4. PULIZIA DELLA MEMORIA (Cruciale!) */
    /* Prima libero le viste (array di puntatori) */
    free(P_date);
    free(P_codice);
    free(P_partenza);
    free(P_arrivo);
    
    /* Poi libero i dati veri */
    free(corse);

    return 0;
}