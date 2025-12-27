#include <stdio.h>
#include <stdlib.h>
#include <float.h> // Per FLT_MIN (minimo numero float possibile)

#define K 3 // Grado dell'albero come da esempio (massimo 3 figli)

/*
 * ======================================================================================
 * DEFINIZIONE STRUTTURE DATI (ADT)
 * ======================================================================================
 */
typedef struct node_t {
    int id;                 // Identificativo univoco del nodo
    float weight;           // Peso del singolo nodo (può essere negativo!)
    struct node_t *child[K]; // Array di puntatori ai sottoalberi (Grado K)
} node_t, *link;

/*
 * ======================================================================================
 * NOME FUNZIONE: nuovoNodo
 * ======================================================================================
 * SCOPO:
 * Funzione helper per creare un nuovo nodo nello Heap.
 *
 * PARAMETRI:
 * - int id: Identificativo.
 * - float w: Peso.
 *
 * RITORNA:
 * - link: Puntatore al nodo appena allocato.
 *
 * GESTIONE MEMORIA:
 * - malloc: Allocazione dinamica.
 * - calloc/inizializzazione: È vitale settare i puntatori `child` a NULL, altrimenti
 * punterebbero a memoria "sporca" causando crash.
 * ======================================================================================
 */
link nuovoNodo(int id, float w) {
    link x = (link)malloc(sizeof(node_t));
    if (x == NULL) {
        printf("Errore malloc\n");
        exit(1);
    }
    x->id = id;
    x->weight = w;
    
    /* Inizializzo tutti i figli a NULL */
    for (int i = 0; i < K; i++) {
        x->child[i] = NULL;
    }
    return x;
}

/*
 * ======================================================================================
 * NOME FUNZIONE: visitaRicorsiva (Funzione Ausiliaria)
 * ======================================================================================
 * SCOPO:
 * Calcola il peso totale del sottoalbero radicato in 'root' e, contemporaneamente,
 * controlla se questo peso è il nuovo record massimo globale.
 *
 * PARAMETRI:
 * - link root: Nodo corrente dell'albero.
 * - float *maxWtRef: Puntatore alla variabile che tiene traccia del peso massimo globale.
 * - int *bestIdRef: Puntatore alla variabile che tiene traccia dell'ID del nodo "vincente".
 *
 * RITORNA:
 * - float: Il peso totale del sottoalbero corrente (serve al padre per il calcolo).
 *
 * CONCETTI CHIAVE:
 * - Post-Order Traversal (Visita in Post-Ordine): Prima devo sapere quanto pesano i miei
 * figli (chiamate ricorsive), poi posso sommare il mio peso e capire il totale del
 * mio sottoalbero.
 * - Passaggio per Riferimento: Usiamo puntatori (`*maxWtRef`) per aggiornare variabili
 * che devono "sopravvivere" tra le varie chiamate ricorsive.
 * ======================================================================================
 */
float visitaRicorsiva(link root, float *maxWtRef, int *bestIdRef) {
    /* --- 1. CASO BASE --- */
    if (root == NULL) {
        return 0.0;
    }

    /* --- 2. PASSO RICORSIVO (Discesa) --- */
    float pesoTotaleSottoalbero = root->weight;

    /* Sommo i pesi ritornati da tutti i figli */
    for (int i = 0; i < K; i++) {
        if (root->child[i] != NULL) {
            pesoTotaleSottoalbero += visitaRicorsiva(root->child[i], maxWtRef, bestIdRef);
        }
    }

    /* --- 3. LOGICA DI CONTROLLO (Risalita) --- */
    /* Ora so quanto pesa l'intero albero che parte da 'root'.
       Controllo se è il più pesante mai visto finora. */
    if (pesoTotaleSottoalbero > *maxWtRef) {
        *maxWtRef = pesoTotaleSottoalbero; // Aggiorno il record del peso
        *bestIdRef = root->id;             // Aggiorno il vincitore
    }

    /* Ritorno il peso totale al mio genitore, affinché lui possa fare i suoi calcoli */
    return pesoTotaleSottoalbero;
}

/*
 * ======================================================================================
 * NOME FUNZIONE: maxSum (Wrapper richiesto)
 * ======================================================================================
 * SCOPO:
 * Interfaccia principale richiesta dal testo dell'esercizio.
 * Inizializza le variabili di tracciamento e lancia la ricorsione.
 *
 * PARAMETRI:
 * - link root: Radice dell'albero.
 * - float *maxwt: Puntatore dove scrivere il risultato del peso massimo.
 *
 * RITORNA:
 * - int: L'identificativo del nodo radice del sottoalbero a peso massimo.
 * ======================================================================================
 */
int maxSum(link root, float *maxwt) {
    /* Inizializzazione:
     * Uso FLT_MIN (un numero negativo molto grande in modulo o piccolissimo) 
     * per assicurarmi che qualsiasi somma (anche negativa) lo superi.
     * Se usassi 0, e tutti i pesi fossero negativi, il risultato sarebbe sbagliato.
     */
    *maxwt = -FLT_MAX; 
    int bestID = -1;

    /* Lancio il motore ricorsivo */
    visitaRicorsiva(root, maxwt, &bestID);

    return bestID;
}

/*
 * ======================================================================================
 * FUNZIONE DI PULIZIA
 * ======================================================================================
 */
void liberaAlbero(link root) {
    if (root == NULL) return;
    for (int i = 0; i < K; i++) {
        liberaAlbero(root->child[i]);
    }
    free(root);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    /* --- 1. COSTRUZIONE ALBERO DI ESEMPIO --- */
    /* Ricostruisco parzialmente l'albero della figura per verificare il calcolo su nodo 2.
     * Struttura rilevante:
     * Nodo 2 (peso 3)
     * |-> Figlio: Nodo 6 (peso 5.3)
     * |-> Figlio: Nodo 8 (peso 2.4)
     * |-> Figlio: Nodo 9 (peso -5.0)
     *
     * Totale atteso: 3 + 5.3 + 2.4 - 5.0 = 5.7
     */
    
    printf("--- Creazione Albero ---\n");
    
    // Radice fittizia per agganciare il nodo 2 (come nell'immagine 0 -> 2)
    link root = nuovoNodo(0, 15.2);
    
    // Nodo 2
    root->child[1] = nuovoNodo(2, 3.0);
    link nodo2 = root->child[1];

    // Nodo 6 (figlio di 2)
    nodo2->child[1] = nuovoNodo(6, 5.3);
    link nodo6 = nodo2->child[1];

    // Nodi 8 e 9 (figli di 6)
    nodo6->child[0] = nuovoNodo(8, 2.4);
    nodo6->child[1] = nuovoNodo(9, -5.0);

    /* Aggiungo altri rami per testare che non interferiscano */
    root->child[0] = nuovoNodo(1, -100.0); // Ramo molto negativo

    /* --- 2. ESECUZIONE FUNZIONE --- */
    float maxWeightFound;
    int idMax = maxSum(root, &maxWeightFound);

    /* --- 3. STAMPA RISULTATI --- */
    printf("Analisi completata.\n");
    printf("ID Sottoalbero Max: %d\n", idMax);
    printf("Peso Massimo: %.2f\n", maxWeightFound);

    /* Verifica automatica */
    if (idMax == 2 && (maxWeightFound > 5.6 && maxWeightFound < 5.8)) {
        printf("[OK] Il risultato corrisponde all'esempio (5.7).\n");
    } else {
        printf("[FAIL] Risultato inatteso.\n");
    }

    /* --- 4. PULIZIA MEMORIA --- */
    liberaAlbero(root);

    return 0;
}