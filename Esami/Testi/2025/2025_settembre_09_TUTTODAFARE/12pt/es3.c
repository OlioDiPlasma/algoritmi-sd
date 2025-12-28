#include <stdio.h>
#include <stdlib.h>

/* ========================================================================= */
/* 1. DEFINIZIONE ADT GRAFO (Liste di Adiacenza)                             */
/* ========================================================================= */

/*
 * Struttura del Nodo della lista di adiacenza.
 * Rappresenta un "amico" nella lista di un vertice.
 */
typedef struct node *link;
struct node {
    int v;      /* Indice del vertice (l'amico) */
    link next;  /* Puntatore al prossimo amico nella lista */
};

/*
 * Struttura del Grafo.
 * V: numero di persone (vertici).
 * E: numero di relazioni (archi).
 * ladj: Vettore di liste. ladj[i] è la lista degli amici di i.
 */
struct graph {
    int V;
    int E;
    link *ladj; 
};
typedef struct graph *Graph;

/* ========================================================================= */
/* 2. FUNZIONI DI SUPPORTO (Creazione, Distruzione, Inserimento)             */
/* ========================================================================= */

/* Crea un nodo della lista */
link NEW(int v, link next) {
    link x = malloc(sizeof *x);
    if (x == NULL) return NULL; 
    x->v = v;
    x->next = next;
    return x;
}

/* Inizializza il Grafo vuoto */
Graph GRAPHinit(int V) {
    Graph G = malloc(sizeof *G);
    G->V = V;
    G->E = 0;
    /* Allocazione dell'array di puntatori (le teste delle liste) */
    /* sizeof(link) è la dimensione di un puntatore */
    G->ladj = malloc(V * sizeof(link)); 
    for (int i = 0; i < V; i++)
        G->ladj[i] = NULL;
    return G;
}

/* * Inserisce un arco NON ORIENTATO (amicizia reciproca).
 * Aggiunge w alla lista di v, e v alla lista di w.
 */
void GRAPHinsertE(Graph G, int v, int w) {
    G->ladj[v] = NEW(w, G->ladj[v]);
    G->ladj[w] = NEW(v, G->ladj[w]); /* Poiché il grafo non è orientato */
    G->E++;
}

/* Libera la memoria */
void GRAPHfree(Graph G) {
    for (int v = 0; v < G->V; v++) {
        link t = G->ladj[v];
        while (t != NULL) {
            link temp = t;
            t = t->next;
            free(temp);
        }
    }
    free(G->ladj);
    free(G);
}

/* ========================================================================= */
/* 3. SOLUZIONE DELL'ESERCIZIO                                               */
/* ========================================================================= */

/*
 * Funzione: GRAPHfriendsOfFriends
 * -------------------------------------------------------------------------
 * Scopo:       Trovare, contare e stampare gli amici degli amici di 'v'
 * che NON sono già amici di 'v' e non sono 'v' stesso.
 * Parametri:
 * - g: il grafo (la rete sociale).
 * - v: il vertice di partenza (la persona di cui analizziamo le conoscenze).
 * * Concetti Chiave:
 * - Vettore di Marcatori (Array diretto): Usiamo un array `status` per tenere 
 * traccia dello stato di ogni nodo. Questo evita cicli annidati costosi per 
 * controllare "è già amico?".
 * Stati possibili in `status[i]`:
 * 0 = Sconosciuto / Non visitato.
 * 1 = Amico diretto (o me stesso). DA IGNORARE.
 * 2 = Amico di amico già contato. (Serve per non contare/stampare doppi).
 * -------------------------------------------------------------------------
 */
int GRAPHfriendsOfFriends(Graph g, int v) {
    /* --- 1. CONTROLLI INIZIALI --- */
    if (g == NULL) return 0;
    if (v < 0 || v >= g->V) return 0;

    int count = 0;
    link t; /* Puntatore per scorrere le liste (iteratore) */

    /* --- 2. ALLOCAZIONE ARRAY DI SUPPORTO --- */
    /*
     * MALLOC: Allochiamo un array di interi grande quanto il numero di vertici.
     * Usiamo calloc per inizializzare tutto a 0 automaticamente.
     * status[i] ci dirà la relazione tra 'v' e 'i'.
     */
    int *status = calloc(g->V, sizeof(int));
    if (status == NULL) return 0; /* Gestione errore memoria */

    /* --- 3. MARCATURA AMICI DIRETTI --- */
    /* Prima fase: Identifichiamo chi NON dobbiamo contare.
       Noi stessi (v) e i nostri amici diretti. */
    
    status[v] = 1; /* Escludo me stesso */

    /* Scorriamo la lista degli amici di v */
    /* t punta al primo nodo; finché t non è NULL; t avanza al prossimo */
    for (t = g->ladj[v]; t != NULL; t = t->next) {
        /* t->v è l'indice dell'amico diretto */
        status[t->v] = 1; /* 1 significa: "Non considerare come amico di amico" */
    }

    /* --- 4. RICERCA AMICI DI AMICI --- */
    /* Ora iteriamo DI NUOVO sugli amici diretti di v.
       Per ogni amico diretto, guardiamo i SUOI amici. */
    
    printf("Amici di amici di %d: ", v);

    for (t = g->ladj[v]; t != NULL; t = t->next) {
        int friend_id = t->v; /* Questo è un amico diretto */

        /* Adesso scorriamo la lista degli amici dell'amico (livello 2) */
        link t2;
        for (t2 = g->ladj[friend_id]; t2 != NULL; t2 = t2->next) {
            int fof_id = t2->v; /* Friend of Friend candidate */

            /*
             * CONTROLLO LOGICO:
             * Se status[fof_id] == 0, significa che:
             * 1. Non è v (che ha status 1).
             * 2. Non è un amico diretto (che ha status 1).
             * 3. Non è stato ancora conteggiato (non ha status 2).
             */
            if (status[fof_id] == 0) {
                printf("%d ", fof_id); /* Stampa richiesta */
                count++;               /* Incrementa contatore */
                status[fof_id] = 2;    /* Marcalo come "già contato" per evitare duplicati */
            }
        }
    }
    printf("\n");

    /* --- 5. PULIZIA MEMORIA --- */
    /* Fondamentale: l'array status è temporaneo, va liberato prima di uscire */
    free(status);

    return count;
}

/* ========================================================================= */
/* 4. MAIN DI TEST                                                           */
/* ========================================================================= */

int main() {
    printf("--- LEZIONE: Grafi e Amici di Amici ---\n");

    /* * Creiamo una rete sociale di prova:
     * 0 è amico di 1 e 2.
     * 1 è amico di 0 e 3.
     * 2 è amico di 0.
     * 3 è amico di 1 e 4.
     * 4 è amico di 3.
     * * Visualizzazione:
     * 2 -- 0 -- 1 -- 3 -- 4
     * * Analisi per il nodo 0:
     * - Amici diretti: 1, 2.
     * - Amici di 1: {0, 3} -> 0 è scartato (sé stesso), 3 è NUOVO.
     * - Amici di 2: {0}    -> 0 è scartato.
     * - Risultato atteso per 0: Solo 3. (4 è troppo lontano, distanza 3).
     */

    int V = 5;
    Graph socialNetwork = GRAPHinit(V);

    /* Inserimento archi (amicizie) */
    GRAPHinsertE(socialNetwork, 0, 1);
    GRAPHinsertE(socialNetwork, 0, 2);
    GRAPHinsertE(socialNetwork, 1, 3);
    GRAPHinsertE(socialNetwork, 3, 4);

    /* Aggiungiamo un caso interessante:
       Supponiamo che anche 2 sia amico di 3.
       Ora il grafo ha un ciclo: 0-1-3-2-0.
       Friends of Friends di 0:
       - Via 1 arrivo a 3.
       - Via 2 arrivo a 3.
       3 deve essere contato UNA SOLA VOLTA.
    */
    GRAPHinsertE(socialNetwork, 2, 3);

    printf("Analisi amici di amici per il nodo 0 (con ciclo aggiunto)...\n");
    /*
     * Situazione attuale:
     * 0 conosce {1, 2}
     * 1 conosce {0, 3}
     * 2 conosce {0, 3}
     * 3 conosce {1, 2, 4}
     * * FoF di 0:
     * Da 1 trovo 3.
     * Da 2 trovo 3 (già visto!).
     * 4 è amico di 3, quindi è amico di amico di amico (distanza 3), NON deve apparire.
     * Risultato atteso: Solo 3.
     */
    int result = GRAPHfriendsOfFriends(socialNetwork, 0);
    printf("Numero totale trovato: %d (Atteso: 1, ovvero il nodo 3)\n", result);

    printf("\nAnalisi amici di amici per il nodo 1...\n");
    /*
     * 1 conosce {0, 3}
     * Amici di 0: {1, 2} -> 2 è valido (distanza 2 da 1).
     * Amici di 3: {1, 2, 4} -> 2 è valido (già preso), 4 è valido (distanza 2 da 1).
     * Risultato atteso: 2 e 4.
     */
    result = GRAPHfriendsOfFriends(socialNetwork, 1);
    printf("Numero totale trovato: %d (Atteso: 2, ovvero nodi 2 e 4)\n", result);

    /* Pulizia finale */
    GRAPHfree(socialNetwork);
    printf("\nMemoria liberata. Lezione conclusa.\n");

    return 0;
}