#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* --- 1. STRUTTURE DATI --- */

/* * Nodo della lista.
 * Contiene la stringa (char*) e il puntatore al prossimo nodo.
 */
typedef struct node {
    char *data;
    struct node *next;
} Node;

/*
 * Struttura Wrapper per l'ADT I Classe.
 * La LIST è un puntatore a questa scatola, che contiene la testa della catena.
 * Usiamo un wrapper per poter modificare 'head' senza usare doppi puntatori
 * nei parametri delle funzioni chiamanti.
 */
struct list_struct {
    Node *head;
    Node *tail; /* Ottimizzazione: teniamo la coda per inserimenti O(1) */
};

/* --- 2. FUNZIONI DI SUPPORTO (NO LIBRERIE) --- */

/*
 * Scopo: Calcolare la lunghezza di una stringa manualmente.
 * Parametri:
 * - s: stringa di input.
 * Ritorna: numero di caratteri prima di '\0'.
 */
int my_strlen(char *s) {
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

/*
 * Scopo: Creare una copia di una stringa nell'heap (equivalente a strdup).
 * Concetti Chiave: 
 * - Allocazione dinamica: dobbiamo preservare la stringa nel nodo.
 * - +1: Spazio vitale per il terminatore '\0'.
 */
char *my_strdup_range(char *source, int len) {
    /* Malloc: dimensione calcolata * grandezza char */
    char *new_str = (char *)malloc((len + 1) * sizeof(char));
    if (new_str == NULL) return NULL; // Controllo errore allocazione
    
    /* Copia manuale byte per byte */
    for (int i = 0; i < len; i++) {
        new_str[i] = source[i];
    }
    new_str[len] = '\0'; /* Fondamentale: chiudere la stringa */
    return new_str;
}

/*
 * Scopo: Inizializzare una lista vuota.
 */
LIST list_create() {
    LIST l = malloc(sizeof(struct list_struct));
    if (l != NULL) {
        l->head = NULL;
        l->tail = NULL;
    }
    return l;
}

/*
 * Scopo: Aggiungere un nodo in coda alla lista.
 * Concetti Chiave:
 * - Gestione caso lista vuota vs lista non vuota.
 * - Uso del puntatore 'tail' per evitare di scorrere tutta la lista ogni volta.
 */
void list_append(LIST l, char *val) {
    Node *new_node = malloc(sizeof(Node));
    new_node->data = val; /* La stringa è già stata allocata dal chiamante */
    new_node->next = NULL;

    if (l->head == NULL) {
        /* Caso 1: Lista vuota */
        l->head = new_node;
        l->tail = new_node;
    } else {
        /* Caso 2: Appendo alla fine */
        l->tail->next = new_node; /* Collego l'attuale ultimo al nuovo */
        l->tail = new_node;       /* Aggiorno il puntatore all'ultimo */
    }
}

/* --- 3. IMPLEMENTAZIONE CORE (RICHIESTA DALL'ESERCIZIO) --- */

/*
 * Scopo: Suddividere la stringa in nodi basandosi sul separatore.
 * Parametri:
 * - l: la lista (ADT) dove inserire i risultati.
 * - str: la stringa sorgente.
 * - sep: il carattere separatore.
 *
 * Logica Esempio: "ramarro", sep='r' -> "rama" -> "r" -> "ro"
 * Regola: Spezzare quando si incontra 'sep'. 'sep' va nella stringa successiva.
 */
void split(LIST l, char *str, char sep) {
    /* Controllo input difensivo */
    if (l == NULL || str == NULL) return;

    /* Buffer temporaneo per costruire la sottostringa corrente.
       La dimensione massima di una sottostringa è la stringa stessa. */
    int source_len = my_strlen(str);
    char *buffer = malloc((source_len + 1) * sizeof(char));
    int buf_idx = 0;

    /* --- LOGICA DI SCANSIONE --- */
    for (int i = 0; str[i] != '\0'; i++) {
        char current = str[i];

        /* * Condizione di taglio:
         * 1. Troviamo il separatore (current == sep).
         * 2. Il buffer non è vuoto (buf_idx > 0). 
         * Perché > 0? Perché se la stringa inizia con 'sep' (es. ramarro),
         * il primo 'r' appartiene al primo nodo, non c'è nulla prima da chiudere.
         */
        if (current == sep && buf_idx > 0) {
            /* Trovato separatore valido per taglio: chiudiamo il nodo corrente */
            
            /* A. Finalizza la stringa nel buffer */
            buffer[buf_idx] = '\0';
            
            /* B. Crea nodo e appendi (usiamo la nostra my_strdup interna) */
            char *node_str = my_strdup_range(buffer, buf_idx);
            list_append(l, node_str);

            /* C. Reset buffer per il prossimo nodo */
            buf_idx = 0;
            
            /* D. REGOLA CRUCIALE: Il separatore corrente fa parte del NUOVO nodo */
            buffer[buf_idx] = current; 
            buf_idx++;
        } 
        else {
            /* Carattere normale (o separatore iniziale): accumula nel buffer */
            buffer[buf_idx] = current;
            buf_idx++;
        }
    }

    /* --- GESTIONE RESIDUO --- */
    /* Se alla fine del loop c'è qualcosa nel buffer, creiamo l'ultimo nodo */
    if (buf_idx > 0) {
        buffer[buf_idx] = '\0';
        char *node_str = my_strdup_range(buffer, buf_idx);
        list_append(l, node_str);
    }

    /* Pulizia memoria temporanea */
    free(buffer);
}

/* --- 4. FUNZIONI DI OUTPUT E PULIZIA --- */

void list_print(LIST l) {
    if (l == NULL) return;
    Node *curr = l->head;
    while (curr != NULL) {
        printf("%s", curr->data);
        if (curr->next != NULL) printf(" -> ");
        curr = curr->next;
    }
    printf("\n");
}

void list_destroy(LIST l) {
    if (l == NULL) return;
    Node *curr = l->head;
    while (curr != NULL) {
        Node *temp = curr;
        curr = curr->next;
        free(temp->data); /* Libera la stringa */
        free(temp);       /* Libera il nodo */
    }
    free(l); /* Libera la struttura ADT */
}