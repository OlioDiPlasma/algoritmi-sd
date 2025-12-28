#include "albero.h"
#include <stdlib.h>
#include <stdio.h>

/* ============================================================================
 * DEFINIZIONE DELLA STRUTTURA DATI
 * ============================================================================
 * Concetto Chiave: Albero N-ario.
 * A differenza di un albero binario (sinistra/destra), qui ogni nodo può avere
 * un numero arbitrario di figli. Per gestire ciò, usiamo un "array dinamico".
 *
 * Dettaglio Sintattico: struct nodo **figli;
 * Perché un doppio puntatore?
 * 1. 'struct nodo *' è un puntatore a un singolo nodo.
 * 2. Noi vogliamo un ARRAY di questi puntatori.
 * 3. In C, un array dinamico è gestito tramite un puntatore al primo elemento.
 * Quindi: puntatore a (puntatore a nodo).
 */
struct nodo {
    int info;           // Il dato contenuto nel nodo
    int n_figli;        // La dimensione dell'array 'figli'
    struct nodo **figli; // Vettore dinamico di puntatori ai sotto-alberi
};

/* ============================================================================
 * FUNZIONE AUSILIARIA (CORE LOGIC)
 * ============================================================================
 * Scopo:
 * Attraversa l'albero ricorsivamente per trovare la lunghezza del cammino
 * consecutivo di nodi positivi più lungo.
 *
 * Parametri:
 * - n: Il nodo che stiamo visitando attualmente (T è alias di struct nodo*).
 * - max_assoluto: Puntatore a intero (int *).
 *
 * Concetto Chiave: Passaggio per Riferimento (int *max_assoluto)
 * Stiamo usando la ricorsione. Ogni chiamata ha le sue variabili locali.
 * Se trovassimo un cammino lungo in un sotto-ramo profondo, come faremmo a
 * dirlo alla funzione principale?
 * Passiamo l'INDIRIZZO della variabile 'max_len' definita in f().
 * In questo modo, qualsiasi chiamata ricorsiva può scrivere direttamente
 * nella memoria della funzione chiamante per aggiornare il record mondiale.
 *
 * Ritorna:
 * - Un int rappresentante la lunghezza della catena positiva che parte da 'n'
 * e scende verso il basso. Se 'n' è negativo, ritorna 0.
 */
int conta_cammino_rec(T n, int *max_assoluto) {
    /* --- 1. CASO BASE (Safety Check) --- */
    // Se il puntatore è NULL, l'albero è finito o vuoto.
    if (n == NULL) {
        return 0;
    }

    int max_profondita_figli = 0; // Terrà traccia del figlio "migliore"

    /* --- 2. FASE DI DISCESA (Ricorsione) --- */
    // Prima di analizzare il nodo corrente, esploriamo tutti i sotto-alberi.
    // Questo è un approccio "Post-Order" (prima i figli, poi il padre),
    // necessario perché il padre ha bisogno di sapere quanto sono lunghi i cammini sotto.

    for (int i = 0; i < n->n_figli; i++) {
        // Dettaglio Sintattico: n->figli[i]
        // Accediamo all'i-esimo puntatore nell'array dei figli del nodo n.
        int profondita_figlio = conta_cammino_rec(n->figli[i], max_assoluto);

        // Cerchiamo il massimo tra tutti i figli diretti
        if (profondita_figlio > max_profondita_figli) {
            max_profondita_figli = profondita_figlio;
        }
    }

    /* --- 3. ELABORAZIONE DEL NODO CORRENTE --- */
    // Ora decidiamo se il nodo 'n' può far parte di un cammino.

    if (n->info >= 0) {
        // CASO POSITIVO: Il nodo estende il cammino proveniente dal figlio migliore.
        // Se max_profondita_figli era 3, e io sono valido, ora la catena è 4.
        int cammino_corrente = 1 + max_profondita_figli;

        // Dettaglio Sintattico: *max_assoluto
        // Dereferenziamo il puntatore per leggere il valore attuale del record globale.
        if (cammino_corrente > *max_assoluto) {
            // Se abbiamo battuto il record, scriviamo nella memoria puntata.
            *max_assoluto = cammino_corrente;
        }

        // Ritorniamo al padre (chi ci ha chiamato) la lunghezza della catena che parte da qui.
        return cammino_corrente;

    } else {
        // CASO NEGATIVO: La catena si spezza.
        // Anche se sotto di me c'era un cammino lungo 100, io sono negativo (-5),
        // quindi non posso estendere quel cammino verso il mio genitore.

        // Nota: Non dobbiamo preoccuparci di aggiornare max_assoluto qui,
        // perché lo hanno già fatto le chiamate ricorsive sui figli se necessario.

        return 0; // Comunico al mio genitore che da qui parte un cammino lungo 0.
    }
}

/* ============================================================================
 * FUNZIONE WRAPPER (Interfaccia Pubblica)
 * ============================================================================
 * Scopo:
 * Nascondere la complessità della ricorsione e della gestione del puntatore
 * allo stato (*max_assoluto) all'utente finale.
 *
 * Parametri:
 * - t: La radice dell'albero.
 */
int f(T t) {
    /* --- 1. INIZIALIZZAZIONE --- */
    // Questa variabile vive nello stack di f() e traccerà il massimo globale.
    int max_len = 0;

    /* --- 2. AVVIO RICORSIONE --- */
    if (t != NULL) {
        // Passiamo l'indirizzo (&) di max_len.
        // La funzione ausiliaria userà questo puntatore come una "lavagna condivisa".
        conta_cammino_rec(t, &max_len);
    }

    /* --- 3. RITORNO RISULTATO --- */
    return max_len;
}

/* ============================================================================
 * AREA DI TEST (MAIN E UTILITY)
 * ============================================================================
 */

/*
 * Funzione Helper: crea_nodo
 * Scopo: Semplificare l'allocazione mnemonica per il test.
 * Gestione Memoria:
 * - malloc(sizeof(struct nodo)): Alloca spazio per la struttura base.
 * - malloc(n * sizeof(T)): Alloca spazio per l'array di PUNTATORI ai figli.
 */
T crea_nodo(int valore, int numero_figli) {
    T n = (T)malloc(sizeof(struct nodo));
    if (n == NULL) { perror("Malloc fallita nodo"); exit(1); }

    n->info = valore;
    n->n_figli = numero_figli;

    if (numero_figli > 0) {
        // Allocazione dell'array di puntatori.
        // sizeof(T) è la dimensione di un puntatore (solitamente 8 byte su 64bit).
        n->figli = (T *)malloc(numero_figli * sizeof(T));
        if (n->figli == NULL) { perror("Malloc fallita figli"); exit(1); }
    } else {
        n->figli = NULL;
    }

    return n;
}

/*
 * Funzione Helper: free_albero
 * Scopo: Liberare correttamente tutta la memoria per evitare Memory Leaks.
 * Logica: Bisogna liberare PRIMA i figli, POI l'array di figli, POI il nodo stesso.
 */
void free_albero(T n) {
    if (n == NULL) return;

    // 1. Ricorsione sui figli
    for (int i = 0; i < n->n_figli; i++) {
        free_albero(n->figli[i]);
    }

    // 2. Libero l'array dinamico dei puntatori (se esiste)
    if (n->figli != NULL) {
        free(n->figli);
    }

    // 3. Libero la struttura del nodo corrente
    free(n);
}

int main() {
    printf("--- LEZIONE DI PROGRAMMAZIONE C: ALBERI N-ARI E RICORSIONE ---\n\n");

    /*
     * COSTRUZIONE DELL'ALBERO DI TEST
     * L'obiettivo è creare percorsi con valori positivi e vedere se trova il più lungo.
     *
     * Struttura visiva dell'albero:
     * (Root: -5)   <-- Negativo! Interrompe tutto ciò che arriva da sotto.
     * /     |     \
     * (10)  (2)    (8)
     * |     |      |
     * (5)   (-9)   (4)
     * |      |
     * (6)    (4)
     * |
     * (4)
     *
     * Analisi logica attesa:
     * 1. Ramo Sinistro: 10 -> 5 (Lun: 2).
     * 2. Ramo Centrale: 2 -> spezzato da -9. Sotto c'è 6 (Lun: 1). Totale ramo: 1.
     * 3. Ramo Destro:   8 -> 4 -> 4 -> 4 (Lun: 4). VINCENTE.
     */

    printf("1. Costruzione albero in memoria heap...\n");

    // Radice
    T root = crea_nodo(-5, 3);

    // Livello 1
    root->figli[0] = crea_nodo(10, 1);
    root->figli[1] = crea_nodo(2, 1);
    root->figli[2] = crea_nodo(8, 1);

    // Livello 2
    root->figli[0]->figli[0] = crea_nodo(5, 0);       // Fine ramo A
    root->figli[1]->figli[0] = crea_nodo(-9, 1);      // Spezza ramo B
    root->figli[2]->figli[0] = crea_nodo(4, 1);       // Continua ramo C

    // Livello 3
    root->figli[1]->figli[0]->figli[0] = crea_nodo(6, 0); // Ripresa isolata B
    root->figli[2]->figli[0]->figli[0] = crea_nodo(4, 1); // Continua ramo C

    // Livello 4
    root->figli[2]->figli[0]->figli[0]->figli[0] = crea_nodo(4, 0); // Fine ramo C

    printf("2. Albero costruito. Lancio funzione f(root)...\n");

    // ESECUZIONE
    int risultato = f(root);

    printf("\n--- RISULTATI ---\n");
    printf("Lunghezza massima calcolata: %d\n", risultato);
    printf("Lunghezza attesa: 4\n");

    if (risultato == 4) {
        printf(">>> TEST SUPERATO: La logica ricorsiva funziona correttamente. <<<\n");
    } else {
        printf(">>> TEST FALLITO: C'è un errore nella logica. <<<\n");
    }

    // PULIZIA MEMORIA
    printf("\n3. Pulizia della memoria (free_albero)...\n");
    free_albero(root);
    printf("Memoria liberata. Programma terminato.\n");

    return 0;
}

