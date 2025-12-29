#include "articolo.h"

/* --- LETTURA ARTICOLO ---
 * Scopo: Legge una riga dal file e popola la struct.
 * Parametri: File pointer aperto, id numerico progressivo.
 * Gestione Memoria: Le stringhe sono lette in buffer temporanei
 * e poi duplicate con strdup (che fa malloc interna) per risparmiare spazio.
 */
Articolo leggi_articolo(FILE *fp, int id) {
    Articolo a;
    char buffT[256], buffR[256], buffA[256];

    // Leggiamo: Titolo Relatore Slot Argomento
    if (fscanf(fp, "%s %s %d %s", buffT, buffR, &a.slot, buffA) == 4) {
        a.id = id;
        // strdup alloca la memoria esatta necessaria per la stringa
        a.titolo = strdup(buffT);
        a.relatore = strdup(buffR);
        a.argomento = strdup(buffA);
    } else {
        // Segnale di errore/fine
        a.slot = -1; 
    }
    return a;
}

void free_articolo(Articolo a) {
    if (a.titolo) free(a.titolo);
    if (a.relatore) free(a.relatore);
    if (a.argomento) free(a.argomento);
}