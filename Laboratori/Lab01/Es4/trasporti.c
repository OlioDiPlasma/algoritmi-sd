#include "trasporti.h"

/* ... (leggiComando e leggiFile rimangono identici all'esercizio precedente, copiali pure) ... */

/* REINSERISCO SOLO LE PARTI MODIFICATE/NUOVE PER L'ESERCIZIO 4 */

comando_e leggiComando(void) {
    const char *tabella[] = {
        "stampa", "stampa_file", /* stampa usa l'ordinamento corrente, stampa_file l'ordine originale */
        "date", "partenza", "capolinea", "ritardo", "ritardo_tot",
        "ricerca_codice", "ricerca_partenza",
        "fine"
    };
    char cmd[MAX_STR];
    printf("\nComandi: stampa (vista corrente), stampa_file (ordine orig.), ricerca_codice, fine...\nInserisci: ");
    if (scanf("%s", cmd) != 1) return r_err;
    for (int i = 0; i < r_err; i++) {
        if (strcmp(cmd, tabella[i]) == 0) return (comando_e)i;
    }
    return r_err;
}

Corsa* leggiFile(const char* nomeFile, int* n_corse) {
    FILE *fp = fopen(nomeFile, "r");
    if (fp == NULL) return NULL;
    fscanf(fp, "%d", n_corse);
    Corsa *vet = (Corsa*) malloc(*n_corse * sizeof(Corsa));
    if (vet == NULL) return NULL;
    for (int i = 0; i < *n_corse; i++) {
        fscanf(fp, "%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d",
               vet[i].codice, vet[i].partenza, vet[i].destinazione,
               &vet[i].data.a, &vet[i].data.m, &vet[i].data.g,
               &vet[i].ora_part.h, &vet[i].ora_part.m, &vet[i].ora_part.s,
               &vet[i].ora_arr.h, &vet[i].ora_arr.m, &vet[i].ora_arr.s,
               &vet[i].ritardo);
    }
    fclose(fp);
    return vet;
}

/* =========================================================================
   SEZIONE: GESTIONE PUNTATORI (MULTI-ORDINAMENTO)
   ========================================================================= */

/* Scopo: Riempire un vettore di puntatori.
   vett_rif[0] punterà a vett_dati[0]
   vett_rif[1] punterà a vett_dati[1]
   ...
*/
void inizializzaPuntatori(Corsa **vett_rif, Corsa *vett_dati, int n) {
    for (int i=0; i<n; i++) {
        vett_rif[i] = &vett_dati[i]; // Assegno l'indirizzo della struct, non copio i dati!
    }
}

/* Helper Date e Ora (Identico a prima) */
int confrontaDate(Data d1, Data d2) {
    if (d1.a != d2.a) return d1.a - d2.a;
    if (d1.m != d2.m) return d1.m - d2.m;
    return d1.g - d2.g;
}

/* Helper Stampa singola (riceve puntatore) */
void stampaCorsa(Corsa *c) {
    printf("%-10s %-15s %04d/%02d/%02d %02d:%02d\n", c->codice, c->partenza, c->data.a, c->data.m, c->data.g, c->ora_part.h, c->ora_part.m);
}

/* Scopo: Stampare iterando sul vettore di PUNTATORI.
   Concetto Chiave: Dereferenziazione indiretta.
   Accesso: vett_rif[i] (è un Corsa*) -> codice
*/
void stampaDaPuntatori(Corsa **vett_rif, int n) {
    for (int i=0; i<n; i++) {
        stampaCorsa(vett_rif[i]);
    }
}

/* =========================================================================
   SEZIONE: FUNZIONI DI CONFRONTO (WRAPPER)
   ========================================================================= */

/* ATTENZIONE: Qui la logica cambia rispetto all'esercizio 3.
   qsort passa alla funzione di confronto un puntatore all'elemento dell'array che sta ordinando.
   L'array è di tipo Corsa* (puntatori).
   Quindi qsort ci passa un puntatore a un puntatore: Corsa**
*/

int cmpDataOraPtr(const void *a, const void *b) {
    /* Dereferenzio due volte per arrivare alla struct, o casto a Corsa** e dereferenzio una volta per avere Corsa* */
    Corsa *c1 = *(Corsa**)a; 
    Corsa *c2 = *(Corsa**)b;

    int res = confrontaDate(c1->data, c2->data);
    if (res != 0) return res;
    return (c1->ora_part.h * 60 + c1->ora_part.m) - (c2->ora_part.h * 60 + c2->ora_part.m);
}

int cmpCodicePtr(const void *a, const void *b) {
    Corsa *c1 = *(Corsa**)a;
    Corsa *c2 = *(Corsa**)b;
    return strcmp(c1->codice, c2->codice);
}

int cmpPartenzaPtr(const void *a, const void *b) {
    Corsa *c1 = *(Corsa**)a;
    Corsa *c2 = *(Corsa**)b;
    return strcmp(c1->partenza, c2->partenza);
}

int cmpArrivoPtr(const void *a, const void *b) {
    Corsa *c1 = *(Corsa**)a;
    Corsa *c2 = *(Corsa**)b;
    return strcmp(c1->destinazione, c2->destinazione);
}

/* Wrapper per qsort */
void ordinaVettorePuntatori(Corsa **vett_rif, int n, int tipo_ord) {
    switch(tipo_ord) {
        case ORD_DATA:
            qsort(vett_rif, n, sizeof(Corsa*), cmpDataOraPtr);
            break;
        case ORD_CODICE:
            qsort(vett_rif, n, sizeof(Corsa*), cmpCodicePtr);
            break;
        case ORD_PARTENZA:
            qsort(vett_rif, n, sizeof(Corsa*), cmpPartenzaPtr);
            break;
        case ORD_ARRIVO:
            qsort(vett_rif, n, sizeof(Corsa*), cmpArrivoPtr);
            break;
    }
}

/* =========================================================================
   SEZIONE: RICERCA DICOTOMICA SU PUNTATORI
   ========================================================================= */
void ricercaDicotomicaSuPuntatori(Corsa **vett_ordinato, int n) {
    /* Funziona SOLO sul vettore ordinato per codice! */
    char target[MAX_STR];
    printf("Codice da cercare: ");
    scanf("%s", target);

    int L = 0, R = n - 1;
    int trovato = 0;
    while (L <= R && !trovato) {
        int M = (L + R) / 2;
        /* Accesso indiretto: vett_ordinato[M]->codice */
        int res = strcmp(vett_ordinato[M]->codice, target);
        if (res == 0) {
            printf("Trovato:\n");
            stampaCorsa(vett_ordinato[M]);
            trovato = 1;
        } else if (res < 0) L = M + 1;
        else R = M - 1;
    }
    if (!trovato) printf("Non trovato.\n");
}