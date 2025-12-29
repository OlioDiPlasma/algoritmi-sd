/* main.c */
#include "magazzino.h"

int main() {
    printf("=== GESTIONE MAGAZZINO ===\n");

    /* --- 1. INIZIALIZZAZIONE --- */
    // Creiamo il file config.txt al volo se non esiste (per comodità tua)
    // Nota: Normalmente questo file è creato manualmente, lo faccio qui per rendere il codice self-contained
    FILE *f = fopen("config.txt", "w");
    fprintf(f, "2 4\n0 2\n1 3\n"); // 2 Corridoi, 4 Slot. C0 ha 2 scaffali, C1 ha 3.
    fclose(f);

    Magazzino *wh = initMagazzino("config.txt");
    if (!wh) return -1;
    printf("Magazzino inizializzato.\n");

    /* --- 2. INSERIMENTO PACCHI --- */
    printf("\n[TEST] Inserimento Pacchi...\n");
    Posizione p1 = {0, 0, 0}; // Corridoio 0, Scaffale 0, Slot 0
    inserimentoPacco(wh, "PCK001", p1);

    Posizione p2 = {0, 0, 2}; // Lasciamo uno spazio vuoto (slot 1)
    inserimentoPacco(wh, "PCK002", p2);

    Posizione p3 = {0, 1, 0}; // Altro scaffale
    inserimentoPacco(wh, "PCK003", p3);

    stampaStatoMagazzino(wh);

    /* --- 3. RICERCA (Verifica O(log N)) --- */
    printf("\n[TEST] Ricerca 'PCK002'...\n");
    Posizione res = ricercaPacco(wh, "PCK002");
    if (res.id_corridoio != -1)
        printf("Trovato in: C:%d S:%d Slot:%d\n", res.id_corridoio, res.id_scaffale, res.id_slot);
    else
        printf("Non trovato.\n");

    /* --- 4. SPOSTAMENTO --- */
    printf("\n[TEST] Spostamento PCK001 da C0-S0-Slot0 a C0-S0-Slot1...\n");
    Posizione dest = {0, 0, 1}; // Slot vuoto
    spostamentoPacco(wh, p1, dest);
    stampaStatoMagazzino(wh);
    
    // Verifica aggiornamento BST dopo spostamento
    res = ricercaPacco(wh, "PCK001");
    printf("Verifica Indice BST post-spostamento PCK001: Slot atteso 1, ottenuto %d\n", res.id_slot);

    /* --- 5. COMPATTAZIONE --- */
    printf("\n[TEST] Compattazione: Spostare tutto da Scaffale 0 a Scaffale 1 (Corridoio 0)...\n");
    // Al momento in S0 ci sono 2 pacchi (slot 1 e 2). In S1 c'è 1 pacco (slot 0).
    // Capacità K=4. Totale 3 pacchi. Dovrebbe starci.
    compattazioneScaffali(wh, 0, 1, 0); // Destinazione: S1, Sorgente: S0
    stampaStatoMagazzino(wh);

    /* --- 6. PULIZIA --- */
    liberaMagazzino(wh);
    printf("\nMemoria liberata. Fine programma.\n");

    return 0;
}