#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * DEFINIZIONE TIPI DI DATO (ADT)
 * ============================================================================
 * L'esercizio richiede un "ADT di prima categoria". In C, questo si realizza
 * nascondendo i dettagli implementativi del nodo e fornendo una struttura
 * "wrapper" (contenitore) che rappresenta la lista stessa.
 */

/* 1. Definizione del Nodo (Struttura interna) */
typedef struct node {
    char *cognome;          // Stringa dinamica
    char *nome;             // Stringa dinamica
    struct node *next;      // Puntatore al prossimo elemento
} node_t;

/* 2. Definizione dell'ADT Lista (Wrapper) */
/*
 * Perché questa struttura?
 * La funzione riceve 'lista_t *lista'. Se 'lista_t' fosse direttamente il nodo,
 * avremmo problemi a modificare la testa della lista (dovremmo usare doppi puntatori).
 * Usando una struct che CONTIENE il puntatore alla testa (head), possiamo
 * modificare 'head' pur passando il puntatore alla struttura wrapper.
 */
typedef struct list {
    node_t *head;
} lista_t;

/* ============================================================================
 * FUNZIONI HELPER (Non richieste esplicitamente, ma necessarie per l'ADT)
 * ============================================================================
 */

/* Inizializza una lista vuota */
void initList(lista_t *l) {
    if (l != NULL) l->head = NULL;
}

/* ============================================================================
 * FUNZIONE: inserisciInOrdine
 * ============================================================================
 * SCOPO:
 * Inserisce una nuova persona nella lista mantenendo l'ordine alfabetico
 * (chiave primaria: cognome, chiave secondaria: nome).
 * Gestisce l'allocazione dinamica delle stringhe e controlla i duplicati.
 *
 * PARAMETRI:
 * - lista_t *lista: Puntatore alla struttura che gestisce la lista (ADT).
 * - char *cognome:  Stringa sorgente per il cognome.
 * - char *nome:     Stringa sorgente per il nome.
 *
 * RITORNO:
 * - int: 1 se l'inserimento avviene con successo, 0 se il dato è già presente
 * (duplicato) o se c'è un errore di memoria.
 *
 * CONCETTI CHIAVE:
 * 1. Deep Copy (Copia Profonda):
 * Non copiamo i puntatori passati in input! I dati originali potrebbero
 * essere variabili locali dello stack del main. Dobbiamo allocare nuova
 * memoria (malloc) e copiare i caratteri (strcpy).
 *
 * 2. Scansione con due puntatori (curr e prev):
 * Per inserire in mezzo a una lista singolarmente concatenata, dobbiamo
 * sapere chi è il nodo "precedente" per aggiornare il suo 'next'.
 * ============================================================================
 */
int inserisciInOrdine(lista_t *lista, char *cognome, char *nome) {

    /* --- 1. CONTROLLO INPUT --- */
    if (lista == NULL || cognome == NULL || nome == NULL) return 0;

    /* --- 2. RICERCA POSIZIONE DI INSERIMENTO --- */
    node_t *curr = lista->head; // Puntatore corrente (scorre la lista)
    node_t *prev = NULL;        // Puntatore al nodo precedente (inizialmente NULL)

    int comparison = 0; // Variabile per salvare il risultato di strcmp

    /*
     * Loop di scansione:
     * Continuiamo finché ci sono nodi (curr != NULL)
     */
    while (curr != NULL) {
        /*
         * LOGICA DI CONFRONTO:
         * 1. Confrontiamo i cognomi.
         */
        comparison = strcmp(curr->cognome, cognome);

        /* Caso A: Il cognome nella lista è "maggiore" di quello da inserire.
         * Es: lista ha "Verdi", noi inseriamo "Bianchi".
         * Abbiamo trovato il punto di inserimento (prima di curr). Usciamo dal loop.
         */
        if (comparison > 0) {
            break; 
        }

        /* Caso B: I cognomi sono uguali. Dobbiamo controllare il nome. */
        if (comparison == 0) {
            int nome_comp = strcmp(curr->nome, nome);
            
            if (nome_comp > 0) {
                /* Il nome in lista è "maggiore". Es: "Rossi Mario" vs "Rossi Luigi".
                 * Inseriamo qui. Break. */
                break;
            } else if (nome_comp == 0) {
                /* Cognome E Nome identici. È un DUPLICATO.
                 * Ritorniamo 0 come richiesto */
                return 0; 
            }
            /* Se nome_comp < 0, continuiamo a scorrere (es: "Rossi Alberto" viene prima di "Rossi Luigi") */
        }

        /* Avanzamento puntatori */
        prev = curr;
        curr = curr->next;
    }

    /* --- 3. ALLOCAZIONE NUOVO NODO --- */
    /* Alloco la struttura del nodo */
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) return 0; // Errore memoria

    /* --- 4. ALLOCAZIONE STRINGHE (DEEP COPY) --- */
    /*
     * Dettaglio Critico:
     * sizeof(char) è 1.
     * strlen(s) conta i caratteri ESCLUSO il terminatore '\0'.
     * Quindi dobbiamo sempre allocare (strlen + 1).
     */
    new_node->cognome = (char *)malloc((strlen(cognome) + 1) * sizeof(char));
    new_node->nome = (char *)malloc((strlen(nome) + 1) * sizeof(char));

    /* Controllo se le malloc delle stringhe sono andate a buon fine */
    if (new_node->cognome == NULL || new_node->nome == NULL) {
        /* Pulizia parziale per evitare memory leak se una malloc fallisce */
        free(new_node->cognome); // Safe anche se NULL
        free(new_node->nome);    // Safe anche se NULL
        free(new_node);
        return 0;
    }

    /* Copia effettiva dei dati */
    strcpy(new_node->cognome, cognome);
    strcpy(new_node->nome, nome);

    /* --- 5. COLLEGAMENTO DEL NODO (LINKING) --- */
    
    /* Il nuovo nodo punterà sempre al 'curr' (che può essere un nodo o NULL) */
    new_node->next = curr;

    /*
     * Caso 1: Inserimento in TESTA
     * Se prev è ancora NULL, significa che:
     * a) La lista era vuota.
     * b) Siamo usciti dal loop subito perché il nuovo elemento è il "più piccolo" di tutti.
     */
    if (prev == NULL) {
        // Accediamo al campo 'head' dell'ADT e lo aggiorniamo
        lista->head = new_node;
    } 
    /*
     * Caso 2: Inserimento nel MEZZO o in CODA
     * Il nodo precedente deve puntare al nuovo nodo.
     */
    else {
        prev->next = new_node;
    }

    return 1; // Successo
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Gestione Memoria:
 * 1. Creiamo l'ADT lista.
 * 2. Inseriamo dati (anche duplicati per test).
 * 3. Stampiamo.
 * 4. Liberiamo TUTTO (Nodi + Stringhe Cognome + Stringhe Nome).
 * ============================================================================
 */
int main() {
    /* Creazione istanza ADT */
    lista_t rubrica;
    initList(&rubrica);

    /* --- 1. TEST INSERIMENTO --- */
    printf("--- Inserimento Dati ---\n");
    
    // Inserimento disordinato per testare l'ordinamento
    if (inserisciInOrdine(&rubrica, "Rossi", "Mario")) printf("Inserito: Rossi Mario\n");
    if (inserisciInOrdine(&rubrica, "Verdi", "Giuseppe")) printf("Inserito: Verdi Giuseppe\n");
    if (inserisciInOrdine(&rubrica, "Bianchi", "Antonio")) printf("Inserito: Bianchi Antonio\n");
    
    // Test caso "Stesso Cognome, Nome diverso"
    if (inserisciInOrdine(&rubrica, "Rossi", "Luigi")) printf("Inserito: Rossi Luigi\n");
    
    // Test caso "Duplicato" (Non deve inserire)
    if (!inserisciInOrdine(&rubrica, "Verdi", "Giuseppe")) printf("Rifiutato Duplicato: Verdi Giuseppe\n");

    /* --- 2. STAMPA VERIFICA --- */
    printf("\n--- Contenuto Lista (Atteso ordine alfabetico) ---\n");
    node_t *temp = rubrica.head;
    while (temp != NULL) {
        printf("%s %s\n", temp->cognome, temp->nome);
        temp = temp->next;
    }

    /* --- 3. LIBERAZIONE MEMORIA (FREE) --- */
    /*
     * Regola: Per ogni malloc, una free.
     * Abbiamo allocato: Nodo, Cognome, Nome. Dobbiamo liberare 3 cose per elemento.
     */
    node_t *current = rubrica.head;
    node_t *next_node;
    
    while (current != NULL) {
        next_node = current->next; // Salviamo il riferimento al prossimo
        
        free(current->cognome);    // 1. Libero stringa cognome
        free(current->nome);       // 2. Libero stringa nome
        free(current);             // 3. Libero il nodo strutturale
        
        current = next_node;
    }
    printf("\nMemoria liberata con successo.\n");

    return 0;
}