#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * DEFINIZIONE DELLA STRUTTURA (NODO)
 * ============================================================================
 * Come richiesto dall'esercizio, definiamo il nodo della lista.
 * Ogni nodo deve contenere:
 * 1. La sottostringa (char *).
 * 2. Il puntatore al prossimo nodo (struct node_t *).
 */
typedef struct node {
    char *val;          // Puntatore alla stringa allocata dinamicamente
    struct node *next;  // Puntatore al nodo successivo
} node_t;

/* ============================================================================
 * FUNZIONE: splitStr
 * ============================================================================
 * SCOPO:
 * Divide una stringa in sottostringhe basandosi sul separatore punto "."
 * e crea una lista concatenata dove ogni nodo contiene una sottostringa.
 * L'ordine dei nodi sarà inverso rispetto alla stringa originale.
 *
 * PARAMETRI:
 * - char *str: La stringa da dividere (es. "a.bb.ccc").
 *
 * RITORNO:
 * - node_t *: Il puntatore alla testa della nuova lista creata.
 *
 * CONCETTI CHIAVE:
 * 1. Tokenizzazione (strtok):
 * Usiamo la funzione standard strtok per spezzare la stringa ogni volta
 * che troviamo un punto.
 *
 * 2. Inserimento in Testa (Head Insertion):
 * Poiché l'esercizio mostra che l'ultimo elemento della stringa ("fffff")
 * diventa il primo della lista, dobbiamo inserire ogni nuovo pezzo trovato
 * all'inizio della lista.
 * Logica: NuovoNodo -> VecchiaTesta; NuovaTesta = NuovoNodo.
 *
 * 3. Deep Copy (Copia Profonda):
 * Non possiamo semplicemente puntare alla stringa originale (che viene
 * modificata da strtok). Dobbiamo allocare nuova memoria per ogni
 * parola e copiarci dentro i caratteri.
 * ============================================================================
 */
node_t *splitStr(char *str) {
    
    /* --- 1. INIZIALIZZAZIONE --- */
    node_t *head = NULL; // La lista è inizialmente vuota
    char *token;         // Puntatore temporaneo per le sottostringhe trovate

    /* * NOTA IMPORTANTE SU STRTOK:
     * strtok modifica la stringa originale inserendo '\0' al posto dei delimitatori.
     * La prima chiamata richiede la stringa 'str'.
     * Le chiamate successive richiedono NULL per continuare sulla stessa stringa.
     */
    token = strtok(str, ".");

    /* --- 2. CICLO DI SCANSIONE --- */
    while (token != NULL) {
        
        /* --- 3. ALLOCAZIONE NODO --- */
        /*
         * Allocazione dello spazio per la STUTTURA del nodo.
         * sizeof(node_t) calcola i byte necessari per (char* + struct node*).
         */
        node_t *new_node = (node_t *)malloc(sizeof(node_t));
        
        // Controllo difensivo: se la malloc fallisce, interrompiamo
        if (new_node == NULL) {
            return head; // Ritorniamo quello che abbiamo fatto finora (o NULL)
        }

        /* --- 4. ALLOCAZIONE DINAMICA DELLA STRINGA (Deep Copy) --- */
        /*
         * Concetto Critico:
         * Il puntatore 'token' punta a un pezzo della stringa 'str' originale.
         * Noi vogliamo che il nodo abbia la PROPRIA copia indipendente.
         *
         * strlen(token) + 1: Lunghezza caratteri + 1 per il terminatore '\0'.
         */
        new_node->val = (char *)malloc((strlen(token) + 1) * sizeof(char));
        
        // Controllo allocazione stringa
        if (new_node->val == NULL) {
            free(new_node); // Liberiamo il nodo appena creato perché inutile senza stringa
            return head;
        }

        /* Copiamo i caratteri dalla stringa token alla memoria del nodo */
        strcpy(new_node->val, token);

        /* --- 5. INSERIMENTO IN TESTA --- */
        /*
         * Dettaglio Sintattico:
         * 1. new_node->next = head; 
         * Il 'next' del nuovo nodo punta a quella che ERA la testa della lista.
         * (Colleghiamo il nuovo vagone al resto del treno).
         *
         * 2. head = new_node;
         * Aggiorniamo il puntatore 'head' generale per dire che il treno
         * ora inizia dal nuovo nodo.
         */
        new_node->next = head;
        head = new_node;

        /* --- 6. PASSAGGIO AL PROSSIMO TOKEN --- */
        /* Passiamo NULL per dire a strtok di continuare da dove aveva finito */
        token = strtok(NULL, ".");
    }

    return head;
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Gestione Memoria:
 * Il main deve:
 * 1. Creare una stringa MODIFICABILE (array, non literal).
 * 2. Ricevere la lista allocata dalla funzione.
 * 3. Usare la lista.
 * 4. Liberare TUTTA la memoria (sia i nodi, sia le stringhe dentro i nodi).
 * ============================================================================
 */
int main() {
    /*
     * ATTENZIONE:
     * Non usare: char *s = "a.bb.ccc"; (String Literal, è in sola lettura -> crash con strtok)
     * Usare: char s[] = ...; (Array nello stack, modificabile)
     */
    char test_string[] = "a.bb.ccc.dddd.eeeee.fffff";
    
    printf("Stringa input: %s\n", test_string);

    /* Chiamata alla funzione */
    node_t *lista = splitStr(test_string);

    /* --- STAMPA E VERIFICA --- */
    printf("Lista generata (dovrebbe essere inversa):\n");
    printf("HEAD -> ");
    
    node_t *curr = lista;
    while (curr != NULL) {
        printf("[%s] -> ", curr->val);
        curr = curr->next;
    }
    printf("NULL\n");

    /* --- PULIZIA DELLA MEMORIA (FREE) --- */
    /* * Dobbiamo scorrere la lista e liberare DUE cose per ogni elemento:
     * 1. La stringa allocata (curr->val)
     * 2. Il nodo stesso (curr)
     * Bisogna salvare il puntatore 'next' PRIMA di liberare il nodo corrente.
     */
    while (lista != NULL) {
        node_t *temp = lista;      // Salviamo il nodo da cancellare
        lista = lista->next;       // Spostiamo la testa avanti (salviamo il resto della lista)
        
        free(temp->val);           // 1. Libero la stringa
        free(temp);                // 2. Libero il nodo
    }
    printf("Memoria liberata correttamente.\n");

    return 0;
}