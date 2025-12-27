#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * NOME FUNZIONE: risolviTaglioNastro
 * ======================================================================================
 * SCOPO:
 * Calcola il guadagno massimo ottenibile tagliando un nastro di lunghezza 'n'
 * dato un set di lunghezze permesse e i relativi prezzi.
 * Visualizza anche la strategia di taglio (quali pezzi tagliare).
 *
 * PARAMETRI:
 * - int n: La lunghezza totale del nastro da tagliare.
 * - int m: Il numero di opzioni di taglio disponibili (dimensione degli array).
 * - int *lunghezze: Array contenente le lunghezze dei pezzi vendibili.
 * - int *prezzi: Array contenente i prezzi corrispondenti alle lunghezze.
 *
 * CONCETTI CHIAVE (Programmazione Dinamica - Bottom Up):
 * Invece di provare tutte le combinazioni ricorsivamente (che sarebbe lentissimo),
 * usiamo un array di supporto 'dp'.
 * dp[i] conterrà il massimo guadagno ottenibile per un nastro di lunghezza 'i'.
 * Costruiamo questo array partendo da i=1 fino a i=n.
 *
 * Per ricostruire la soluzione (quali pezzi abbiamo scelto), usiamo un array
 * parallelo 'scelta' che memorizza l'indice del taglio migliore effettuato per ogni lunghezza.
 * ======================================================================================
 */
void risolviTaglioNastro(int n, int m, int *lunghezze, int *prezzi) {

    /* --- 1. ALLOCAZIONE MEMORIA (Strutture Dati di Supporto) --- */
    
    /* * dp: Array per il Valore Ottimo.
     * dp[i] = guadagno massimo per un nastro di lunghezza i.
     * Usiamo calloc per inizializzare tutto a 0 (per lunghezza 0, guadagno è 0).
     * Dimensione: n + 1 (perché vogliamo l'indice n incluso).
     */
    int *dp = (int*)calloc(n + 1, sizeof(int));

    /*
     * scelta: Array per la Ricostruzione della Soluzione.
     * scelta[i] = indice 'j' (riferito agli array lunghezze/prezzi) che rappresenta
     * l'ultimo taglio effettuato per ottenere il massimo guadagno a lunghezza 'i'.
     */
    int *scelta = (int*)malloc((n + 1) * sizeof(int));

    /* Controllo difensivo sulla memoria */
    if (dp == NULL || scelta == NULL) {
        printf("Errore di allocazione memoria.\n");
        exit(1);
    }

    /* Inizializzo l'array scelta a -1 (nessun taglio fatto) */
    for (int i = 0; i <= n; i++) scelta[i] = -1;

    /* --- 2. LOGICA (Programmazione Dinamica) --- */
    
    /* * Ciclo Esterno: 'i' rappresenta la lunghezza del nastro che stiamo risolvendo ORA.
     * Andiamo da 1 fino a n.
     */
    for (int i = 1; i <= n; i++) {
        
        /* Ciclo Interno: Proviamo tutti i possibili tagli 'j' disponibili nel listino */
        for (int j = 0; j < m; j++) {
            
            /* Controllo di validità: Posso fare un taglio di lunghezza[j] 
             * solo se il nastro attuale (i) è abbastanza lungo. */
            if (lunghezze[j] <= i) {
                
                /*
                 * FORMULA FONDAMENTALE:
                 * Il valore potenziale è: Prezzo del taglio 'j' + Guadagno Ottimo del resto del nastro.
                 * Il resto del nastro ha lunghezza (i - lunghezze[j]).
                 * Poiché stiamo andando bottom-up, dp[i - lunghezze[j]] è già stato calcolato!
                 */
                int valorePotenziale = prezzi[j] + dp[i - lunghezze[j]];

                /* Se questo taglio mi dà più soldi di quello che avevo trovato finora per 'i' */
                if (valorePotenziale > dp[i]) {
                    dp[i] = valorePotenziale; // Aggiorno il massimo guadagno
                    scelta[i] = j;            // Mi ricordo CHE taglio ho fatto (indice j)
                }
            }
        }
    }

    /* --- 3. STAMPA DEI RISULTATI --- */
    printf("\n--- Analisi per nastro lungo %d ---\n", n);
    printf("Valore Massimo Ottenibile: %d\n", dp[n]);

    printf("Strategia di taglio: ");
    /* * Ricostruzione (Backtracking):
     * Partiamo dalla lunghezza totale 'n'.
     * Guardiamo in scelta[n] quale taglio abbiamo fatto.
     * Sottraiamo la lunghezza di quel taglio e ripetiamo finché il nastro non è finito (>0).
     */
    int tempN = n;
    int first = 1; // Flag solo per formattazione virgole
    
    while (tempN > 0) {
        int idxTaglio = scelta[tempN];
        
        /* Controllo di sicurezza: se non c'è soluzione valida */
        if (idxTaglio == -1) {
            printf("(Resto di lunghezza %d inutilizzato)", tempN);
            break; 
        }

        if (!first) printf(", ");
        printf("Pezzo %d (val %d)", lunghezze[idxTaglio], prezzi[idxTaglio]);
        
        /* Riduco la lunghezza del nastro della dimensione del pezzo appena stampato */
        tempN = tempN - lunghezze[idxTaglio];
        first = 0;
    }
    printf("\n");

    /* --- 4. GESTIONE MEMORIA (Free) --- */
    /* Importante: liberare gli array di supporto allocati nello Heap */
    free(dp);
    free(scelta);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    /* --- ESEMPIO 1 (Dall'immagine - Diagramma n=4) --- */
    /* * Nell'immagine c'è un esempio grafico con n=4.
     * Ipotizziamo un listino prezzi compatibile con la soluzione (c):
     * Pezzo da 2 vale 5. (Due pezzi da 2 = 10).
     * Pezzo da 1 vale 1. (Quattro pezzi da 1 = 4).
     * Pezzo da 3 vale 6.
     * Pezzo da 4 vale 9.
     */
    int n1 = 4;
    int lunghezze1[] = {1, 2, 3, 4};
    int prezzi1[]    = {1, 5, 6, 9};
    int m1 = 4;

    printf("TEST 1 (Diagramma Immagine n=4)\n");
    risolviTaglioNastro(n1, m1, lunghezze1, prezzi1);

    /* --- ESEMPIO 2 (Dal testo dell'esercizio) --- */
    /* * Testo: m=8, lungh={7,4,8,1,5,2,6,3}, prezzi={17,9,20,1,10,5,17,8}
     * Qui proviamo con n=8 come suggerito implicitamente dall'array lunghezza[8].
     */
    int n2 = 8;
    int lunghezze2[] = {7,  4,  8, 1,  5, 2,  6, 3};
    int prezzi2[]    = {17, 9, 20, 1, 10, 5, 17, 8};
    int m2 = 8; // Numero di opzioni disponibili

    printf("\nTEST 2 (Dati Testuali Esercizio n=8)\n");
    risolviTaglioNastro(n2, m2, lunghezze2, prezzi2);

    /* --- ESEMPIO 3 (Nastro molto lungo) --- */
    /* Verifica stabilità con nastro più lungo */
    int n3 = 10;
    printf("\nTEST 3 (Stessi dati, n=10)\n");
    risolviTaglioNastro(n3, m2, lunghezze2, prezzi2);

    return 0;
}