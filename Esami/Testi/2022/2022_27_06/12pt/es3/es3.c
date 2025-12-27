#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* ============================================================================
 * SPIEGAZIONE DEL MODELLO COMBINATORIO
 * ============================================================================
 * Il problema richiede di generare sequenze di lunghezza k. L'ordine conta
 * (es. "AB" è diverso da "BA") e gli elementi possono ripetersi.
 *
 * Modello: DISPOSIZIONI CON RIPETIZIONE.
 * Dato un alfabeto di 52 simboli (26 minuscole + 26 maiuscole), cerchiamo
 * le disposizioni di lunghezza k che soddisfano i vincoli.
 *
 * Strategia: BACKTRACKING (Ricorsione).
 * Costruiamo la soluzione passo dopo passo. A ogni passo (livello della ricorsione)
 * decidiamo quale carattere inserire nella posizione corrente.
 *
 * CRITERI DI PRUNING (POTATURA):
 * Per rendere l'algoritmo efficiente, non generiamo tutto per poi filtrare alla fine.
 * Blocchiamo la ricorsione ("tagliamo il ramo") non appena un vincolo viene violato:
 *
 * 1. Vincolo Minuscole: Se abbiamo già raggiunto il numero massimo di minuscole
 * consentito (k/2), non proviamo nemmeno ad aggiungere un'altra lettera minuscola.
 * 2. Vincolo Ripetizione: Se il carattere che stiamo per inserire è uguale all'ultimo
 * inserito (case-insensitive) e abbiamo già raggiunto 'p' ripetizioni consecutive,
 * saltiamo quel carattere.
 */

/* ============================================================================
 * FUNZIONE: generate_recursive
 * ============================================================================
 * SCOPO:
 * Funzione "core" ricorsiva che riempie il vettore soluzione posizione per posizione.
 *
 * PARAMETRI:
 * - int pos: L'indice della casella che stiamo riempiendo in questa chiamata (da 0 a k-1).
 * - char *sol: Il vettore (buffer) che contiene la sequenza in costruzione.
 * - int k: La lunghezza totale della sequenza desiderata.
 * - int p: Il numero massimo di ripetizioni consecutive ammesse.
 * - int cnt_lower: Contatore delle minuscole inserite finora (dall'indice 0 a pos-1).
 * - int last_char_idx: L'indice numerico (0-25) dell'ultimo carattere inserito.
 * Serve per controllare il vincolo delle ripetizioni ignorando il case.
 * Es: 'a' e 'A' hanno entrambi indice 0.
 * - int consec_count: Quante volte consecutive è apparso 'last_char_idx' fino ad ora.
 * - int *count_sol: Un puntatore a un intero per contare quante soluzioni troviamo.
 *
 * CONCETTI CHIAVE:
 * - Ricorsione: Ogni chiamata si occupa di UNA sola posizione e delega il resto alla chiamata successiva.
 * - Pruning: I controlli 'if' all'interno del ciclo impediscono chiamate ricorsive inutili.
 * ============================================================================
 */
void generate_recursive(int pos, char *sol, int k, int p, 
                        int cnt_lower, int last_char_idx, int consec_count, 
                        int *count_sol) {

    /* --- 1. CASO BASE (Terminazione) --- */
    /* Se pos == k, significa che abbiamo riempito validamente le posizioni da 0 a k-1.
     * La stringa è completa e rispetta i vincoli (altrimenti saremmo stati "potati" prima).
     */
    if (pos == k) {
        /* Aggiungiamo il terminatore di stringa per poterla stampare con printf */
        sol[k] = '\0'; 
        printf("Soluzione %d: %s\n", ++(*count_sol), sol);
        return; /* Torniamo indietro al livello precedente (Backtracking) */
    }

    /* --- 2. ITERAZIONE SULLE SCELTE --- */
    /* Invece di iterare su 52 caratteri misti, itero sugli indici astratti 0-25 (A-Z).
     * Per ogni indice 'i', proverò a inserire sia la versione MAIUSCOLA che quella MINUSCOLA.
     * Questo semplifica enormemente il controllo del vincolo "consecutivi case-insensitive".
     */
    for (int i = 0; i < 3; i++) { //cambia i<tot (originalmente 26) se provo con 3 dovrebbe uscire 48 meglio per debug 
        
        /* --- 3. GESTIONE VINCOLO RIPETIZIONE (PRUNING 1) --- */
        /* Calcoliamo quante volte consecutive apparirebbe il carattere 'i' se lo scegliessimo ora. */
        int current_consec = 1;

        if (pos > 0) { // Se non siamo alla prima lettera
            if (i == last_char_idx) {
                /* Se stiamo provando lo stesso carattere del livello precedente... */
                if (consec_count >= p) {
                    /* PRUNING: Abbiamo già raggiunto il limite p.
                     * Non possiamo aggiungere ancora questo carattere.
                     * 'continue' salta alla prossima iterazione del ciclo for (prossima lettera).
                     */
                    continue; 
                }
                /* Se siamo sotto il limite, incrementiamo il contatore */
                current_consec = consec_count + 1;
            } else {
                /* Se il carattere è diverso dal precedente, il contatore riparte da 1 */
                current_consec = 1;
            }
        }

        /* Ora proviamo le due varianti: Maiuscola e Minuscola */

        /* --- TENTATIVO A: LETTERA MAIUSCOLA ('A' + i) --- */
        /* Le maiuscole non hanno limiti di quantità totale, quindi le proviamo sempre
         * (purché il vincolo di ripetizione sopra sia soddisfatto).
         */
        sol[pos] = 'A' + i; // Assegno il carattere, es: 'A' + 0 = 'A', 'A' + 1 = 'B'
        
        /* CHIAMATA RICORSIVA */
        generate_recursive(pos + 1, sol, k, p, 
                           cnt_lower,     // Il numero di minuscole non cambia
                           i,             // Questo è il nuovo 'last_char_idx'
                           current_consec,// Questo è il nuovo contatore ripetizioni
                           count_sol);


        /* --- TENTATIVO B: LETTERA MINUSCOLA ('a' + i) --- */
        /* PRUNING 2: Posso aggiungere una minuscola solo se non supero la metà di k.
         * La condizione è: (cnt_lower + 1) <= (k / 2)
         */
        if (cnt_lower + 1 <= k / 2) {
            
            sol[pos] = 'a' + i; // Sovrascrivo sol[pos] con la versione minuscola
            
            /* CHIAMATA RICORSIVA */
            generate_recursive(pos + 1, sol, k, p, 
                               cnt_lower + 1, // Ho aggiunto una minuscola!
                               i,             // L'indice astratto è lo stesso ('a' è sempre indice 0)
                               current_consec,// Il contatore ripetizioni è lo stesso (case-insensitive)
                               count_sol);
        }
    }
}

/* ============================================================================
 * FUNZIONE: wrapper_generate
 * ============================================================================
 * SCOPO:
 * Prepara l'ambiente per la ricorsione (alloca memoria) e avvia il processo.
 *
 * PARAMETRI:
 * - int k: Lunghezza sequenza.
 * - int p: Max ripetizioni consecutive.
 *
 * GESTIONE MEMORIA:
 * Utilizza malloc per creare il buffer della soluzione. È fondamentale perché
 * k non è noto a tempo di compilazione (C99 supporta VLA, ma malloc è più sicuro/portabile).
 * ============================================================================
 */
void wrapper_generate(int k, int p) {
    /* Controllo di sicurezza sui parametri */
    if (k <= 0) return;

    /* --- 1. ALLOCAZIONE --- */
    /* Alloco k + 1 char. Perché +1? Per il terminatore di stringa '\0' necessario per printf.
     * sizeof(char) è sempre 1, ma lo scrivo per chiarezza didattica.
     */
    char *sol = (char *)malloc((k + 1) * sizeof(char));
    
    if (sol == NULL) {
        printf("Errore: allocazione memoria fallita.\n");
        return;
    }

    int count_sol = 0;

    printf("--- INIZIO GENERAZIONE (k=%d, p=%d) ---\n", k, p);

    /* --- 2. LANCIO RICORSIONE --- */
    /* Parametri iniziali:
     * pos = 0 (partiamo dalla prima cella)
     * cnt_lower = 0 (nessuna minuscola usata)
     * last_char_idx = -1 (nessun carattere precedente)
     * consec_count = 0 (nessuna ripetizione attiva)
     */
    generate_recursive(0, sol, k, p, 0, -1, 0, &count_sol);

    printf("--- FINE: Trovate %d soluzioni ---\n", count_sol);

    /* --- 3. PULIZIA --- */
    /* Mai dimenticare free()! */
    free(sol);
}


/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 */
int main() {
    /* * CASO DI TEST:
     * k = 3 (sequenze corte per leggibilità)
     * p = 1 (nessun carattere può ripetersi consecutivamente, né 'AA' né 'Aa')
     *
     * Vincolo minuscole: max k/2 = 3/2 = 1 minuscola ammessa.
     */
    
    int k = 3;
    int p = 1;

    wrapper_generate(k, p);

    /* ESEMPIO DI OUTPUT ATTESO (parziale):
     * ABA (Valido)
     * ABa (Valido: 'a' != 'A' come carattere, ma 'a' è ripetizione di 'A'? 
     * No, l'esercizio dice "lo stesso carattere... non può apparire più di p volte".
     * Tuttavia, il mio codice tratta 'A' e 'a' come lo stesso indice 0.
     * Quindi con p=1, "Aa" NON è valido, "AA" NON è valido.
     * "ABa" è valido (A->B->a).
     */

    return 0;
}