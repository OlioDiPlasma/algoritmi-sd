/*
 * File: main.c
 * Scopo: Test bench per la prova del 21/2/2025.
 * Dati: Hardcoded dall'esempio del testo d'esame.
 */

#include "act.h"

int main() {
    printf("--- INIZIO PROGRAMMA ESAME ---\n");

    /* --- 1. CREAZIONE DATI (Hardcoded come da esempio) --- */
    // Attività: 8, Dipendenze: 4
    int NA = 8;
    int ND = 4;
    ACT a = ACTinit(NA, ND);

    // Mappatura nomi -> indici per facilità di inserimento
    // 0: A1A, 1: BB75, 2: AB12B, 3: ADD, 4: A4, 5: CNT3, 6: T3B, 7: YA
    
    /* Inserimento Attività [Nome, Inizio, Fine, Costo] [cite: 125] */
    ACTaddAttivita(a, 0, "A1A",   1, 4,  12);
    ACTaddAttivita(a, 1, "BB75",  12, 14, 270);
    ACTaddAttivita(a, 2, "AB12B", 0, 6,  201);
    ACTaddAttivita(a, 3, "ADD",   5, 7,  45);
    ACTaddAttivita(a, 4, "A4",    3, 5,  36);
    ACTaddAttivita(a, 5, "CNT3",  5, 9,  74);
    ACTaddAttivita(a, 6, "T3B",   8, 11, 130);
    ACTaddAttivita(a, 7, "YA",    6, 10, 103);

    /* Inserimento Dipendenze [act, dep] (act dipende da dep) [cite: 126] */
    // (ADD, A1A) -> ADD (3) dipende da A1A (0)
    ACTaddDipendenza(a, 0, 3, 0); 
    // (T3B, ADD) -> T3B (6) dipende da ADD (3)
    ACTaddDipendenza(a, 1, 6, 3);
    // (T3B, AB12B) -> T3B (6) dipende da AB12B (2)
    ACTaddDipendenza(a, 2, 6, 2);
    // (CNT3, A4) -> CNT3 (5) dipende da A4 (4)
    ACTaddDipendenza(a, 3, 5, 4);

    /* --- 2. ESECUZIONE VERIFICHE (Problema 1) --- */
    printf("\n[STEP 1] Verifica Fattibilita' Temporale:\n");
    if (ACTcheckDep(a)) {
        printf(" -> OK: I tempi rispettano le dipendenze.\n");
    } else {
        printf(" -> ERRORE: Violazione temporale rilevata!\n");
    }

    printf("\n[STEP 2] Analisi Grafo Dipendenze:\n");
    ACTprintSrcSnk(a);

    /* --- 3. ESECUZIONE OTTIMIZZAZIONE (Problema 2) --- */
    printf("\n[STEP 3] Calcolo Minimo Persone:\n");
    int minP = ACTminPers(a);
    printf(" -> Minimo persone necessarie (N_min): %d\n", minP);

    printf("\n[STEP 4] Calcolo Best Plan (Bilanciamento Carico):\n");
    ACTbestPlan(a);

    /* --- 4. PULIZIA MEMORIA --- */
    ACTfree(a);
    printf("\n--- FINE PROGRAMMA ---\n");

    return 0;
}