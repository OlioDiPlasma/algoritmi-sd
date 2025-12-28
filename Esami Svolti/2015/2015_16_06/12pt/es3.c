#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- DEFINIZIONE STRUTTURE DATI (ADT) --- */

/* * Struttura per la lista secondaria: PRODOTTO 
 */
struct Prodotto {
    char nome[31];          /* Nome del prodotto (max 30 char + \0) */
    float prezzo;           /* Valore reale */
    struct Prodotto *next;  /* Puntatore al prossimo prodotto */
};

/* * Struttura per la lista principale: PRODUTTORE 
 */
struct Produttore {
    char nome[31];              /* Nome del produttore */
    struct Prodotto *prodotti;  /* PUNTATORE alla testa della lista dei prodotti */
    struct Produttore *next;    /* Puntatore al prossimo produttore */
};

/*
 * ======================================================================================
 * NOME FUNZIONE: cercaProdottoRicorsiva (Helper)
 * ======================================================================================
 * SCOPO:
 * Funzione ausiliaria ricorsiva che cerca un prodotto specifico all'interno della
 * lista dei prodotti di un singolo produttore.
 *
 * PARAMETRI:
 * * head : Puntatore alla testa della lista dei prodotti.
 * * nomeProdotto : Stringa da cercare.
 *
 * CONCETTI CHIAVE:
 * 1. Ordinamento: La lista è ordinata. Se il nome corrente è "maggiore" di quello cercato,
 * possiamo fermarci subito (il prodotto non c'è sicuramente dopo).
 * ======================================================================================
 */
void cercaProdottoRicorsiva(struct Prodotto *head, char *nomeProdotto) {
    /* --- 1. CASI BASE --- */
    
    /* Caso A: Lista finita senza trovare il prodotto */
    if (head == NULL) {
        printf("Errore: Prodotto '%s' non trovato.\n", nomeProdotto);
        return;
    }

    /* Confrontiamo le stringhe: strcmp restituisce 0 se uguali */
    int cmp = strcmp(head->nome, nomeProdotto);

    /* Caso B: Trovato! */
    if (cmp == 0) {
        printf("Prezzo trovato: %.2f Euro\n", head->prezzo);
        return;
    }

    /* Caso C: Ottimizzazione Ordinamento.
     * Se head->nome viene dopo alfabeticamente rispetto a nomeProdotto (cmp > 0),
     * dato che la lista è ordinata, è inutile cercare oltre.
     */
    if (cmp > 0) {
        printf("Errore: Prodotto '%s' non trovato (interruzione anticipata).\n", nomeProdotto);
        return;
    }

    /* --- 2. PASSO RICORSIVO --- */
    /* Se non siamo nei casi base, cerchiamo nel resto della lista */
    cercaProdottoRicorsiva(head->next, nomeProdotto);
}

/*
 * ======================================================================================
 * NOME FUNZIONE: trovaPrezzo (Funzione Richiesta)
 * ======================================================================================
 * SCOPO:
 * Funzione ricorsiva principale. Scorre la lista dei produttori.
 * Se trova il produttore, delega la ricerca del prodotto alla funzione ausiliaria.
 * Se non lo trova o finisce la lista, segnala errore.
 *
 * PARAMETRI:
 * * headProduttori : Puntatore alla lista principale.
 * * nomeProduttore : Target produttore.
 * * nomeProdotto   : Target prodotto.
 *
 * CONCETTI CHIAVE:
 * 1. Lista di Liste: Navighiamo prima "orizzontalmente" (tra i produttori).
 * Solo quando troviamo il produttore giusto, scendiamo "verticalmente" (nella sua lista prodotti).
 * 2. strcmp: Fondamentale per navigare e sfruttare l'ordine alfabetico.
 * ======================================================================================
 */
void trovaPrezzo(struct Produttore *headProduttori, char *nomeProduttore, char *nomeProdotto) {
    
    /* --- 1. CASI BASE (Produttore) --- */
    
    /* Caso A: Siamo arrivati alla fine della lista produttori */
    if (headProduttori == NULL) {
        printf("Errore: Produttore '%s' inesistente.\n", nomeProduttore);
        return;
    }

    int cmp = strcmp(headProduttori->nome, nomeProduttore);

    /* Caso B: Produttore Trovato */
    if (cmp == 0) {
        /*
         * DETTAGLIO SINTATTICO:
         * Accediamo al campo 'prodotti' del nodo corrente (headProduttori->prodotti).
         * Questo puntatore è la 'head' della lista secondaria che passiamo alla helper.
         */
        cercaProdottoRicorsiva(headProduttori->prodotti, nomeProdotto);
        return; 
    }

    /* Caso C: Ottimizzazione (lista produttori ordinata) */
    if (cmp > 0) {
        printf("Errore: Produttore '%s' inesistente (interruzione anticipata).\n", nomeProduttore);
        return;
    }

    /* --- 2. PASSO RICORSIVO (Produttore) --- */
    /* Passiamo al prossimo produttore nella lista principale */
    trovaPrezzo(headProduttori->next, nomeProduttore, nomeProdotto);
}

/*
 * ======================================================================================
 * FUNZIONI DI UTILITÀ PER IL TEST (COSTRUZIONE E PULIZIA)
 * ======================================================================================
 */

/* Helper per creare un nuovo prodotto */
struct Prodotto* nuovoProdotto(char *nome, float prezzo) {
    /* GESTIONE MEMORIA: Allocazione dinamica struct Prodotto */
    struct Prodotto *p = (struct Prodotto*)malloc(sizeof(struct Prodotto));
    strcpy(p->nome, nome);
    p->prezzo = prezzo;
    p->next = NULL;
    return p;
}

/* Helper per creare un nuovo produttore */
struct Produttore* nuovoProduttore(char *nome) {
    /* GESTIONE MEMORIA: Allocazione dinamica struct Produttore */
    struct Produttore *p = (struct Produttore*)malloc(sizeof(struct Produttore));
    strcpy(p->nome, nome);
    p->prodotti = NULL;
    p->next = NULL;
    return p;
}

/* Helper per liberare TUTTA la memoria (Lista di Liste) */
void liberaTutto(struct Produttore *head) {
    if (head == NULL) return;

    /* Prima libero la lista secondaria di questo produttore */
    struct Prodotto *prod = head->prodotti;
    while (prod != NULL) {
        struct Prodotto *temp = prod;
        prod = prod->next;
        free(temp); /* Libero il singolo prodotto */
    }

    /* Poi libero il resto dei produttori ricorsivamente */
    liberaTutto(head->next);

    /* Infine libero il produttore corrente */
    free(head);
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Scopo: Creare manualmente una base dati e testare i casi di successo/errore.
 * Struttura creata:
 * "Barilla" -> [Fusilli: 1.20] -> [Penne: 1.10]
 * |
 * "DeCecco" -> [Rigatoni: 1.50]
 * ======================================================================================
 */
int main() {
    printf("--- CREAZIONE DATABASE DI TEST ---\n");

    /* 1. Creazione Produttori (ordinati alfabeticamente: Barilla < DeCecco) */
    struct Produttore *prod1 = nuovoProduttore("Barilla");
    struct Produttore *prod2 = nuovoProduttore("DeCecco");
    
    /* Collegamento Produttori */
    prod1->next = prod2; /* Barilla punta a DeCecco */

    /* 2. Creazione Prodotti per Barilla (ordinati: Fusilli < Penne) */
    prod1->prodotti = nuovoProdotto("Fusilli", 1.20f);
    prod1->prodotti->next = nuovoProdotto("Penne", 1.10f);

    /* 3. Creazione Prodotti per DeCecco */
    prod2->prodotti = nuovoProdotto("Rigatoni", 1.50f);

    struct Produttore *database = prod1;

    /* --- TEST CASE --- */
    
    printf("\nTEST 1: Ricerca corretta (Barilla - Penne)\n");
    trovaPrezzo(database, "Barilla", "Penne"); // Atteso: 1.10

    printf("\nTEST 2: Produttore errato (Voiello)\n");
    trovaPrezzo(database, "Voiello", "Spaghetti"); // Atteso: Errore Produttore

    printf("\nTEST 3: Produttore ok, Prodotto errato (Barilla - Spaghetti)\n");
    trovaPrezzo(database, "Barilla", "Spaghetti"); // Atteso: Errore Prodotto (dopo Penne)

    printf("\nTEST 4: Ottimizzazione (Cerca 'AlceNero' che verrebbe prima di Barilla)\n");
    /* Essendo ordinato, dovrebbe fermarsi subito controllando Barilla */
    trovaPrezzo(database, "AlceNero", "Miele"); 

    /* --- PULIZIA MEMORIA --- */
    printf("\n--- PULIZIA MEMORIA ---\n");
    liberaTutto(database);
    printf("Memoria liberata.\n");

    return 0;
}