#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Serve per INT_MAX

/*
 * ======================================================================================
 * NOME FUNZIONE: cercaPercorsoRicorsivo
 * ======================================================================================
 * SCOPO:
 * Questa è la funzione "operaia" (helper) che esegue il lavoro sporco.
 * Esplora ricorsivamente tutti i salti possibili da una certa piattaforma per trovare
 * la strada verso la fine. Se trova una strada più breve di quella conosciuta finora,
 * aggiorna il record.
 *
 * PARAMETRI:
 * - piattaforme: (int *) L'array con le lunghezze dei salti massimi.
 * - P:           (int)   Numero totale di piattaforme (dimensione array).
 * - posCorrente: (int)   L'indice della piattaforma su cui ci troviamo ora.
 * - pathCorrente:(int *) Array che tiene traccia dei salti fatti in QUESTO tentativo.
 * - saltiAttuali:(int)   Quanto siamo profondi nella ricorsione (numero di salti fatti finora).
 * - bestPath:    (int *) Array dove salviamo la soluzione migliore trovata finora.
 * - minSalti:    (int *) Puntatore a un intero che conserva il record "minimo salti" globale.
 * Usiamo un puntatore per poter modificare il valore tra le chiamate.
 *
 * CONCETTI CHIAVE:
 * 1. Backtracking e Potatura (Pruning):
 * Prima di provare a saltare, controlliamo: "Ho già fatto più salti del record migliore
 * che ho trovato 5 minuti fa?". Se sì, mi fermo subito (return). È inutile continuare
 * su una strada che è già peggiore di una soluzione che ho già in tasca.
 *
 * 2. Passaggio per Riferimento (int *minSalti):
 * La variabile 'minSalti' deve essere condivisa tra tutte le centinaia di chiamate
 * ricorsive. Se la passassimo per valore, ogni funzione avrebbe la sua copia e non
 * saprebbe se un'altra "versione di se stessa" ha trovato una strada migliore.
 * ======================================================================================
 */
void cercaPercorsoRicorsivo(int *piattaforme, int P, int posCorrente, 
                            int *pathCorrente, int saltiAttuali, 
                            int *bestPath, int *minSalti) {

    /* --- 1. MEMORIZZAZIONE PASSO CORRENTE --- */
    // Salviamo l'indice della piattaforma attuale nel percorso che stiamo costruendo.
    pathCorrente[saltiAttuali] = posCorrente;

    /* --- 2. CONTROLLO POTATURA (PRUNING) --- */
    // Se stiamo facendo un percorso più lungo di uno già trovato ottimale, ci fermiamo.
    // Questo velocizza enormemente l'esecuzione.
    if (saltiAttuali >= *minSalti) {
        return; 
    }

    /* --- 3. CASO BASE: ARRIVO A DESTINAZIONE --- */
    // Siamo arrivati all'ultima piattaforma (indice P-1)?
    if (posCorrente == P - 1) {
        // Se siamo qui, grazie al controllo al punto 2, sappiamo per certo che
        // saltiAttuali è MINORE di *minSalti. Aggiorniamo il record!
        *minSalti = saltiAttuali;

        // Copiamo il percorso attuale dentro bestPath per non perderlo.
        for (int i = 0; i <= saltiAttuali; i++) {
            bestPath[i] = pathCorrente[i];
        }
        return;
    }

    /* --- 4. PASSO RICORSIVO (TRY ALL JUMPS) --- */
    // Leggiamo quanto possiamo saltare al massimo da qui.
    int maxSalto = piattaforme[posCorrente];

    // Proviamo tutti i salti possibili: da 1 passo fino a maxSalto.
    // NOTA LOGICA: Conviene provare prima i salti LUNGHI? Spesso sì, per arrivare prima.
    // Ma l'algoritmo funziona comunque iterando da 1 a maxSalto.
    for (int salto = 1; salto <= maxSalto; salto++) {
        
        int prossimaPos = posCorrente + salto;

        // Controllo Bounds: non saltiamo fuori dall'array!
        if (prossimaPos < P) {
            // Chiamata ricorsiva: ci spostiamo sulla prossima piattaforma
            // e incrementiamo il contatore dei salti (saltiAttuali + 1).
            cercaPercorsoRicorsivo(piattaforme, P, prossimaPos, 
                                   pathCorrente, saltiAttuali + 1, 
                                   bestPath, minSalti);
        }
    }
}

/*
 * ======================================================================================
 * NOME FUNZIONE: trovaSequenzaOttima
 * ======================================================================================
 * SCOPO:
 * Funzione Wrapper (involucro). Prepara la memoria, inizializza le variabili e lancia
 * la ricorsione. Alla fine stampa il risultato.
 *
 * PARAMETRI:
 * - piattaforme: Array di input.
 * - P:           Dimensione array.
 *
 * GESTIONE MEMORIA:
 * Qui allochiamo due array temporanei ('pathCorrente' e 'bestPath') che servono
 * per appuntarci la strada durante i calcoli. È fondamentale liberarli (free) prima di uscire.
 * ======================================================================================
 */
void trovaSequenzaOttima(int *piattaforme, int P) {
    /* --- 1. ALLOCAZIONE MEMORIA DI SUPPORTO --- */
    // Allocazione nello Heap.
    // Dimensione P: nel caso peggiore facciamo saltini da 1 (visitiamo tutte le piattaforme).
    int *pathCorrente = (int *)malloc(P * sizeof(int));
    int *bestPath = (int *)malloc(P * sizeof(int));

    // Controllo malloc
    if (pathCorrente == NULL || bestPath == NULL) {
        printf("Errore malloc\n");
        exit(1);
    }

    /* --- 2. INIZIALIZZAZIONE VARIABILI --- */
    // Impostiamo il "record da battere" a un numero altissimo (infinito).
    // INT_MAX è una costante definita in limits.h
    int minSalti = INT_MAX;

    /* --- 3. LANCIO DELLA RICORSIONE --- */
    // Partiamo dalla posizione 0, con 0 salti effettuati.
    cercaPercorsoRicorsivo(piattaforme, P, 0, pathCorrente, 0, bestPath, &minSalti);

    /* --- 4. STAMPA RISULTATI --- */
    if (minSalti == INT_MAX) {
        printf("Non esiste un percorso per arrivare alla fine.\n");
    } else {
        printf("Soluzione ottima trovata in %d salti:\n", minSalti);
        printf("Sequenza piattaforme: ");
        // Stampiamo il percorso. Nota: i va da 0 a minSalti (incluso), perché
        // se facciamo 2 salti, tocchiamo 3 piattaforme (Start -> A -> End).
        for (int i = 0; i <= minSalti; i++) {
            printf("%d ", bestPath[i]);
            if (i < minSalti) printf("-> ");
        }
        printf("\n");
    }

    /* --- 5. PULIZIA MEMORIA --- */
    // Mai lasciare memory leaks!
    free(pathCorrente);
    free(bestPath);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Scopo: Ricreare l'esempio dell'immagine (P=5, valori 2 2 3 2 1) e testare la funzione.
 * ======================================================================================
 */
int main() {
    // Caso d'esempio dell'esercizio
    int P = 5;
    
    // Allocazione dinamica dell'array di input (simuliamo un caso reale)
    int *piattaforme = (int *)malloc(P * sizeof(int));
    
    // Popolamento manuale
    // Indice:  0  1  2  3  4
    // Valori:  2  2  3  2  1
    piattaforme[0] = 2;
    piattaforme[1] = 2;
    piattaforme[2] = 3;
    piattaforme[3] = 2;
    piattaforme[4] = 1;

    printf("--- Configurazione Piattaforme ---\n");
    for(int i=0; i<P; i++) printf("[%d] ", piattaforme[i]);
    printf("\n------------------------------\n");

    // Chiamata alla funzione principale
    trovaSequenzaOttima(piattaforme, P);

    // Liberiamo l'array di input
    free(piattaforme);

    return 0;
}