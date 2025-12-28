#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Cardinalità massima dei set (10 caratteri + 1 terminatore '\0') */
#define MAX_SET_SIZE 11

/*
 * ======================================================================================
 * NOME FUNZIONE: generaSigleRicorsiva
 * ======================================================================================
 * SCOPO:
 * Funzione "worker" che implementa l'algoritmo di Backtracking.
 * Esplora tutte le combinazioni possibili riempiendo una posizione alla volta.
 *
 * PARAMETRI:
 * * sets    : Matrice di caratteri (ogni riga è un set di opzioni per quella posizione).
 * N         : Lunghezza totale della sigla.
 * depth     : Profondità attuale (indice della posizione che stiamo riempiendo: 0, 1... N-1).
 * *buffer   : Stringa temporanea dove costruiamo la sigla.
 * *fpOut    : Puntatore al file già aperto dove scrivere i risultati.
 * ======================================================================================
 */
void generaSigleRicorsiva(char **sets, int N, int depth, char *buffer, FILE *fpOut) {
    
    /* --- 1. CASO BASE (Terminazione) --- */
    /* Se la profondità eguaglia N, abbiamo riempito tutte le posizioni (da 0 a N-1). */
    if (depth == N) {
        /* Scriviamo la sigla completa nel file */
        fprintf(fpOut, "%s\n", buffer);
        return; /* Torniamo indietro (backtrack) */
    }

    /* --- 2. LOGICA RICORSIVA (Iterazione sul livello corrente) --- */
    /* Recuperiamo la stringa che contiene i caratteri validi per questa posizione 'depth' */
    char *insiemeCorrente = sets[depth];
    
    /* Iteriamo su ogni carattere possibile di questo set */
    for (int i = 0; insiemeCorrente[i] != '\0'; i++) {
        
        /* * Assegniamo il carattere corrente alla posizione 'depth' del buffer.
         * Stiamo "tentando" questa strada.
         */
        buffer[depth] = insiemeCorrente[i];

        /* * Scendiamo in profondità (depth + 1).
         * La funzione chiamata completerà il resto della sigla.
         */
        generaSigleRicorsiva(sets, N, depth + 1, buffer, fpOut);
        
        /* * Al ritorno dalla chiamata ricorsiva, il ciclo for continua.
         * Sovrascriveremo buffer[depth] con il prossimo carattere dell'insieme.
         */
    }
}

/*
 * ======================================================================================
 * NOME FUNZIONE: wrapperGeneraSigle
 * ======================================================================================
 * SCOPO:
 * Gestisce l'I/O (Input/Output), l'allocazione dinamica e lancia la ricorsione.
 * È l'interfaccia tra il main e la logica complessa.
 *
 * CONCETTI CHIAVE:
 * - char **sets: Usiamo un puntatore doppio per creare un array di stringhe dinamico.
 * - Gestione Errori: Se un file non esiste, il programma deve avvisare e chiudersi pulitamente.
 * ======================================================================================
 */
void wrapperGeneraSigle(char *nomeFileIn, char *nomeFileOut) {
    
    /* --- 1. LETTURA FILE INPUT --- */
    FILE *fin = fopen(nomeFileIn, "r");
    if (fin == NULL) {
        printf("ERRORE CRITICO: Non trovo il file '%s'. Assicurati di averlo creato!\n", nomeFileIn);
        return;
    }

    int N;
    /* Leggiamo N (lunghezza sigla) dalla prima riga */
    if (fscanf(fin, "%d", &N) != 1) {
        printf("ERRORE: Formato file non valido (manca N).\n");
        fclose(fin);
        return;
    }

    /* --- 2. ALLOCAZIONE MATRICE DEI SET (char **) --- */
    /*
     * Allocazione dell'array "verticale" di puntatori.
     * Ogni elemento di questo array punterà a una stringa.
     */
    char **sets = (char **)malloc(N * sizeof(char *));
    if (sets == NULL) {
        printf("ERRORE: Memoria insufficiente per i puntatori.\n");
        fclose(fin);
        return;
    }

    /* Allocazione delle singole righe (le stringhe vere e proprie) */
    for (int i = 0; i < N; i++) {
        sets[i] = (char *)malloc(MAX_SET_SIZE * sizeof(char));
        /* Leggiamo la stringa dal file direttamente nella memoria allocata */
        fscanf(fin, "%s", sets[i]);
    }
    
    /* Chiudiamo il file di input, non serve più */
    fclose(fin);

    /* --- 3. ALLOCAZIONE BUFFER DI LAVORO --- */
    /* N + 1 serve per ospitare il carattere terminatore '\0' */
    char *buffer = (char *)malloc((N + 1) * sizeof(char));
    buffer[N] = '\0'; // Importante: chiudiamo subito la stringa in fondo

    /* --- 4. PREPARAZIONE FILE OUTPUT --- */
    FILE *fout = fopen(nomeFileOut, "w");
    if (fout == NULL) {
        printf("ERRORE: Impossibile creare il file di output.\n");
        /* Nota: qui dovremmo liberare la memoria allocata sopra prima di uscire */
        return;
    }

    /* --- 5. ESECUZIONE --- */
    printf("Inizio generazione sigle...\n");
    generaSigleRicorsiva(sets, N, 0, buffer, fout);
    printf("Generazione completata. Risultati salvati su '%s'.\n", nomeFileOut);

    /* Chiudiamo il file di output per salvare i dati su disco */
    fclose(fout);

    /* --- 6. PULIZIA MEMORIA (FREE) --- */
    /* * Rilasciamo la memoria in ordine inverso all'allocazione.
     * 1. Liberiamo le singole stringhe.
     */
    for (int i = 0; i < N; i++) {
        free(sets[i]);
    }
    /* 2. Liberiamo l'array di puntatori */
    free(sets);
    /* 3. Liberiamo il buffer */
    free(buffer);
}

/*
 * ======================================================================================
 * MAIN
 * ======================================================================================
 * Scopo: Richiamare il wrapper con nomi di file "reali".
 * Non crea file temporanei, si aspetta che input.txt esista già.
 * ======================================================================================
 */
int main() {
    /* * Definiamo i nomi dei file.
     * Assicurati che "input.txt" esista nella cartella del progetto.
     */
    char *fileInput = "input.txt";
    char *fileOutput = "output.txt";

    printf("--- PROGRAMMA GENERATORE SIGLE ---\n");
    printf("Leggo da: %s\n", fileInput);
    printf("Scrivo su: %s\n", fileOutput);
    printf("----------------------------------\n");

    /* Chiamata alla funzione principale */
    wrapperGeneraSigle(fileInput, fileOutput);

    printf("----------------------------------\n");
    printf("Fine del programma.\n");

    return 0;
}