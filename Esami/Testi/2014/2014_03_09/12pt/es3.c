#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * DEFINIZIONE DELLA STRUTTURA (NODO)
 * ============================================================================
 * L'esercizio richiede specifici campi dati (due stringhe dinamiche e un int)
 * e due puntatori specifici per la logica Left-Child Right-Sibling.
 */
typedef struct node {
    char *cognome;              // Stringa allocata dinamicamente
    char *nome;                 // Stringa allocata dinamicamente
    int voto;                   // Valore intero
    
    struct node *left_child;    // Puntatore al PRIMO FIGLIO
    struct node *right_sibling; // Puntatore al PROSSIMO FRATELLO
} node_t;

/* ============================================================================
 * FUNZIONE HELPER: creaNodo (Non richiesta esplicitamente ma necessaria)
 * ============================================================================
 * SCOPO:
 * Alloca memoria per un nuovo nodo e per le sue stringhe, inizializzando i campi.
 *
 * PARAMETRI:
 * - char *c: Cognome da copiare.
 * - char *n: Nome da copiare.
 * - int v: Voto.
 *
 * GESTIONE MEMORIA:
 * Utilizziamo malloc tre volte: una per il nodo, due per le stringhe.
 * È fondamentale usare (strlen(...) + 1) per includere il terminatore '\0'.
 * ============================================================================
 */
node_t *creaNodo(char *c, char *n, int v) {
    /* --- 1. ALLOCAZIONE NODO --- */
    node_t *nuovo = (node_t *)malloc(sizeof(node_t));
    if (nuovo == NULL) return NULL; // Controllo errore allocazione

    /* --- 2. ALLOCAZIONE E COPIA STRINGHE (Deep Copy) --- */
    /* Allocazione per il cognome: lunghezza caratteri + 1 per '\0' */
    nuovo->cognome = (char *)malloc((strlen(c) + 1) * sizeof(char));
    
    /* Allocazione per il nome */
    nuovo->nome = (char *)malloc((strlen(n) + 1) * sizeof(char));

    /* Controllo difensivo: se una delle malloc stringhe fallisce */
    if (nuovo->cognome == NULL || nuovo->nome == NULL) {
        free(nuovo); // Evitiamo memory leak parziale
        return NULL;
    }

    /* Copia effettiva dei dati nelle nuove aree di memoria */
    strcpy(nuovo->cognome, c);
    strcpy(nuovo->nome, n);
    nuovo->voto = v;

    /* --- 3. INIZIALIZZAZIONE PUNTATORI --- */
    /* Un nodo appena nato non ha ancora figli o fratelli collegati */
    nuovo->left_child = NULL;
    nuovo->right_sibling = NULL;

    return nuovo;
}

/* ============================================================================
 * FUNZIONE RICHIESTA: visualizzaAlbero
 * ============================================================================
 * SCOPO:
 * Visita ricorsivamente l'albero e stampa il contenuto di ogni nodo.
 * Utilizziamo una visita in "Pre-Order" (Prima la radice, poi i figli).
 *
 * PARAMETRI:
 * - node_t *root: Il puntatore al nodo attuale che stiamo analizzando.
 *
 * CONCETTI CHIAVE:
 * 1. Ricorsione Doppia:
 * La funzione chiama se stessa due volte:
 * - Una volta su `left_child` per scendere in profondità (visitare i figli).
 * - Una volta su `right_sibling` per scorrere lateralmente (visitare i fratelli).
 * * 2. Logica LCRS:
 * Anche se strutturalmente è un albero binario, logicamente stiamo visitando
 * una gerarchia padre-figli.
 * ============================================================================
 */
void visualizzaAlbero(node_t *root) {
    /* --- 1. CASO BASE --- */
    /* Se il puntatore è NULL, siamo arrivati a una foglia o alla fine della lista fratelli */
    if (root == NULL) {
        return;
    }

    /* --- 2. ELABORAZIONE DEL NODO CORRENTE --- */
    /* Accediamo ai campi della struttura tramite l'operatore freccia '->' */
    printf("Studente: %s %s - Voto: %d\n", root->cognome, root->nome, root->voto);

    /* --- 3. PASSO RICORSIVO (DISCESA VERSO I FIGLI) --- */
    /* Andiamo verso il 'left_child'. In questo modello, stiamo visitando
       il primo figlio del nodo corrente (e poi tutti i discendenti di quel figlio) */
    visualizzaAlbero(root->left_child);

    /* --- 4. PASSO RICORSIVO (SPOSTAMENTO AI FRATELLI) --- */
    /* Andiamo verso il 'right_sibling'. Stiamo visitando gli altri nodi
       che hanno lo stesso padre del nodo corrente */
    visualizzaAlbero(root->right_sibling);
}

/* ============================================================================
 * FUNZIONE DI PULIZIA: liberaAlbero
 * ============================================================================
 * SCOPO:
 * Liberare correttamente tutta la memoria allocata (malloc) per evitare Memory Leaks.
 *
 * GESTIONE MEMORIA:
 * L'ordine è CRUCIALE. Non possiamo liberare il nodo 'root' prima di aver letto
 * i puntatori ai suoi figli/fratelli, altrimenti avremmo un "Use After Free".
 * Si usa una logica "Post-Order": prima libero i sotto-elementi, poi me stesso.
 * ============================================================================
 */
void liberaAlbero(node_t *root) {
    if (root == NULL) return;

    /* Prima libero ricorsivamente tutto ciò che "pende" da questo nodo */
    liberaAlbero(root->left_child);
    liberaAlbero(root->right_sibling);

    /* Ora che figli e fratelli sono salvi (liberati), posso distruggere il nodo corrente */
    
    /* --- DETTAGLIO: LIBERARE LE STRINGHE INTERNE --- */
    /* root->cognome è un puntatore a un'area malloc. Va liberata prima della struct. */
    free(root->cognome);
    free(root->nome);
    
    /* Infine, libero il contenitore (la struct nodo) */
    free(root);
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Riproduciamo la struttura della Fig. 1(b) dell'esercizio.
 * Gerarchia:
 * a (padre) -> b (figlio di a)
 * b -> c (fratello di b)
 * b -> d (fratello di c)
 * * b -> e (figlio di b)
 * e -> h (figlio di e)
 * h -> i (fratello di h) -> j (fratello di i)
 *
 * d -> f (figlio di d) -> g (fratello di f)
 * ============================================================================
 */
int main() {
    /* --- 1. CREAZIONE DEI NODI (DATI FITTIZI) --- */
    /* Creiamo i nodi singolarmente per chiarezza, poi li colleghiamo */
    node_t *a = creaNodo("Rossi", "Andrea", 28); // Radice
    node_t *b = creaNodo("Bianchi", "Bruno", 25);
    node_t *c = creaNodo("Verdi", "Carlo", 22);
    node_t *d = creaNodo("Neri", "Dario", 30);
    node_t *e = creaNodo("Gialli", "Elio", 18);
    node_t *f = creaNodo("Blu", "Fabio", 27);
    node_t *g = creaNodo("Arancio", "Gino", 24);
    node_t *h = creaNodo("Viola", "Hugo", 29);
    node_t *i = creaNodo("Rosa", "Ivan", 21);
    node_t *j = creaNodo("Marroni", "Juri", 26);

    /* --- 2. COLLEGAMENTO LOGICO (COSTRUZIONE ALBERO FIG 1b) --- */
    
    /* Livello 1: a è radice. Il suo primo figlio è b */
    a->left_child = b;

    /* Livello 2: b ha fratelli c e d */
    b->right_sibling = c;
    c->right_sibling = d;
    /* d è l'ultimo fratello, il suo right_sibling resta NULL */

    /* Sottoalbero di b: b ha figlio e */
    b->left_child = e;
    
    /* Sottoalbero di e: e ha figlio h */
    e->left_child = h;
    /* h ha fratelli i, j */
    h->right_sibling = i;
    i->right_sibling = j;

    /* Sottoalbero di d: d ha figlio f */
    d->left_child = f;
    /* f ha fratello g */
    f->right_sibling = g;

    /* --- 3. TEST VISUALIZZAZIONE --- */
    printf("--- VISUALIZZAZIONE ALBERO (PRE-ORDER) ---\n");
    visualizzaAlbero(a);

    /* --- 4. PULIZIA MEMORIA --- */
    liberaAlbero(a);
    printf("\n--- MEMORIA LIBERATA CORRETTAMENTE ---\n");

    return 0;
}