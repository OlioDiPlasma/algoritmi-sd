#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * FUNZIONE: searchStr
 * ============================================================================
 * SCOPO:
 * Analizza una stringa in input per trovare la sottosequenza più lunga composta
 * dallo stesso carattere consecutivo.
 * Restituisce per riferimento (tramite puntatori) l'indice di inizio e la
 * lunghezza di tale sequenza.
 *
 * PARAMETRI:
 * - char *str:    La stringa (array di caratteri terminato da '\0') da analizzare.
 * - int *start:   Puntatore a intero dove scriveremo l'indice di inizio della sequenza trovata.
 * - int *length:  Puntatore a intero dove scriveremo la lunghezza della sequenza trovata.
 *
 * CONCETTI CHIAVE:
 * 1. Passaggio per Riferimento:
 * In C, le funzioni non possono ritornare più di un valore direttamente.
 * Per restituire sia 'start' che 'length', il chiamante (main) ci passa
 * gli INDIRIZZI di memoria delle sue variabili (&s, &l). Noi useremo
 * l'operatore di dereferenziazione (*) per scrivere dentro quelle celle.
 *
 * 2. Scansione Lineare con "Memoria":
 * Dobbiamo ricordare la lunghezza della sequenza "corrente" mentre scorriamo
 * la stringa e confrontarla con la sequenza "massima" trovata finora.
 * ============================================================================
 */
void searchStr(char *str, int *start, int *length) {

    /* --- 1. CONTROLLO VALIDITÀ INPUT --- */
    /*
     * È buona norma difensiva controllare se i puntatori sono validi (non NULL).
     * Se la stringa è NULL o i puntatori di output sono NULL, non facciamo nulla.
     */
    if (str == NULL || start == NULL || length == NULL) {
        return;
    }

    /* --- 2. INIZIALIZZAZIONE VARIABILI --- */
    
    // Variabili per tracciare la sequenza "Massima" trovata finora
    int max_start_index = 0; // Indice in cui inizia la sequenza migliore
    int max_len_found = 0;   // Lunghezza della sequenza migliore

    // Variabili per tracciare la sequenza "Corrente" che stiamo analizzando
    int current_start_index = 0;
    int current_len = 0;

    // Caso base: stringa vuota
    if (str[0] == '\0') {
        // Dereferenzio i puntatori per assegnare 0 ai valori nel main
        *start = 0;
        *length = 0;
        return;
    }

    /* Inizializziamo il contatore con il primo carattere */
    current_len = 1;

    /* --- 3. LOOP DI SCANSIONE (LOGICA PRINCIPALE) --- */
    /* * Iteriamo dal secondo carattere (indice 1) fino alla fine della stringa.
     * Usiamo 'i' come indice corrente.
     */
    int i;
    for (i = 1; str[i] != '\0'; i++) {

        /* * DETTAGLIO SINTATTICO: Confronto caratteri
         * str[i]     -> carattere attuale
         * str[i-1]   -> carattere precedente
         */
        if (str[i] == str[i-1]) {
            // Se il carattere è uguale al precedente, la sequenza continua.
            current_len++; 
        } else {
            // Se il carattere è diverso, la sequenza corrente è finita.
            
            /* Controlliamo se la sequenza appena finita è la nuova "vincitrice" */
            if (current_len > max_len_found) {
                max_len_found = current_len;     // Aggiorno la lunghezza record
                max_start_index = current_start_index; // Aggiorno l'indice record
            }

            // Resettiamo i contatori per la nuova sequenza che inizia all'indice 'i'
            current_len = 1;
            current_start_index = i;
        }
    }

    /* --- 4. CONTROLLO FINALE (POST-LOOP) --- */
    /*
     * Concetto Critico:
     * Il loop finisce quando trova '\0'. Se la sequenza più lunga si trova
     * PROPRIO ALLA FINE della stringa (es: "aaabbb"), l'else del loop non viene
     * mai eseguito per l'ultimo gruppo. Dobbiamo fare un controllo extra qui.
     */
    if (current_len > max_len_found) {
        max_len_found = current_len;
        max_start_index = current_start_index;
    }

    /* --- 5. ESPORTAZIONE RISULTATI --- */
    /*
     * Scriviamo i risultati nelle variabili del chiamante.
     * Sintassi: *ptr = valore; (Scrivi 'valore' all'indirizzo puntato da ptr)
     */
    *start = max_start_index;
    *length = max_len_found;
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: Verificare il funzionamento con casi di prova.
 * Gestione Memoria: Alloca dinamicamente la stringa per dimostrare malloc/free.
 * ============================================================================
 */
int main() {
    
    /* --- 1. ALLOCAZIONE DATI DI PROVA --- */
    /*
     * Usiamo malloc per allocare memoria nello HEAP.
     * La stringa dell'esercizio è "abbccccddddeeeee" (16 caratteri + 1 terminatore).
     * sizeof(char) è sempre 1, ma lo scriviamo per chiarezza didattica.
     */
    char *test_str = (char *)malloc(30 * sizeof(char)); 
    
    // Verifica allocazione (sempre necessaria!)
    if (test_str == NULL) {
        fprintf(stderr, "Errore di allocazione memoria.\n");
        return 1;
    }

    // Copiamo la stringa dell'esempio nell'area allocata
    // strcpy gestisce il terminatore '\0' automaticamente.
    strcpy(test_str, "abbcccddddeeeee");

    /* Variabili dove la funzione scriverà i risultati */
    int risultato_start;
    int risultato_length;

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    printf("Analisi della stringa: %s\n", test_str);
    
    /* * Passiamo &risultato_start e &risultato_length.
     * L'operatore & (address-of) ottiene l'indirizzo di memoria delle variabili.
     */
    searchStr(test_str, &risultato_start, &risultato_length);

    /* --- 3. STAMPA RISULTATI --- */
    printf("Risultati attesi: start = 10, length = 5\n");
    printf("Risultati ottenuti: start = %d, length = %d\n", risultato_start, risultato_length);

    /* --- 4. VERIFICA --- */
    if (risultato_start == 10 && risultato_length == 5) {
        printf("--> TEST SUPERATO!\n");
    } else {
        printf("--> TEST FALLITO.\n");
    }

    /* --- 5. PULIZIA DELLA MEMORIA (FREE) --- */
    /*
     * Regola d'oro: Per ogni malloc deve esserci una free.
     * Se non lo facciamo, creiamo un "memory leak" (perdita di memoria).
     */
    free(test_str);
    test_str = NULL; // Buona norma: evitare puntatori "dangling" (che puntano a memoria liberata)

    return 0;
}