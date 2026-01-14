#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "anagrafica.h"

/* =========================================================================
   SEZIONE 1: FUNZIONI DI UTILITA' E GESTIONE ITEM
   ========================================================================= */

/*
 * Scopo: Confrontare due date per stabilire l'anzianita'.
 * Parametri: d1, d2 (le due date).
 * Ritorna: 
 * < 0 se d1 e' precedente a d2 (d1 e' piu' anziano)
 * 0 se uguali
 * > 0 se d1 e' successiva a d2
 */
int confrontaDate(Data d1, Data d2) {
    if (d1.a != d2.a) return d1.a - d2.a;
    if (d1.m != d2.m) return d1.m - d2.m;
    return d1.g - d2.g;
}

/*
 * Scopo: Legge i campi di una persona.
 * File di Input: Funziona sia con stdin (tastiera) che con file di testo aperti.
 * Concetti Chiave: fscanf ritorna il numero di campi letti correttamente.
 */
Item leggiItem(FILE *fp) {
    Item t;
    // Formato lettura: stringhe senza spazi e data nel formato gg/mm/aaaa
    // Nota: usiamo fscanf anche per la tastiera (fp = stdin)
    if (fscanf(fp, "%s %s %s %d/%d/%d %s %s %d", 
        t.codice, t.nome, t.cognome, 
        &t.data_nascita.g, &t.data_nascita.m, &t.data_nascita.a,
        t.via, t.citta, &t.cap) != 9) {
        
        // Se la lettura fallisce, ritorniamo un item "vuoto" riconoscibile
        strcpy(t.codice, "ERR");
    }
    return t;
}

/*
 * Scopo: Stampa formattata di un elemento.
 */
void stampaItem(Item item, FILE *fp) {
    fprintf(fp, "%-6s %-15s %-15s %02d/%02d/%04d %-20s %-15s %d\n",
            item.codice, item.nome, item.cognome,
            item.data_nascita.g, item.data_nascita.m, item.data_nascita.a,
            item.via, item.citta, item.cap);
}

Item creaItemVuoto() {
    Item t;
    strcpy(t.codice, ""); // Codice vuoto segnala errore/fine
    return t;
}

/* =========================================================================
   SEZIONE 2: GESTIONE LISTA (CUORE DELL'ESERCIZIO)
   ========================================================================= */

/*
 * Scopo: Inserire un elemento mantenendo l'ordine di anzianita' (data crescente).
 * Parametri: 
 * - head: Doppio puntatore (Lista *) perche' potremmo cambiare la testa della lista.
 * - val: Il dato Item da inserire.
 * Gestione Memoria: Allocazione dinamica con malloc.
 */
void inserisciOrdinato(Lista *head, Item val) {
    /* --- 1. ALLOCAZIONE --- */
    // Chiediamo al sistema operativo lo spazio per un nodo
    Nodo *nuovo = (Nodo *)malloc(sizeof(Nodo));
    if (nuovo == NULL) {
        printf("Errore di allocazione memoria.\n");
        return;
    }
    nuovo->info = val; // Copiamo i dati nella nuova cella
    nuovo->next = NULL;

    /* --- 2. LOGICA DI INSERIMENTO --- */
    // Caso 1: Lista vuota o inserimento in testa (nuovo e' il piu' anziano)
    // Dereferenziamo (*head) per vedere dove punta l'inizio della lista attuale
    if (*head == NULL || confrontaDate(val.data_nascita, (*head)->info.data_nascita) < 0) {
        nuovo->next = *head; // Il nuovo punta al vecchio primo
        *head = nuovo;       // Aggiorniamo il puntatore testa (modifica permamente grazie al doppio puntatore)
    } 
    else {
        // Caso 2: Inserimento nel mezzo o in coda
        Nodo *corr = *head;
        // Scorriamo finche' c'e' un prossimo nodo E quel nodo e' piu' "vecchio" (data minore) del nuovo
        while (corr->next != NULL && confrontaDate(val.data_nascita, corr->next->info.data_nascita) >= 0) {
            corr = corr->next;
        }
        // Inserimento chirurgico
        nuovo->next = corr->next;
        corr->next = nuovo;
    }
}

/*
 * Scopo: Trovare un elemento dato il codice.
 * Ritorna: Puntatore al nodo trovato o NULL.
 */
Lista ricercaCodice(Lista head, char *codice) {
    while (head != NULL) {
        if (strcmp(head->info.codice, codice) == 0) {
            return head; // Trovato
        }
        head = head->next;
    }
    return NULL; // Non trovato
}

/*
 * Scopo: Rimuovere un elemento specifico e restituirne il valore.
 * Concetti Chiave: 
 * - Serve un doppio puntatore perche' potremmo cancellare il primo elemento.
 * - Dobbiamo fare "free" del nodo, ma ritornare l'Item (copia).
 */
Item estraiPerCodice(Lista *head, char *codice) {
    Nodo *curr = *head;
    Nodo *prev = NULL;
    Item trovato = creaItemVuoto();

    while (curr != NULL) {
        // Se troviamo il codice
        if (strcmp(curr->info.codice, codice) == 0) {
            trovato = curr->info; // Salviamo i dati prima di distruggere il nodo

            /* --- CANCELLAZIONE FISICA --- */
            if (prev == NULL) {
                // Stiamo cancellando la testa
                *head = curr->next;
            } else {
                // Stiamo cancellando in mezzo o coda
                prev->next = curr->next;
            }
            
            free(curr); // IMPORTANTE: Liberiamo la memoria mallocata per questo nodo
            return trovato;
        }
        // Avanzamento
        prev = curr;
        curr = curr->next;
    }
    return trovato; // Ritorna item vuoto se non trovato
}

/*
 * Scopo: Estrarre IL PRIMO elemento trovato nel range di date.
 * Logica: Il testo chiede una funzione che estragga UN solo elemento. 
 * Il main la chiamera' ciclicamente.
 * Parametri: head (doppio puntatore), d1 (inizio), d2 (fine).
 */
Item estraiIntervalloDate(Lista *head, Data d1, Data d2) {
    Nodo *curr = *head;
    Nodo *prev = NULL;
    Item estratto = creaItemVuoto();

    while (curr != NULL) {
        // Verifica se la data corrente e' compresa tra d1 e d2 (inclusi)
        if (confrontaDate(curr->info.data_nascita, d1) >= 0 && 
            confrontaDate(curr->info.data_nascita, d2) <= 0) {
            
            estratto = curr->info; // Salviamo il dato

            // Scolleghiamo il nodo dalla lista
            if (prev == NULL) {
                *head = curr->next;
            } else {
                prev->next = curr->next;
            }

            free(curr); // Liberiamo memoria
            return estratto; // Ritorniamo al chiamante che ne fara' l'uso che vuole
        }
        
        prev = curr;
        curr = curr->next;
    }
    
    return estratto; // Se arriviamo qui, non c'e' nulla nell'intervallo
}

void stampaListaSuFile(Lista head, char *nomeFile) {
    FILE *fp = fopen(nomeFile, "w");
    if (fp == NULL) {
        printf("Errore apertura file %s per scrittura.\n", nomeFile);
        return;
    }
    
    //     // Scorriamo la lista fino alla fine
    while (head != NULL) {
        stampaItem(head->info, fp);
        head = head->next;
    }
    fclose(fp);
    printf("Lista salvata correttamente su %s\n", nomeFile);
}

void liberaLista(Lista head) {
    Nodo *tmp;
    while (head != NULL) {
        tmp = head;       // Salviamo il riferimento al nodo corrente
        head = head->next; // Avanziamo alla prossima scatola
        free(tmp);        // Distruggiamo la scatola corrente
    }
}