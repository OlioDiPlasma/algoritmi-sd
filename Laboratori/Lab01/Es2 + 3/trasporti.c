#include "trasporti.h"

/* =========================================================================
   SEZIONE: GESTIONE MENU E COMANDI
   ========================================================================= */

/*
   Scopo: Legge una stringa da tastiera, la confronta con una tabella di comandi
          e ritorna il valore enum corrispondente.
   Parametri: Nessuno (legge da stdin).
   Concetti Chiave: 
     - Tabella di lookup: array di stringhe `tabella`.
     - `strcasecmp` (o loop con tolower): qui usiamo strcmp standard, 
       quindi l'utente deve scrivere i comandi in minuscolo esatto.
*/
comando_e leggiComando(void) {
    /* Tabella statica delle stringhe di comando. L'ordine deve corrispondere all'enum! */
    const char *tabella[] = {
        "date", "partenza", "capolinea", "ritardo", "ritardo_tot",
        "ord_data", "ord_codice", "ord_partenza", "ord_arrivo",
        "ricerca_codice", "ricerca_partenza",
        "stampa", "fine"
    };
    
    char cmd[MAX_STR];
    printf("\nInserisci comando (date, partenza, capolinea, ritardo, ritardo_tot,\n");
    printf("ord_data, ord_codice, ord_partenza, ord_arrivo,\n");
    printf("ricerca_codice, ricerca_partenza, stampa, fine): ");
    
    if (scanf("%s", cmd) != 1) return r_err;

    /* Scansione lineare della tabella comandi */
    for (int i = 0; i < r_err; i++) { // r_err è l'ultimo valore, funge da dimensione
        if (strcmp(cmd, tabella[i]) == 0)
            return (comando_e)i; /* Cast int -> enum */
    }
    return r_err;
}

/* =========================================================================
   SEZIONE: GESTIONE FILE E MEMORIA
   ========================================================================= */

/*
   Scopo: Apre il file, legge il numero di righe e alloca dinamicamente il vettore.
   File di input: Richiesto formato specifico (N righe, poi dati).
   Parametri: 
     - nomeFile: stringa col nome.
     - n_corse: PUNTATORE a intero. Serve per restituire al main il numero di righe lette.
   Return: Puntatore al vettore allocato (o NULL se errore).
   Concetti Chiave: 
     - `malloc`: Allocazione dinamica nello HEAP.
     - Passaggio per riferimento (`int *n_corse`): Modifichiamo la variabile del main.
*/
Corsa* leggiFile(const char* nomeFile, int* n_corse) {
    FILE *fp = fopen(nomeFile, "r");
    if (fp == NULL) {
        printf("Errore apertura file.\n");
        return NULL;
    }

    /* --- 1. LETTURA DIMENSIONE --- */
    fscanf(fp, "%d", n_corse);
    
    /* --- 2. ALLOCAZIONE --- */
    /* Dettaglio Sintattico:
       malloc ritorna void*, lo castiamo a Corsa*.
       La dimensione è n_corse * sizeof(Corsa).
       Se n_corse è 1000 e Corsa pesa 100 byte, allochiamo 100.000 byte contigui.
    */
    Corsa *vet = (Corsa*) malloc(*n_corse * sizeof(Corsa));
    if (vet == NULL) {
        printf("Errore allocazione memoria.\n");
        fclose(fp);
        return NULL;
    }

    /* --- 3. CARICAMENTO DATI --- */
    for (int i = 0; i < *n_corse; i++) {
        /*
           Dettaglio Sintattico:
           vet[i].codice: Accedo all'i-esima struct, campo codice.
           Notare l'uso di indirizzi &vet[i].data.a per le struct annidate.
        */
        fscanf(fp, "%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d",
               vet[i].codice, 
               vet[i].partenza, 
               vet[i].destinazione,
               &vet[i].data.a, &vet[i].data.m, &vet[i].data.g,
               &vet[i].ora_part.h, &vet[i].ora_part.m, &vet[i].ora_part.s,
               &vet[i].ora_arr.h, &vet[i].ora_arr.m, &vet[i].ora_arr.s,
               &vet[i].ritardo);
    }

    fclose(fp);
    return vet;
}

/* =========================================================================
   SEZIONE: UTILITÀ E STAMPA
   ========================================================================= */

/* Funzione helper per confrontare due date.
   Ritorna <0 se d1 < d2, 0 se uguali, >0 se d1 > d2 */
int confrontaDate(Data d1, Data d2) {
    if (d1.a != d2.a) return d1.a - d2.a;
    if (d1.m != d2.m) return d1.m - d2.m;
    return d1.g - d2.g;
}

void stampaCorsa(Corsa c) {
    /* Formato fisso per allineamento colonne */
    printf("%-10s %-15s %-15s %04d/%02d/%02d %02d:%02d %02d:%02d Rit:%d\n",
           c.codice, c.partenza, c.destinazione,
           c.data.a, c.data.m, c.data.g,
           c.ora_part.h, c.ora_part.m,
           c.ora_arr.h, c.ora_arr.m,
           c.ritardo);
}

void stampaTutto(Corsa* corse, int n) {
    for(int i=0; i<n; i++) stampaCorsa(corse[i]);
}

/* =========================================================================
   SEZIONE: ESERCIZIO 2 (FILTRI E RICERCHE LINEARI)
   ========================================================================= */

void elencaPerDate(Corsa* corse, int n) {
    Data d1, d2;
    printf("Inserisci data inizio (aaaa mm gg): ");
    scanf("%d %d %d", &d1.a, &d1.m, &d1.g);
    printf("Inserisci data fine (aaaa mm gg): ");
    scanf("%d %d %d", &d2.a, &d2.m, &d2.g);

    for (int i=0; i<n; i++) {
        /* Controllo se la data corrente è >= d1 E <= d2 */
        if (confrontaDate(corse[i].data, d1) >= 0 && 
            confrontaDate(corse[i].data, d2) <= 0) {
            stampaCorsa(corse[i]);
        }
    }
}

void elencaPerPartenza(Corsa* corse, int n) {
    char fermata[MAX_STR];
    printf("Inserisci fermata partenza: ");
    scanf("%s", fermata);
    for (int i=0; i<n; i++) {
        if (strcmp(corse[i].partenza, fermata) == 0)
            stampaCorsa(corse[i]);
    }
}

void elencaPerCapolinea(Corsa* corse, int n) {
    char fermata[MAX_STR];
    printf("Inserisci capolinea: ");
    scanf("%s", fermata);
    for (int i=0; i<n; i++) {
        if (strcmp(corse[i].destinazione, fermata) == 0)
            stampaCorsa(corse[i]);
    }
}

void elencaRitardiInDate(Corsa* corse, int n) {
    Data d1, d2;
    printf("Inserisci data inizio e fine (aaaa mm gg) (aaaa mm gg): ");
    scanf("%d %d %d %d %d %d", &d1.a, &d1.m, &d1.g, &d2.a, &d2.m, &d2.g);
    
    for (int i=0; i<n; i++) {
        if (corse[i].ritardo > 0 &&
            confrontaDate(corse[i].data, d1) >= 0 && 
            confrontaDate(corse[i].data, d2) <= 0) {
            stampaCorsa(corse[i]);
        }
    }
}

void ritardoTotaleCodice(Corsa* corse, int n) {
    char codice[MAX_STR];
    int tot = 0;
    printf("Inserisci codice tratta: ");
    scanf("%s", codice);
    
    for (int i=0; i<n; i++) {
        if (strcmp(corse[i].codice, codice) == 0) {
            tot += corse[i].ritardo;
        }
    }
    printf("Ritardo totale per %s: %d minuti\n", codice, tot);
}

/* =========================================================================
   SEZIONE: ESERCIZIO 3 (ORDINAMENTO)
   ========================================================================= */

/* Per l'ordinamento usiamo qsort (QuickSort) della libreria standard.
   qsort richiede una funzione di confronto che accetta due const void*.
   Dobbiamo fare il casting ai nostri tipi per confrontare.
*/

/* --- 1. Confronto Data e Ora --- */
int cmpDataOra(const void *a, const void *b) {
    /* Casting da void* al tipo reale Corsa* */
    Corsa *c1 = (Corsa*)a;
    Corsa *c2 = (Corsa*)b;

    /* Prima chiave: Data */
    int res = confrontaDate(c1->data, c2->data);
    if (res != 0) return res;

    /* Se date uguali, Seconda chiave: Ora partenza */
    if (c1->ora_part.h != c2->ora_part.h) return c1->ora_part.h - c2->ora_part.h;
    if (c1->ora_part.m != c2->ora_part.m) return c1->ora_part.m - c2->ora_part.m;
    return c1->ora_part.s - c2->ora_part.s;
}

void ordinaDataOra(Corsa* corse, int n) {
    /* qsort(array, num_elementi, dimensione_singolo_elemento, funzione_confronto) */
    qsort(corse, n, sizeof(Corsa), cmpDataOra);
    printf("Ordinamento per data e ora completato.\n");
}

/* --- 2. Confronto Codice Tratta --- */
int cmpCodice(const void *a, const void *b) {
    Corsa *c1 = (Corsa*)a;
    Corsa *c2 = (Corsa*)b;
    return strcmp(c1->codice, c2->codice);
}

void ordinaCodice(Corsa* corse, int n) {
    qsort(corse, n, sizeof(Corsa), cmpCodice);
    printf("Ordinamento per codice completato.\n");
}

/* --- 3. Confronto Stazione Partenza --- */
int cmpPartenza(const void *a, const void *b) {
    return strcmp(((Corsa*)a)->partenza, ((Corsa*)b)->partenza);
}

void ordinaPartenza(Corsa* corse, int n) {
    qsort(corse, n, sizeof(Corsa), cmpPartenza);
    printf("Ordinamento per stazione di partenza completato.\n");
}

/* --- 4. Confronto Stazione Arrivo --- */
int cmpArrivo(const void *a, const void *b) {
    return strcmp(((Corsa*)a)->destinazione, ((Corsa*)b)->destinazione);
}

void ordinaArrivo(Corsa* corse, int n) {
    qsort(corse, n, sizeof(Corsa), cmpArrivo);
    printf("Ordinamento per stazione di arrivo completato.\n");
}

/* =========================================================================
   SEZIONE: ESERCIZIO 3 (RICERCA DICOTOMICA E LINEARE PARZIALE)
   ========================================================================= */

/*
   Scopo: Ricerca Binaria (Dicotomica) su un vettore ordinato.
   ATTENZIONE: Funziona solo se il vettore e' ordinato per codice!
   Algoritmo: O(log N). Divide et impera.
*/
void ricercaDicotomicaCodice(Corsa* corse, int n) {
    char target[MAX_STR];
    printf("Inserisci codice tratta da cercare (es. GTT001): ");
    scanf("%s", target);

    /* IMPORTANTE: Per sicurezza in un esame reale, qui si dovrebbe
       controllare se il vettore è ordinato o forzare un riordino.
       Assumiamo che l'utente abbia chiamato "ordinaCodice" prima.
    */
    
    int L = 0, R = n - 1;
    int trovato = 0;

    while (L <= R && !trovato) {
        int M = (L + R) / 2; // Indice medio
        int res = strcmp(corse[M].codice, target);
        
        if (res == 0) {
            printf("Trovato con ricerca dicotomica:\n");
            stampaCorsa(corse[M]);
            trovato = 1;
        } else if (res < 0) {
            L = M + 1; // Cerca nella metà destra
        } else {
            R = M - 1; // Cerca nella metà sinistra
        }
    }
    
    if (!trovato) printf("Codice non trovato.\n");
}

/*
   Scopo: Ricerca Lineare per prefisso (ricerca parziale).
   Se l'utente scrive "To", deve trovare "Torino", "Tortona", ecc.
   Concetto Chiave: `strncmp`
*/
void ricercaLinearePartenza(Corsa* corse, int n) {
    char input[MAX_STR];
    printf("Inserisci inizio nome stazione partenza: ");
    scanf("%s", input);
    
    int len = strlen(input);
    int trovati = 0;

    for (int i=0; i<n; i++) {
        /*
           strncmp confronta solo i primi 'len' caratteri.
           Se input è "Tor" (len 3), confronta i primi 3 char di corse[i].partenza
        */
        if (strncmp(corse[i].partenza, input, len) == 0) {
            stampaCorsa(corse[i]);
            trovati++;
        }
    }
    
    if (trovati == 0) printf("Nessuna stazione inizia con %s\n", input);
}