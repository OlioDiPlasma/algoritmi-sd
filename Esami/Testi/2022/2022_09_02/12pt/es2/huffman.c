#include <stdio.h>
#include <stdlib.h> // Necessaria solo per malloc/free
#include "huffman.h"

/* --- 1. DEFINIZIONE STRUTTURE DATI --- */

/* * QUASI-ADT: Il Nodo.
 * È visibile solo in questo file (staticamente o implicitamente nascosto).
 * Contiene il carattere (valido solo se è una foglia) e i puntatori ai figli.
 */
typedef struct nodo {
    char val;           // Il carattere decodificato (es: 'A')
    struct nodo *l;     // Figlio sinistro (percorso '0')
    struct nodo *r;     // Figlio destro (percorso '1')
} *link; // 'link' è un puntatore a struct nodo

/* * ADT DI PRIMA CLASSE: Il Wrapper H.
 * Questa struct "avvolge" la radice. Serve per mantenere un riferimento stabile
 * all'intero albero.
 */
struct h_wrapper {
    link root; 
};

/* --- 2. FUNZIONI AUSILIARIE (NO LIBRERIE) --- */

/*
 * Scopo: Calcolare la lunghezza di una stringa senza usare strlen di string.h.
 * Concetto: Aritmetica dei puntatori o scorrimento array.
 */
int mia_strlen(char *s) {
    int len = 0;
    // Scorriamo finché non troviamo il terminatore di stringa '\0'
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

/* --- 3. FUNZIONE RICHIESTA: DECODE --- */

/*
 * Scopo: Decodifica la sequenza di bit (rappresentati come char '0'/'1').
 * Parametri:
 * - H h: L'handle (puntatore) all'albero di Huffman.
 * - char *str: La stringa di input (es: "01011").
 * * Concetti Chiave:
 * - Navigazione albero binario: Discesa a sinistra per '0', a destra per '1'.
 * - Reset alla radice: Quando trovo una foglia, ho trovato un carattere,
 * quindi riparto dalla radice per il prossimo bit.
 * - Allocazione dinamica: Non sappiamo quanto sarà lunga la stringa di output,
 * ma sappiamo che non sarà mai più lunga di quella di input (nel caso peggiore 
 * ogni bit produce un char, impossibile in huffman ma sicuro per allocare).
 */
char *decode(H h, char *str) {

    /* --- CONTROLLI INIZIALI --- */
    if (h == NULL || h->root == NULL || str == NULL) {
        return NULL; // Gestione casi limite
    }

    /* --- A. ALLOCAZIONE MEMORIA RISULTATO --- */
    /* * Calcoliamo la lunghezza massima.
     * Nota tecnica: In Huffman, la stringa decodificata è solitamente PIÙ CORTA 
     * della stringa di bit "01010". Allocare `len + 1` è uno spreco accettabile 
     * per garantire sicurezza senza usare realloc complesse.
     */
    int len_input = mia_strlen(str);
    
    // Malloc: Chiede RAM allo Heap. 
    // sizeof(char) è sempre 1, ma lo scriviamo per chiarezza didattica.
    // +1 serve obbligatoriamente per il terminatore '\0'.
    char *risultato = (char *)malloc((len_input + 1) * sizeof(char));
    
    if (risultato == NULL) {
        printf("Errore malloc\n");
        return NULL;
    }

    /* --- B. INIZIALIZZAZIONE VARIABILI DI SCANSIONE --- */
    link curr = h->root;  // Puntatore per navigare l'albero
    int i_res = 0;        // Indice per scrivere nella stringa 'risultato'
    int i_str = 0;        // Indice per leggere la stringa 'str'

    /* --- C. LOOP DI DECODIFICA --- */
    // Continuiamo finché non finisce la stringa di input
    while (str[i_str] != '\0') {
        
        // Leggiamo il carattere corrente ('0' o '1')
        char bit = str[i_str];

        /* --- NAVIGAZIONE --- */
        if (bit == '0') {
            // Se leggo '0', vado a sinistra
            if (curr->l != NULL) 
                curr = curr->l;
        } else if (bit == '1') {
            // Se leggo '1', vado a destra
            if (curr->r != NULL) 
                curr = curr->r;
        } 
        // Nota: Se il bit non è ne 0 ne 1, lo ignoriamo o gestiamo errore (qui ignoriamo).

        /* --- VERIFICA FOGLIA (Carattere Trovato) --- */
        // Una foglia in Huffman non ha figli.
        if (curr->l == NULL && curr->r == NULL) {
            
            // 1. Scriviamo il carattere trovato nel buffer risultato
            risultato[i_res] = curr->val;
            
            // 2. Avanziamo l'indice del risultato
            i_res++;

            // 3. CRUCIALE: Resettiamo il puntatore alla radice dell'albero
            // per iniziare a decodificare il prossimo carattere.
            curr = h->root;
        }

        // Passiamo al prossimo bit nella stringa di input
        i_str++;
    }

    /* --- D. CHIUSURA STRINGA --- */
    // Ogni stringa C deve terminare con '\0' affinché printf sappia dove fermarsi.
    risultato[i_res] = '\0';

    return risultato; // Ritorniamo l'indirizzo della memoria heap allocata
}

/* -----------------------------------------------------------
 * FUNZIONI DI SUPPORTO (Solo per far funzionare il Main)
 * Non strettamente parte dell'algoritmo di decode, ma necessarie
 * per creare l'ambiente di test.
 * ----------------------------------------------------------- */

// Helper per allocare un nodo (privata)
link new_node(char c, link l, link r) {
    link x = malloc(sizeof(struct nodo));
    x->val = c;
    x->l = l;
    x->r = r;
    return x;
}

// Crea un albero hardcoded: 
//       (root)
//      /      \
//    'A'      (interno)
//    (0)      /       \
//           'B'       'C'
//           (10)      (11)
H crea_albero_test_hardcoded() {
    H h = malloc(sizeof(struct h_wrapper));
    
    // Costruiamo dal basso verso l'alto
    link nodoB = new_node('B', NULL, NULL); // Codice 10
    link nodoC = new_node('C', NULL, NULL); // Codice 11
    
    // Nodo interno che unisce B e C
    link interno = new_node('*', nodoB, nodoC); // Il char qui non conta
    
    // Nodo A (foglia sinistra)
    link nodoA = new_node('A', NULL, NULL); // Codice 0
    
    // Radice che unisce A e il sottoalbero destro
    h->root = new_node('*', nodoA, interno);
    
    return h;
}

// Helper ricorsivo per liberare memoria
void free_r(link x) {
    if (x == NULL) return;
    free_r(x->l);
    free_r(x->r);
    free(x); // Libera il nodo corrente solo dopo aver liberato i figli
}

void libera_albero(H h) {
    if (h != NULL) {
        free_r(h->root);
        free(h); // Libera il wrapper
    }
}