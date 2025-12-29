/*
 * File: gestione_sci.c
 * Autore: Il tuo Professore di C
 */

#include "gestione_sci.h"

/* --- SEZIONE 1: FUNZIONI AUSILIARIE (HELPER) --- */

/*
 * Funzione: trova_impianto
 * Scopo: Cerca un impianto nella lista lineare.
 * Costo: O(M) dove M e' il numero di impianti (molto piccolo).
 */
Skilift* trova_impianto(Skilift *head, char *id) {
    Skilift *curr = head;
    while (curr != NULL) {
        // strcmp restituisce 0 se le stringhe sono uguali
        if (strcmp(curr->id, id) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

/*
 * Funzione: trova_o_crea_sciatore
 * Scopo: Cerca uno sciatore nel BST globale. Se non esiste, lo crea.
 * Parametri:
 * - root: Doppio puntatore (NodoSciatore**) perche' potremmo dover cambiare 
 * la radice se l'albero e' vuoto o se inseriamo un nuovo nodo.
 * Costo: O(log N) grazie alla struttura ad albero.
 */
NodoSciatore* trova_o_crea_sciatore(NodoSciatore **root, long cardId) {
    if (*root == NULL) {
        /* CASO BASE: Nodo non trovato o albero vuoto -> CREAZIONE */
        /* * Allocazione dinamica (malloc): chiediamo spazio nello Heap.
         * sizeof(NodoSciatore) calcola i byte necessari per la struct.
         */
        NodoSciatore *nuovo = (NodoSciatore*)malloc(sizeof(NodoSciatore));
        if (nuovo == NULL) {
            printf("Errore: Memoria esaurita!\n");
            exit(1);
        }
        nuovo->cardId = cardId;
        nuovo->storia = NULL; // All'inizio non ha usato impianti
        nuovo->left = NULL;
        nuovo->right = NULL;
        
        // Colleghiamo il nuovo nodo al puntatore padre
        *root = nuovo;
        return nuovo;
    }

    /* RICORSIONE: Navigazione dell'albero */
    if (cardId < (*root)->cardId) {
        return trova_o_crea_sciatore(&((*root)->left), cardId);
    } else if (cardId > (*root)->cardId) {
        return trova_o_crea_sciatore(&((*root)->right), cardId);
    } else {
        /* Trovato! */
        return *root;
    }
}

/*
 * Funzione: aggiorna_statistiche_impianto
 * Scopo: Inserisce o aggiorna il contatore di uno sciatore nel BST dell'impianto.
 * Costo: O(log N).
 */
void aggiorna_statistiche_impianto(NodoStatistica **root, long cardId) {
    if (*root == NULL) {
        // Sciatore mai visto su questo impianto -> Creo nodo
        NodoStatistica *nuovo = (NodoStatistica*)malloc(sizeof(NodoStatistica));
        nuovo->cardId = cardId;
        nuovo->contatore = 1; // Prima volta
        nuovo->left = NULL;
        nuovo->right = NULL;
        *root = nuovo;
        return;
    }

    if (cardId < (*root)->cardId) {
        aggiorna_statistiche_impianto(&((*root)->left), cardId);
    } else if (cardId > (*root)->cardId) {
        aggiorna_statistiche_impianto(&((*root)->right), cardId);
    } else {
        // Sciatore gia' presente -> Incremento contatore
        (*root)->contatore++;
    }
}

/* --- SEZIONE 2: LETTURA FILE --- */

Skilift* carica_skilift(const char *nomeFile) {
    FILE *fp = fopen(nomeFile, "r");
    if (fp == NULL) {
        printf("Errore: Impossibile aprire il file %s\n", nomeFile);
        return NULL;
    }

    Skilift *head = NULL;
    char tempId[100]; // Buffer temporaneo
    int intervallo;

    /*
     * fscanf legge formattato dal file.
     * Restituisce il numero di campi letti correttamente (qui deve essere 2).
     */
    while (fscanf(fp, "%s %d", tempId, &intervallo) == 2) {
        // Creo nuovo nodo Skilift
        Skilift *nuovo = (Skilift*)malloc(sizeof(Skilift));
        
        // Copio la stringa in modo sicuro. strncpy evita buffer overflow.
        strncpy(nuovo->id, tempId, 10);
        nuovo->id[10] = '\0'; // Assicuro il terminatore
        
        nuovo->intervalloMin = intervallo;
        nuovo->utentiAbilitati = NULL; // Inizializzo il BST interno
        
        // Inserimento in testa alla lista (piu' semplice ed efficiente O(1))
        nuovo->next = head;
        head = nuovo;
    }

    fclose(fp);
    return head;
}

/* --- SEZIONE 3: LOGICA CORE (Authorize) --- */

int authorize(long cardId, char *skiliftId, int time, Skilift *listaImpianti, NodoSciatore **alberoSciatori) {
    
    // 1. Trovo l'impianto (se non esiste, nego accesso per sicurezza)
    Skilift *impianto = trova_impianto(listaImpianti, skiliftId);
    if (impianto == NULL) {
        printf("Impianto %s non trovato.\n", skiliftId);
        return 0; // Accesso negato
    }

    // 2. Trovo o creo lo sciatore nel sistema globale (O(log N))
    NodoSciatore *sciatore = trova_o_crea_sciatore(alberoSciatori, cardId);

    // 3. Verifico la storia dello sciatore per questo impianto
    StoriaUtilizzo *currStoria = sciatore->storia;
    StoriaUtilizzo *nodoStoriaTrovato = NULL;
    int puoPassare = 1; // Assumo true, poi verifico vincoli

    while (currStoria != NULL) {
        if (strcmp(currStoria->skiliftId, skiliftId) == 0) {
            // Trovato un utilizzo precedente di questo impianto
            nodoStoriaTrovato = currStoria;
            int tempoTrascorso = time - currStoria->lastTime;
            
            // CONTROLLO DEL VINCOLO TEMPORALE
            if (tempoTrascorso < impianto->intervalloMin) {
                puoPassare = 0; // Troppo presto!
            }
            break; // Ho trovato l'impianto, inutile continuare a cercare nella storia
        }
        currStoria = currStoria->next;
    }

    // 4. Se autorizzato, aggiorno le strutture dati
    if (puoPassare) {
        // A. Aggiorno o aggiungo la voce nella storia dello sciatore
        if (nodoStoriaTrovato != NULL) {
            // C'era gia', aggiorno solo l'ora
            nodoStoriaTrovato->lastTime = time;
        } else {
            // Prima volta su questo impianto per questo sciatore -> Aggiungo in testa alla lista storia
            StoriaUtilizzo *nuovaStoria = (StoriaUtilizzo*)malloc(sizeof(StoriaUtilizzo));
            strcpy(nuovaStoria->skiliftId, skiliftId);
            nuovaStoria->lastTime = time;
            nuovaStoria->next = sciatore->storia;
            sciatore->storia = nuovaStoria;
        }

        // B. Aggiorno le statistiche dentro l'impianto (Richiesto dal testo)
        // Aggiorno il BST interno all'impianto: O(log N)
        aggiorna_statistiche_impianto(&(impianto->utentiAbilitati), cardId);
    }

    return puoPassare;
}

/* --- SEZIONE 4: GESTIONE MEMORIA (PULIZIA) --- */

/* Helper per liberare ricorsivamente un albero BST */
void free_bst_stats(NodoStatistica *root) {
    if (root == NULL) return;
    free_bst_stats(root->left);
    free_bst_stats(root->right);
    free(root);
}

void free_storia(StoriaUtilizzo *head) {
    while (head != NULL) {
        StoriaUtilizzo *temp = head;
        head = head->next;
        free(temp);
    }
}

void free_bst_sciatori(NodoSciatore *root) {
    if (root == NULL) return;
    free_bst_sciatori(root->left);
    free_bst_sciatori(root->right);
    free_storia(root->storia); // Libero la lista interna
    free(root);
}

void libera_tutto(Skilift *listaImpianti, NodoSciatore *alberoSciatori) {
    // 1. Libero sciatori
    free_bst_sciatori(alberoSciatori);

    // 2. Libero impianti
    while (listaImpianti != NULL) {
        Skilift *temp = listaImpianti;
        listaImpianti = listaImpianti->next;
        free_bst_stats(temp->utentiAbilitati); // Libero BST interno all'impianto
        free(temp);
    }
}