#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * DEFINIZIONE TIPO DI DATO (ADT)
 * ======================================================================================
 * Definiamo la struttura del nodo come richiesto dal testo.
 * Usiamo `typedef` per rendere il codice più leggibile: 'link' sarà un puntatore a 'nodo'.
 */
struct nodo {
    char val;           // Il dato (carattere)
    struct nodo *next;  // Il puntatore al nodo successivo
};

typedef struct nodo *link;


/*
 * ======================================================================================
 * NOME FUNZIONE: nuovoNodo
 * ======================================================================================
 * SCOPO:
 * Funzione di utilità per creare un singolo nodo nello Heap.
 * Serve a evitare di ripetere la malloc e l'inizializzazione ogni volta.
 *
 * PARAMETRI:
 * - char c: Il carattere da inserire nel nodo.
 *
 * RITORNA:
 * - link: L'indirizzo del nuovo nodo appena allocato.
 *
 * GESTIONE MEMORIA:
 * - Chiama malloc: è fondamentale controllare se restituisce NULL.
 * ======================================================================================
 */
link nuovoNodo(char c) {
    /* --- 1. ALLOCAZIONE --- */
    link x = (link)malloc(sizeof(struct nodo));
    
    /* Controllo difensivo */
    if (x == NULL) {
        printf("Errore: memoria esaurita.\n");
        exit(1); 
    }

    /* --- 2. INIZIALIZZAZIONE --- */
    x->val = c;
    x->next = NULL; /* Importante: il next deve nascere NULL per sicurezza */
    
    return x;
}

/*
 * ======================================================================================
 * NOME FUNZIONE: aggiungiInCoda
 * ======================================================================================
 * SCOPO:
 * Aggiunge un carattere alla fine della nuova lista che stiamo costruendo.
 * Modifica il puntatore di testa (se la lista era vuota) e aggiorna il puntatore di coda.
 *
 * PARAMETRI:
 * - link *headRef: Puntatore al puntatore della testa (per poter modificare head nel chiamante).
 * - link *tailRef: Puntatore al puntatore della coda (per inserimento O(1) invece di O(N)).
 * - char c: Il carattere da aggiungere.
 *
 * CONCETTI CHIAVE:
 * - Doppio Puntatore (**): Passiamo `&head2` e `&tail2` perché se la lista è vuota, 
 * dobbiamo modificare dove punta la variabile `head2` nel main/funzione chiamante.
 * ======================================================================================
 */
void aggiungiInCoda(link *headRef, link *tailRef, char c) {
    /* Creo il nuovo nodo usando la funzione helper */
    link nuovo = nuovoNodo(c);

    /* Se la lista è vuota (*headRef è NULL), il nuovo nodo è sia testa che coda */
    if (*headRef == NULL) {
        *headRef = nuovo; // Dereferenzio per modificare il puntatore originale
        *tailRef = nuovo;
    } else {
        /* Altrimenti, attacco il nuovo nodo dopo l'attuale coda */
        (*tailRef)->next = nuovo; // Accedo al campo 'next' dell'ultimo nodo
        *tailRef = nuovo;         // Aggiorno il puntatore di coda all'ultimo arrivato
    }
}

/*
 * ======================================================================================
 * NOME FUNZIONE: purgeList
 * ======================================================================================
 * SCOPO:
 * Genera una nuova lista copiando gli elementi della prima, ma sostituendo 
 * il contenuto delle parentesi tonde (...) con un singolo asterisco *.
 *
 * PARAMETRI:
 * - link head1: Il puntatore alla testa della lista originale (input).
 *
 * RITORNA:
 * - link: Il puntatore alla testa della nuova lista generata (output).
 *
 * LOGICA:
 * Scorriamo la lista 1. Se troviamo caratteri normali, li copiamo.
 * Se troviamo '(', copiamo '(', inseriamo '*', saltiamo tutto fino a ')', copiamo ')'.
 * ======================================================================================
 */
link purgeList(link head1) {
    /* --- 1. PREPARAZIONE --- */
    link head2 = NULL; /* Testa della nuova lista */
    link tail2 = NULL; /* Coda della nuova lista (per inserimenti veloci) */
    link curr = head1; /* Cursore per scorrere la lista input */

    /* --- 2. SCANSIONE DELLA LISTA --- */
    while (curr != NULL) {
        
        /* Caso A: Troviamo una parentesi aperta */
        if (curr->val == '(') {
            
            /* 1. Copiamo la parentesi aperta nella nuova lista */
            aggiungiInCoda(&head2, &tail2, '(');
            
            /* 2. Inseriamo l'asterisco di sostituzione */
            aggiungiInCoda(&head2, &tail2, '*');

            /* 3. Saltiamo il contenuto della lista originale */
            /* Avanziamo finché non troviamo la parentesi chiusa.
               ATTENZIONE: Il testo garantisce che la lista sia corretta e bilanciata,
               quindi non controlliamo curr != NULL nel while interno, assumiamo che ')' esista. */
            while (curr->val != ')') {
                curr = curr->next; /* Saltiamo il nodo */
            }
            
            /* Ora curr punta esattamente alla parentesi chiusa ')' */
            
            /* 4. Copiamo la parentesi chiusa nella nuova lista */
            aggiungiInCoda(&head2, &tail2, ')');

        } 
        /* Caso B: Carattere normale */
        else {
            aggiungiInCoda(&head2, &tail2, curr->val);
        }

        /* Avanziamo al nodo successivo della lista input */
        curr = curr->next;
    }

    return head2;
}

/*
 * ======================================================================================
 * FUNZIONI DI SUPPORTO PER IL TEST (Stampa e Free)
 * ======================================================================================
 */
void stampaLista(link h) {
    while (h != NULL) {
        printf("%c -> ", h->val);
        h = h->next;
    }
    printf("NULL\n");
}

/* * Fondamentale: Libera la memoria per evitare Memory Leaks.
 * Scorriamo la lista, salviamo il puntatore al prossimo, liberiamo il corrente.
 */
void liberaLista(link h) {
    link temp;
    while (h != NULL) {
        temp = h->next; // Salvo il "prossimo" prima di distruggere il ponte
        free(h);        // Libero il nodo corrente
        h = temp;       // Passo al prossimo
    }
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    /* --- 1. CREAZIONE DATI DI PROVA (HARDCODED) --- */
    /* Creiamo la stringa dell'esempio: a b ( a c g ) b e ( ) a ( x x ) f */
    /* Nota: Poiché non possiamo usare funzioni di libreria per convertire stringa->lista,
       lo facciamo manualmente o con un piccolo loop di supporto qui nel main */
    
    char inputString[] = "ab(acg)be()a(xx)f";
    link head1 = NULL;
    link tail1 = NULL; // Solo per costruire head1 velocemente
    
    printf("--- Creazione Lista Input ---\n");
    // Costruiamo la lista 1 manualmente dal vettore di char
    for (int i = 0; inputString[i] != '\0'; i++) {
        aggiungiInCoda(&head1, &tail1, inputString[i]);
    }
    
    printf("Lista 1 (Input) : ");
    stampaLista(head1);

    /* --- 2. LOGICA (CHIAMATA ALLA FUNZIONE) --- */
    printf("\n--- Esecuzione purgeList ---\n");
    link head2 = purgeList(head1);

    /* --- 3. STAMPA RISULTATI --- */
    printf("Lista 2 (Output): ");
    stampaLista(head2);
    
    /* Verifica visuale rispetto all'esempio: a b ( * ) b e ( * ) a ( * ) f */

    /* --- 4. GESTIONE MEMORIA --- */
    printf("\n--- Pulizia Memoria ---\n");
    liberaLista(head1);
    liberaLista(head2);
    printf("Memoria liberata.\n");

    return 0;
}