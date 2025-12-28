#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

/* * DEFINIZIONE NODO (Privata)
 * Doppiamente linkata: ha next e prev.
 */
struct nodo {
    int info;
    struct nodo *next;
    struct nodo *prev;
};

/* * DEFINIZIONE RECORD LISTA (Privata)
 * Questo è il "trucco" per l'ADT di I classe.
 * LIST è un puntatore a questa struttura.
 * Questa struttura contiene il puntatore alla testa.
 * In questo modo, passando 'l' (puntatore al record), possiamo modificare 'l->head'.
 */
struct record_lista {
    struct nodo *head;
    // Potremmo mettere anche *tail o un contatore qui
};

/* --- Funzioni Helper per creare l'ambiente di test --- */
LIST crea_lista_vuota() {
    LIST l = (LIST)malloc(sizeof(struct record_lista));
    l->head = NULL;
    return l;
}

void inserisci_testa(LIST l, int val) {
    struct nodo *nuovo = (struct nodo *)malloc(sizeof(struct nodo));
    nuovo->info = val;
    nuovo->prev = NULL;
    nuovo->next = l->head;
    if (l->head != NULL) {
        l->head->prev = nuovo;
    }
    l->head = nuovo;
}

/*
 * ======================================================================================
 * FUNZIONE RICHIESTA: f
 * ======================================================================================
 *
 * Scopo:
 * Elimina dalla lista doppiamente linkata tutti i nodi con valore compreso tra [a, b].
 *
 * Parametri:
 * - LIST l: Il puntatore al record che gestisce la lista (ADT).
 * - int a, int b: Gli estremi dell'intervallo (inclusi).
 *
 * Concetti Chiave:
 * 1. Iterazione sicura: Quando cancelliamo un nodo, perdiamo il puntatore al successivo.
 * Dobbiamo salvarci il 'next' PRIMA di fare la free.
 * 2. Rilinkaggio doppio: Se rimuoviamo X in A <-> X <-> B, dobbiamo collegare A->next a B
 * e B->prev ad A.
 * 3. Gestione della Testa: Se rimuoviamo il primo nodo, dobbiamo aggiornare l->head.
 * ======================================================================================
 */
void f(LIST l, int a, int b) {
    /* Controllo di sicurezza: se la struttura lista non esiste, esco */
    if (l == NULL) return;

    /* --- 1. INIZIALIZZAZIONE --- */
    struct nodo *curr = l->head; // Parto dal primo nodo effettivo
    struct nodo *temp_next;      // Variabile d'appoggio essenziale

    /* --- 2. CICLO DI SCANSIONE --- */
    while (curr != NULL) {
        
        /* Controllo se il valore è nell'intervallo [a, b] */
        if (curr->info >= a && curr->info <= b) {
            
            /* --- 3. LOGICA DI CANCELLAZIONE --- */
            
            // Salviamo il prossimo nodo, perché 'curr' sta per essere distrutto
            temp_next = curr->next;

            // CASO A: Il nodo ha un predecessore (non è la testa)
            if (curr->prev != NULL) {
                // Il 'next' del precedente deve puntare al successivo del corrente
                curr->prev->next = curr->next;
            } else {
                // CASO B: Il nodo è la testa (prev è NULL)
                // Dobbiamo aggiornare il puntatore head nel record principale
                l->head = curr->next;
            }

            // CASO C: Il nodo ha un successore (non è la coda)
            if (curr->next != NULL) {
                // Il 'prev' del successivo deve puntare al precedente del corrente
                curr->next->prev = curr->prev;
            }

            /* --- 4. GESTIONE MEMORIA --- */
            // Ora che il nodo è isolato (scollegato), possiamo liberarlo
            free(curr);

            // Avanziamo usando il puntatore salvato
            curr = temp_next;
            
        } else {
            /* --- 5. AVANZAMENTO STANDARD --- */
            // Il nodo non va cancellato, passo semplicemente al prossimo
            curr = curr->next;
        }
    }
}