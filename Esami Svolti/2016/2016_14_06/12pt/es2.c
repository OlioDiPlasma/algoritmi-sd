#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * FUNZIONE: separaParole
 * ============================================================================
 * SCOPO:
 * Divide una stringa di input in singole parole, allocando dinamicamente
 * sia il vettore di puntatori che le singole stringhe per ogni parola.
 *
 * PARAMETRI:
 * - frase:  (char *) La stringa di input da analizzare (es. "ciao a tutti").
 * - parole: (char ***) Indirizzo della variabile puntatore dichiarata nel main.
 * Usiamo un triplo puntatore perché dobbiamo modificare il VALORE
 * del doppio puntatore del main (cioè l'indirizzo dell'array).
 *
 * VALORE DI RITORNO:
 * - int: Il numero di parole trovate (dimensione dell'array allocato).
 *
 * CONCETTI CHIAVE (Triplo Puntatore):
 * Immagina che 'parole' nel main sia una scatola vuota destinata a contenere
 * l'indirizzo del tuo array. Per riempire quella scatola da dentro questa funzione,
 * devi sapere DOVE si trova la scatola. Quindi passi &parole.
 * Dentro la funzione, *parole rappresenta la scatola stessa (il char**).
 * ============================================================================
 */
int separaParole(char *frase, char ***parole) {

    /* --- 1. PULIZIA INPUT --- */
    /* fgets spesso lascia un '\n' (a capo) alla fine della stringa.
       Lo rimuoviamo per evitare che diventi parte dell'ultima parola. */
    int len = strlen(frase);
    if (len > 0 && frase[len - 1] == '\n') {
        frase[len - 1] = '\0';
    }

    /* --- 2. CONTEGGIO PAROLE (Primo Passaggio) --- */
    /* Dobbiamo sapere quante parole ci sono per allocare il vettore principale.
       Assumiamo separatore spazio singolo. */
    int count = 0;
    
    /* Gestione stringa vuota */
    if (strlen(frase) == 0) {
        *parole = NULL;
        return 0;
    }

    /* Logica: contiamo le parole.
       Se la stringa non è vuota, c'è almeno una parola.
       Poi ogni spazio aggiunge una parola (secondo la specifica semplificata). */
    count = 1; 
    for (int i = 0; i < strlen(frase); i++) {
        if (frase[i] == ' ') {
            count++;
        }
    }

    /* --- 3. ALLOCAZIONE VETTORE DI PUNTATORI (La "Spina Dorsale") --- */
    /* SINTASSI: *parole
       Stiamo assegnando l'indirizzo della memoria appena creata alla variabile
       che si trova nel main (dereferenziamo il triplo puntatore).
       
       DIMENSIONE: count * sizeof(char *)
       Stiamo allocando spazio per 'count' indirizzi di memoria. */
    *parole = (char **)malloc(count * sizeof(char *));
    if (*parole == NULL) {
        printf("Errore allocazione vettore puntatori.\n");
        return 0;
    }

    /* --- 4. ESTRAZIONE E ALLOCAZIONE SINGOLE PAROLE (Secondo Passaggio) --- */
    /* Useremo 'strtok' che è standard per dividere stringhe.
       Nota: strtok modifica la stringa originale inserendo dei terminator \0.
       Se non si volesse modificare 'frase', bisognerebbe farne una copia prima. */
    
    int indice = 0;
    /* La prima chiamata a strtok vuole la stringa sorgente. */
    char *token = strtok(frase, " ");

    while (token != NULL) {
        /* token ora punta all'inizio di una parola nella stringa originale.
           Dobbiamo creare una copia di questa parola in una nuova area di memoria. */

        /* A. Calcolo lunghezza parola */
        int lenParola = strlen(token);

        /* B. Allocazione memoria per la singola parola
           IMPORTANTE: +1 per il terminatore di stringa '\0'!
           Senza il +1, le funzioni di stampa andrebbero in buffer overflow. */
        (*parole)[indice] = (char *)malloc((lenParola + 1) * sizeof(char));
        
        /* Controllo sicurezza malloc */
        if ((*parole)[indice] == NULL) {
            /* In un codice reale, qui dovremmo liberare tutto quello allocato finora
               prima di uscire per evitare memory leak parziali. */
            return -1; 
        }

        /* C. Copia effettiva dei caratteri */
        strcpy((*parole)[indice], token);

        /* Avanziamo */
        indice++;
        
        /* Chiamate successive a strtok vogliono NULL come primo argomento */
        token = strtok(NULL, " ");
    }

    return count;
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * SCOPO:
 * Simulare l'ambiente d'esame. Creare una stringa, chiamare la funzione,
 * stampare i risultati e PULIRE LA MEMORIA.
 * ============================================================================
 */
int main() {
    /* Simuliamo l'input. Uso un buffer modificabile (non string literal "...") 
       perché strtok modificherà il contenuto. */
    char buffer[1000];
    strcpy(buffer, "esame di programmazione in C");

    /* Variabile per accogliere il risultato.
       È un doppio puntatore perché punterà a un array di stringhe. */
    char **parole_trovate;
    int n, i;

    printf("--- INIZIO TEST ---\n");
    printf("Frase originale: '%s'\n", buffer);

    /* CHIAMATA ALLA FUNZIONE:
       Passiamo &parole_trovate (che è di tipo char***) affinché la funzione
       possa modificare il valore di parole_trovate. */
    n = separaParole(buffer, &parole_trovate);

    printf("Numero parole trovate: %d\n", n);

    /* Stampa risultati */
    for (i = 0; i < n; i++) {
        /* Accesso: parole_trovate[i] è un (char*), cioè una stringa */
        printf("Parola %d: %s\n", i, parole_trovate[i]);
    }

    /* --- GESTIONE MEMORIA (Deallocazione) --- */
    /* Regola: Libera dal livello più profondo a quello più esterno. */
    
    /* 1. Liberiamo le singole stringhe (il contenuto) */
    for (i = 0; i < n; i++) {
        free(parole_trovate[i]); // Libera la memoria allocata nel ciclo while
    }

    /* 2. Liberiamo il vettore di puntatori (il contenitore) */
    free(parole_trovate); // Libera la memoria allocata al punto 3 della funzione

    printf("\nMemoria liberata correttamente.\n");

    return 0;
}