#include "albero.h"
#include <stdlib.h> // Necessario solo per NULL

/* * Definizione della struttura nodo (Quasi ADT interno al modulo).
 * Include il valore, il numero di figli e il vettore dinamico dei figli.
 */
struct nodo {
    int info;              // Valore del nodo
    int n_figli;           // Numero di figli
    struct nodo **figli;   // Vettore di puntatori ai figli (T *)
};

/* --- FUNZIONE RICORSIVA (Ausiliaria) ---
 * Parametri:
 * - n: nodo corrente
 * - max_assoluto: puntatore a intero che traccia il record massimo trovato finora
 * * Ritorna:
 * - La lunghezza del cammino valido che parte da 'n' e scende verso il basso.
 */
int conta_cammino_rec(T n, int *max_assoluto) {
    // Caso base: albero vuoto o nodo nullo
    if (n == NULL) {
        return 0;
    }

    int max_profondita_figli = 0;

    // Iteriamo su tutti i figli del nodo corrente
    for (int i = 0; i < n->n_figli; i++) {
        // Chiamata ricorsiva: chiediamo al figlio quanto è lunga la sua catena
        int profondita_figlio = conta_cammino_rec(n->figli[i], max_assoluto);
        
        // Cerchiamo il figlio che offre il proseguimento più lungo
        if (profondita_figlio > max_profondita_figli) {
            max_profondita_figli = profondita_figlio;
        }
    }

    // Analisi del nodo corrente
    if (n->info >= 0) {
        // Se il nodo è valido, estendiamo il cammino migliore dei figli
        int cammino_corrente = 1 + max_profondita_figli;

        // Controlliamo se questo nuovo cammino è il record assoluto
        if (cammino_corrente > *max_assoluto) {
            *max_assoluto = cammino_corrente;
        }

        // Ritorniamo la lunghezza del cammino che parte da qui
        return cammino_corrente;
    } else {
        // Se il nodo è negativo, il cammino si spezza.
        // Ritorniamo 0 al padre (non posso estendere il cammino sopra di me),
        // ma la ricorsione sui figli è già stata fatta per cercare cammini "sotto".
        return 0;
    }
}

/* --- FUNZIONE WRAPPER ---
 * Interfaccia pubblica
 */
int f(T t) {
    int max_len = 0; // Variabile per tracciare il massimo globale
    
    if (t != NULL) {
        conta_cammino_rec(t, &max_len);
    }
    
    return max_len;
}

/* * ===============================================================
 * AREA DI TEST (Da inserire in fondo al file albero.c)
 * ===============================================================
 */

#include <stdio.h>

/* Funzione Helper per creare nodi rapidamente (solo per il test) */
T crea_nodo(int valore, int numero_figli) {
    T n = (T)malloc(sizeof(struct nodo));
    n->info = valore;
    n->n_figli = numero_figli;
    
    if (numero_figli > 0) {
        n->figli = (T *)malloc(numero_figli * sizeof(T));
    } else {
        n->figli = NULL;
    }
    
    return n;
}

int main() {
    /*
     * COSTRUZIONE DELL'ALBERO DI TEST
     * L'obiettivo è creare percorsi con valori positivi e vedere se trova il più lungo.
     * * Struttura dell'albero creato:
     * * (Root: -5)  <-- Negativo, il cammino non può partire qui
     * /    |    \
     * (10)  (2)   (8)
     * |     |     |
     * (5)  (-9)   (4)
     * |     |
     * (6)   (4)
     * |
     * (4)
     * * Analisi dei cammini positivi:
     * 1. Ramo Sinistro: 10 -> 5. Lunghezza = 2.
     * 2. Ramo Centrale: 2. Poi c'è -9 che spezza. Sotto c'è 6 (lunghezza 1). Max qui = 1.
     * 3. Ramo Destro:   8 -> 4 -> 4 -> 4. Lunghezza = 4.
     * * RISULTATO ATTESO: 4
     */

    printf("--- Inizio Test Funzione f(T t) ---\n");

    // 1. Creiamo la radice (Valore negativo per testare che venga ignorata)
    T root = crea_nodo(-5, 3);

    // 2. Livello 1
    root->figli[0] = crea_nodo(10, 1); // Ramo A
    root->figli[1] = crea_nodo(2, 1);  // Ramo B
    root->figli[2] = crea_nodo(8, 1);  // Ramo C (Quello vincente)

    // 3. Livello 2
    root->figli[0]->figli[0] = crea_nodo(5, 0);   // Fine Ramo A (Len: 2)
    
    root->figli[1]->figli[0] = crea_nodo(-9, 1);  // Ramo B spezzato da negativo
    
    root->figli[2]->figli[0] = crea_nodo(4, 1);   // Continua Ramo C

    // 4. Livello 3
    root->figli[1]->figli[0]->figli[0] = crea_nodo(6, 0); // Ripresa Ramo B (Len: 1)
    
    root->figli[2]->figli[0]->figli[0] = crea_nodo(4, 1); // Continua Ramo C

    // 5. Livello 4
    root->figli[2]->figli[0]->figli[0]->figli[0] = crea_nodo(4, 0); // Fine Ramo C (Len: 4)

    // ESECUZIONE TEST
    int risultato = f(root);

    printf("Lunghezza massima calcolata: %d\n", risultato);
    printf("Lunghezza attesa: 4\n");

    if (risultato == 4) {
        printf(">>> TEST SUPERATO <<<\n");
    } else {
        printf(">>> TEST FALLITO <<<\n");
    }

    // (Opzionale) Qui andrebbe una funzione per liberare la memoria dell'albero

    return 0;
}