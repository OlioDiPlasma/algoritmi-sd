#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

/* * ======================================================================================
 * DEFINIZIONE STRUTTURE DATI (Nascoste / Opaque)
 * ======================================================================================
 * CONCETTO CHIAVE: Information Hiding (Incapsulamento)
 * Definiamo le struct qui nel .c invece che nel .h. In questo modo, il 'main'
 * (o chiunque usi la libreria) non può accedere direttamente ai campi 'head', 
 * 'next' o 'prev'. È costretto a usare le nostre funzioni.
 */

struct node {
    char data;          /* Il dato (carattere) */
    struct node *next;  /* Puntatore al nodo successivo */
    struct node *prev;  /* Puntatore al nodo precedente (Lista Doppia) */
};

struct list {
    struct node *head;  /* Puntatore al primo nodo della lista */
};

/*
 * ======================================================================================
 * NOME FUNZIONE: f (Funzione d'Esame)
 * ======================================================================================
 * SCOPO:
 * Scorre una lista ordinata e compatta i gruppi di elementi uguali consecutivi
 * solo se la lunghezza del gruppo è >= k. Mantiene il primo elemento, cancella gli altri.
 *
 * PARAMETRI:
 * - LIST l: Il puntatore opaco alla lista.
 * - int k:  La soglia minima per far scattare la cancellazione.
 *
 * CONCETTI CHIAVE:
 * 1. Puntatori Multipli: Usiamo 'curr' (elemento corrente da analizzare) e 
 * 'scanner' (per esplorare in avanti).
 * 2. Manipolazione Link Doppi: Quando rimuoviamo nodi, dobbiamo stare attenti a
 * ricucire sia la direzione 'next' che la direzione 'prev'.
 * ======================================================================================
 */
void f(LIST l, int k) {
    /* --- 1. CONTROLLO SICUREZZA --- */
    /* Se la lista non è allocata o è vuota, terminiamo subito. */
    if (l == NULL || l->head == NULL) return;

    struct node *curr = l->head;

    /* --- 2. CICLO PRINCIPALE --- */
    /* Iteriamo finché ci sono nodi da analizzare */
    while (curr != NULL) {
        
        char target = curr->data;       /* Il carattere che stiamo contando */
        struct node *scanner = curr->next; /* Parte dal nodo subito dopo 'curr' */
        int count = 1;                  /* Contiamo anche 'curr' stesso */
        
        /* --- 3. FASE DI SCANSIONE --- */
        /* Avanziamo con 'scanner' finché troviamo caratteri identici a 'target'.
         * Usiamo l'operatore &&: la condizione a sinistra (scanner != NULL) viene 
         * valutata per prima per evitare Crash (Segmentation Fault).
         */
        while (scanner != NULL && scanner->data == target) {
            count++;
            scanner = scanner->next;
        }
        /* A questo punto 'scanner' punta al primo nodo DIVERSO (o a NULL) */

        /* --- 4. FASE DI COMPATTAZIONE --- */
        if (count >= k) {
            /* Caso: Abbiamo trovato abbastanza duplicati. Dobbiamo cancellarli.
             * NON cancelliamo 'curr' (il primo), ma partiamo da curr->next.
             */
            struct node *to_delete = curr->next;
            
            /* Cancelliamo tutto ciò che c'è tra 'curr' e 'scanner' */
            while (to_delete != scanner) {
                struct node *temp = to_delete; /* Salviamo il puntatore per liberarlo dopo */
                
                /* Avanziamo col puntatore PRIMA di fare la free, altrimenti
                 * perderemmo il riferimento al prossimo nodo (Dangling Pointer) */
                to_delete = to_delete->next; 
                
                /* GESTIONE MEMORIA: È vitale liberare la memoria allocata con malloc */
                free(temp);
            }

            /* --- 5. FASE DI "RICITURA" (RE-LINKING) --- */
            /* * Passo A: Il 'next' di curr deve saltare i nodi morti e puntare a scanner.
             * [curr] -----------------> [scanner]
             */
            curr->next = scanner;
            
            /* * Passo B: Se scanner esiste, il suo 'prev' deve puntare indietro a curr.
             * [curr] <----------------- [scanner]
             */
            if (scanner != NULL) {
                scanner->prev = curr;
            }
        }

        /* --- 6. AVANZAMENTO --- */
        /*
         * Trucco di efficienza: Non facciamo curr = curr->next.
         * Siccome sappiamo che da 'curr' fino a 'scanner' erano tutti uguali
         * (e li abbiamo trattati o scartati), possiamo saltare direttamente 
         * all'inizio del prossimo gruppo di caratteri diversi.
         */
        curr = scanner;
    }
}

/*
 * ======================================================================================
 * FUNZIONI DI SUPPORTO (Helpers)
 * ======================================================================================
 * Queste funzioni sono necessarie perché il MAIN non può usare malloc o accedere
 * ai puntatori interni essendo un ADT opaco.
 * ======================================================================================
 */

/* * Funzione: crea_lista
 * Alloca lo "scheletro" della lista (il wrapper).
 */
LIST crea_lista() {
    /* GESTIONE MEMORIA: sizeof(struct list) alloca lo spazio per il puntatore head */
    LIST l = malloc(sizeof(struct list));
    
    /* Inizializzazione sicura a NULL */
    if (l) l->head = NULL; 
    return l;
}

/* * Funzione: append
 * Inserisce un elemento in coda gestendo il doppio puntatore.
 */
void append(LIST l, char c) {
    if (!l) return; // Controllo esistenza lista wrapper

    /* Allocazione del singolo NODO */
    struct node *new_node = malloc(sizeof(struct node));
    new_node->data = c;
    new_node->next = NULL;
    new_node->prev = NULL; /* Importante inizializzarlo a NULL */

    /* Caso 1: Lista vuota */
    if (l->head == NULL) {
        l->head = new_node;
    } 
    /* Caso 2: Lista non vuota, cerchiamo l'ultimo elemento */
    else {
        struct node *temp = l->head;
        /* Scorro fino all'ultimo nodo (quello che ha next == NULL) */
        while (temp->next != NULL) {
            temp = temp->next;
        }
        /* Collego in avanti */
        temp->next = new_node;
        /* Collego all'indietro (doppia lista) */
        new_node->prev = temp;
    }
}

/* * Funzione: print_list
 * Stampa visiva per debug.
 */
void print_list(LIST l) {
    if (!l || !l->head) {
        printf("Lista vuota\n");
        return;
    }
    struct node *temp = l->head;
    while (temp != NULL) {
        printf("%c ", temp->data);
        /* Estetica: mostro la freccia solo se c'è un nodo dopo */
        if(temp->next != NULL) printf("<-> ");
        temp = temp->next;
    }
    printf("\n");
}

/* * Funzione: free_list
 * Distrugge l'intera lista per evitare Memory Leaks alla fine del programma.
 */
void free_list(LIST l) {
    if (!l) return;
    
    struct node *curr = l->head;
    while (curr != NULL) {
        struct node *temp = curr; /* Salvo nodo corrente */
        curr = curr->next;        /* Salvo il prossimo */
        free(temp);               /* Distruggo corrente */
    }
    
    /* Infine, distruggo il contenitore (wrapper) della lista */
    free(l);
}