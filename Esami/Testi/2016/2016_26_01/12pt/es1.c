#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * FUNZIONE: charErase
 * ============================================================================
 * SCOPO:
 * Crea una NUOVA stringa a partire da 'str', eliminando i caratteri che si trovano
 * agli indici specificati nel vettore 'pos'.
 * La stringa originale non viene modificata.
 *
 * PARAMETRI:
 * - str: (char *) Stringa di input (sorgente).
 * - pos: (int *) Vettore di interi contenente gli indici da cancellare.
 * Termina con il valore -1 (sentinella).
 *
 * RITORNA:
 * - char *: Puntatore alla nuova stringa allocata dinamicamente.
 *
 * CONCETTI CHIAVE:
 * 1. Mappatura (Lookup Table): Il vettore 'pos' contiene indici sparsi (es. 7, 4, 2).
 * Per evitare di scorrere tutto 'pos' per ogni carattere di 'str' (lento!),
 * creiamo un array temporaneo di supporto (maschera) grande quanto la stringa.
 * Se maschera[i] == 1, il carattere va cancellato. Se 0, va tenuto.
 * 2. Allocazione Esatta: Prima contiamo quanti caratteri sopravvivono, poi
 * facciamo la malloc della dimensione esatta (+1 per il terminatore).
 * ============================================================================
 */
char *charErase(char *str, int *pos) {

    /* --- 1. ANALISI DIMENSIONI --- */
    /* Calcoliamo la lunghezza della stringa input per dimensionare le nostre strutture. */
    int len = strlen(str);

    /* --- 2. CREAZIONE MASCHERA DI CANCELLAZIONE (Lookup Table) --- */
    /* Vogliamo un array di "booleani" dove:
       - to_remove[i] = 0 -> Tieni il carattere
       - to_remove[i] = 1 -> Cancella il carattere
       
       USIAMO CALLOC:
       Perché? calloc inizializza la memoria a 0. È fondamentale, così assumiamo
       di default che tutti i caratteri siano da tenere, salvo diversa indicazione.
       Sizeof(int) va bene, anche se basterebbe char (risparmio memoria). */
    
    int *to_remove = (int *)calloc(len, sizeof(int));
    if (to_remove == NULL) {
        printf("Errore allocazione memoria temporanea\n");
        return NULL;
    }

    /* Scorriamo il vettore 'pos' fino al terminatore -1 */
    int k = 0;
    while (pos[k] != -1) {
        int indice_da_cancellare = pos[k];

        /* Controllo di sicurezza: l'indice deve essere valido per questa stringa */
        if (indice_da_cancellare >= 0 && indice_da_cancellare < len) {
            /* Accesso diretto alla cella di memoria: tempo O(1) */
            to_remove[indice_da_cancellare] = 1; 
        }
        k++;
    }

    /* --- 3. CONTEGGIO CARATTERI VALIDI --- */
    /* Prima di allocare la stringa risultato, dobbiamo sapere quanto sarà lunga. */
    int new_len = 0;
    for (int i = 0; i < len; i++) {
        /* Se nella maschera c'è 0, il carattere sopravvive */
        if (to_remove[i] == 0) {
            new_len++;
        }
    }

    /* --- 4. ALLOCAZIONE STRINGA RISULTATO --- */
    /* MALLOC: Allochiamo spazio per i caratteri validi PIÙ UNO.
       Il +1 è obbligatorio per il carattere terminatore '\0' che chiude le stringhe in C.
       Senza di esso, le funzioni come printf non sanno dove finisce la stringa (buffer overflow). */
    char *result = (char *)malloc((new_len + 1) * sizeof(char));
    if (result == NULL) {
        printf("Errore allocazione stringa risultato\n");
        free(to_remove); // Ricordiamoci di pulire prima di uscire!
        return NULL;
    }

    /* --- 5. COPIA DEI CARATTERI --- */
    int j = 0; // Indice per scorrere la NUOVA stringa
    for (int i = 0; i < len; i++) {
        /* Se l'indice i non è marcato per la rimozione... */
        if (to_remove[i] == 0) {
            /* Copiamo il carattere dalla sorgente (str) alla destinazione (result).
               Dettaglio Sintattico: result[j++] è post-incremento.
               Usa j corrente per l'assegnazione, poi incrementa j per il prossimo giro. */
            result[j++] = str[i];
        }
    }

    /* FONDAMENTALE: Chiudere la stringa manualmente.
       Malloc contiene "spazzatura", non zeri. Se non mettiamo '\0', è un disastro. */
    result[j] = '\0';

    /* --- 6. PULIZIA MEMORIA TEMPORANEA --- */
    /* L'array 'to_remove' ci serviva solo qui dentro. Non dobbiamo ritornarlo.
       Quindi lo liberiamo per non lasciare memory leaks. */
    free(to_remove);

    return result;
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * SCOPO:
 * Simulare l'esempio della traccia per verificare la correttezza.
 * ============================================================================
 */
int main() {
    /* Dati dell'esempio */
    /* Nota: Usiamo un array modificabile o una string literal. 
       Qui va bene una string literal perché non la modifichiamo, ne creiamo una copia. */
    char *str = "ThisIsAString";
    
    /* Vettore pos terminato da -1. L'ordine non conta. */
    int pos[] = {7, 4, 2, 0, 11, -1};

    printf("--- INIZIO TEST ---\n");
    printf("Stringa originale: \"%s\"\n", str);
    printf("Indici da rimuovere: 7, 4, 2, 0, 11\n");

    /* --- 1. CHIAMATA ALLA FUNZIONE --- */
    /* La funzione ci restituisce un puntatore allo HEAP. */
    char *nuova_stringa = charErase(str, pos);

    /* --- 2. VERIFICA RISULTATO --- */
    if (nuova_stringa != NULL) {
        printf("Stringa ottenuta:  \"%s\"\n", nuova_stringa);
        
        /* Risultato atteso analisi:
           0:T (rimosso)
           1:h
           2:i (rimosso)
           3:s
           4:I (rimosso)
           5:s
           6:A
           7:S (rimosso)
           8:t
           9:r
           10:i
           11:n (rimosso)
           12:g
           Risultato: "hssAtrig"
        */
        printf("Stringa attesa:    \"hssAtrig\"\n");
        
        if (strcmp(nuova_stringa, "hssAtrig") == 0) {
            printf("-> TEST SUPERATO.\n");
        } else {
            printf("-> TEST FALLITO.\n");
        }

        /* --- 3. GESTIONE MEMORIA (FREE) --- */
        /* Poiché charErase ha fatto una malloc, il main (chiamante)
           ha la responsabilità di fare la free. */
        free(nuova_stringa);
        printf("Memoria liberata correttamente.\n");
    } else {
        printf("Errore nella funzione (ritornato NULL).\n");
    }

    return 0;
}