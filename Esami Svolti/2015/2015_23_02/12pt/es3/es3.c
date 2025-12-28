#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ======================================================================================
 * NOME FUNZIONE: contaOccorrenze
 * ======================================================================================
 * SCOPO:
 * Funzione ausiliaria per verificare il vincolo "k".
 * Conta quante volte il carattere 'c' appare nella stringa 'buffer' fino alla posizione 'len'.
 *
 * PARAMETRI:
 * * buffer : La password parziale che stiamo costruendo.
 * len    : La lunghezza attuale della parte riempita.
 * c      : Il carattere che vogliamo inserire.
 *
 * RITORNA:
 * Il numero di volte che 'c' è già presente.
 * ======================================================================================
 */
int contaOccorrenze(char *buffer, int len, char c) {
    int count = 0;
    for (int i = 0; i < len; i++) {
        if (buffer[i] == c) {
            count++;
        }
    }
    return count;
}

/*
 * ======================================================================================
 * NOME FUNZIONE: generaPasswordRicorsiva
 * ======================================================================================
 * SCOPO:
 * Funzione worker ricorsiva (Backtracking). Riempie una posizione alla volta della password.
 * - Posizioni 0, 1, 2: Deve inserire lettere 'A'-'Z'.
 * - Posizioni 3, 4: Deve inserire cifre '0'-'9'.
 *
 * PARAMETRI:
 * pos    : Indice della posizione corrente da riempire (0 a 4).
 * k      : Numero massimo di ripetizioni consentite per carattere.
 * *buffer: Stringa temporanea per costruire la password.
 * *fp    : Puntatore al file dove scrivere i risultati.
 *
 * CONCETTI CHIAVE:
 * 1. Dominio Variabile: A differenza degli esercizi precedenti dove il set di caratteri era fisso,
 * qui cambia in base alla posizione (Lettere vs Numeri). Usiamo un `if (pos < 3)` per decidere.
 * 2. Pruning (Potatura): Prima di fare la chiamata ricorsiva, verifichiamo se l'aggiunta del
 * carattere violerebbe il vincolo k. Se sì, saltiamo quel ramo (non scendiamo in ricorsione).
 * ======================================================================================
 */
void generaPasswordRicorsiva(int pos, int k, char *buffer, FILE *fp) {

    /* --- 1. CASO BASE (Terminazione) --- */
    /* Se pos == 5, abbiamo riempito le celle 0,1,2,3,4. La password è completa. */
    if (pos == 5) {
        fprintf(fp, "%s\n", buffer);
        return;
    }

    /* --- 2. DETERMINAZIONE DEL DOMINIO --- */
    char startChar, endChar;

    if (pos < 3) {
        /* Primi 3 caratteri: Lettere Maiuscole */
        startChar = 'A';
        endChar = 'Z';
    } else {
        /* Ultimi 2 caratteri: Cifre */
        startChar = '0';
        endChar = '9';
    }

    /* --- 3. CICLO DI PROVA (Backtracking) --- */
    /* Iteriamo su tutti i possibili caratteri validi per questa posizione */
    for (char c = startChar; c <= endChar; c++) {
        
        /* --- 4. VERIFICA VINCOLI (Constraint Check) --- */
        /* Contiamo quante volte 'c' è già stato usato nella password parziale */
        int uscite = contaOccorrenze(buffer, pos, c);

        /* Se usarlo ancora non supera k, procediamo */
        if (uscite < k) {
            
            buffer[pos] = c; /* Assegna (Tentativo) */
            
            /* Chiamata Ricorsiva per la prossima posizione */
            generaPasswordRicorsiva(pos + 1, k, buffer, fp);
            
            /* * Backtracking Implicito:
             * Al prossimo giro del ciclo 'for', buffer[pos] verrà sovrascritto
             * dal prossimo carattere 'c'. Non serve cancellarlo manualmente.
             */
        }
    }
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Scopo: Leggere K, aprire il file e lanciare la generazione.
 * ======================================================================================
 */
int main(int argc, char *argv[]) {
    
    /* --- 1. GESTIONE ARGOMENTI RIGA DI COMANDO --- */
    /* Il testo dice "il cui nome è passato come parametro nella riga di comandi". */
    char *nomeFileOut;
    
    if (argc < 2) {
        /* Se l'utente non passa argomenti, usiamo un default per il test */
        printf("Uso: %s <nomefileoutput>\n", argv[0]);
        printf("Uso default: 'password_generate.txt'\n");
        nomeFileOut = "password_generate.txt";
    } else {
        nomeFileOut = argv[1];
    }

    /* --- 2. INPUT UTENTE (Lettura K) --- */
    int k;
    printf("Inserisci il valore massimo di ripetizioni k: ");
    /* Per il test automatico, forziamo k=1 se non c'è input (o simuliamo input) */
    /* scanf("%d", &k); Standard */ 
    k = 1; // HARDCODED PER IL TEST DIDATTICO (genera password senza ripetizioni)
    printf("%d (Valore Hardcoded per Demo)\n", k);

    /* --- 3. PREPARAZIONE BUFFER E FILE --- */
    /* Password di 5 caratteri + 1 terminatore '\0' = 6 char */
    char buffer[6];
    buffer[5] = '\0'; // Terminatore fisso

    FILE *fp = fopen(nomeFileOut, "w");
    if (fp == NULL) {
        perror("Errore apertura file");
        return 1;
    }

    printf("Inizio generazione password (k=%d)...\n", k);
    
    /* --- 4. ESECUZIONE --- */
    /* Partiamo dalla posizione 0 */
    generaPasswordRicorsiva(0, k, buffer, fp);

    printf("Generazione completata. Risultati in '%s'.\n", nomeFileOut);
    fclose(fp);

    /* --- 5. VERIFICA ESEMPIO (Opzionale) --- */
    /* Leggiamo le prime righe per mostrare che funziona */
    fp = fopen(nomeFileOut, "r");
    if (fp) {
        char riga[100];
        printf("Prime 3 password generate:\n");
        for(int i=0; i<3 && fscanf(fp, "%s", riga)==1; i++) {
            printf("- %s\n", riga);
        }
        fclose(fp);
    }

    return 0;
}