#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =================================================================================
 * FUNZIONE: conta
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Conta quante volte una specifica stringa (pattern) appare all'interno di una 
 * matrice di caratteri. La ricerca avviene in due direzioni:
 * 1. Orizzontale (da sinistra a destra).
 * 2. Verticale (dall'alto verso il basso).
 *
 * PARAMETRI:
 * - char **matrice: Puntatore doppio che rappresenta la matrice (array di array).
 * - int n: Numero di righe della matrice.
 * - int m: Numero di colonne della matrice.
 * - char *stringa: La stringa pattern da cercare (terminata da '\0').
 *
 * CONCETTI CHIAVE:
 * 1. Navigazione Matrice: Usiamo due cicli for annidati per visitare ogni cella 
 * (i, j) della matrice. Ogni cella è considerata un potenziale punto di 
 * partenza per la stringa.
 * 2. Controllo dei Bordi (Boundary Check): Prima di confrontare la stringa, 
 * dobbiamo essere sicuri che ci sia "abbastanza spazio" nella matrice.
 * Esempio: Se cerco "cat" (lunghezza 3) e sono alla colonna m-1 (l'ultima), 
 * non posso cercare in orizzontale perché uscirei dalla memoria.
 * 3. Accesso alla Memoria: Per la ricerca orizzontale, i caratteri sono contigui
 * in memoria (solitamente), ma per quella verticale saltiamo da una riga 
 * all'altra (matrice[i][j], matrice[i+1][j]...), quindi i caratteri sono 
 * lontani in memoria.
 * =================================================================================
 */
int conta(char **matrice, int n, int m, char *stringa) {

    /* --- 1. INIZIALIZZAZIONE --- */
    int contatore_totale = 0;
    int len = strlen(stringa); // Calcoliamo la lunghezza della stringa target

    // Se la stringa è vuota o più grande della matrice stessa, potremmo ritornare 0 subito.
    if (len == 0) return 0;

    /* --- 2. CICLO PRINCIPALE (Scansione di ogni cella) --- */
    // Scorriamo le righe (i) da 0 a n-1
    for (int i = 0; i < n; i++) {
        // Scorriamo le colonne (j) da 0 a m-1
        for (int j = 0; j < m; j++) {

            /* --- 3. RICERCA ORIZZONTALE (Righe) --- */
            // CONTROLLO BORDI: Verifichiamo se c'è spazio sufficiente a destra.
            // Se sono alla colonna j, servono (len) celle. Quindi j + len deve essere <= m.
            if (j + len <= m) {
                int match = 1; // Flag: assumiamo che sia un match, proviamo a smentirlo
                
                // Ciclo interno per confrontare carattere per carattere
                for (int k = 0; k < len; k++) {
                    // Confronto matrice[i][j + k] con stringa[k]
                    // [i] fissa la riga, [j+k] scorre le colonne a destra
                    if (matrice[i][j + k] != stringa[k]) {
                        match = 0; // Trovata differenza
                        break;     // Interrompo il ciclo interno inutile continuare
                    }
                }
                
                if (match == 1) {
                    contatore_totale++;
                }
            }

            /* --- 4. RICERCA VERTICALE (Colonne) --- */
            // CONTROLLO BORDI: Verifichiamo se c'è spazio sufficiente in basso.
            // Se sono alla riga i, servono (len) righe. Quindi i + len deve essere <= n.
            if (i + len <= n) {
                int match = 1;
                
                // Ciclo interno
                for (int k = 0; k < len; k++) {
                    // Confronto matrice[i + k][j] con stringa[k]
                    // [i+k] scorre le righe verso il basso, [j] fissa la colonna
                    if (matrice[i + k][j] != stringa[k]) {
                        match = 0;
                        break;
                    }
                }

                if (match == 1) {
                    contatore_totale++;
                }
            }
        }
    }

    return contatore_totale;
}

/* =================================================================================
 * MAIN DI TEST
 * =================================================================================
 * SCOPO:
 * Creare la matrice dell'esempio (4x5), popolarla con i caratteri indicati,
 * invocare la funzione 'conta' cercando "cat" e stampare il risultato.
 * Infine, liberare correttamente la memoria.
 * =================================================================================
 */
int main() {
    /* --- 1. DEFINIZIONE DATI DI PROVA --- */
    int n = 4; // Righe
    int m = 5; // Colonne
    char *target = "cat";

    /* --- 2. ALLOCAZIONE DINAMICA MATRICE --- */
    // Passo 1: Allocare l'array di puntatori alle righe (spina dorsale)
    // Usiamo sizeof(char*) perché ogni cella conterrà un indirizzo di memoria.
    char **A = (char **)malloc(n * sizeof(char *));
    
    if (A == NULL) {
        printf("Errore malloc righe\n");
        exit(1);
    }

    // Passo 2: Allocare ogni singola riga
    for (int i = 0; i < n; i++) {
        // Usiamo sizeof(char) perché qui memorizziamo i caratteri effettivi
        A[i] = (char *)malloc(m * sizeof(char));
        if (A[i] == NULL) exit(1);
    }

    /* --- 3. POPOLAMENTO DATI (Hardcoded come da esempio) --- */
    // Riga 0: x c e c a
    A[0][0]='x'; A[0][1]='c'; A[0][2]='e'; A[0][3]='c'; A[0][4]='a';
    // Riga 1: w a e c q
    A[1][0]='w'; A[1][1]='a'; A[1][2]='e'; A[1][3]='c'; A[1][4]='q';
    // Riga 2: d t p a z
    A[2][0]='d'; A[2][1]='t'; A[2][2]='p'; A[2][3]='a'; A[2][4]='z';
    // Riga 3: p c a t f
    A[3][0]='p'; A[3][1]='c'; A[3][2]='a'; A[3][3]='t'; A[3][4]='f';

    /* --- 4. VISUALIZZAZIONE MATRICE --- */
    printf("Matrice di test:\n");
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            printf("%c ", A[i][j]);
        }
        printf("\n");
    }
    printf("\nStringa target: %s\n", target);

    /* --- 5. INVOCAZIONE FUNZIONE --- */
    int risultato = conta(A, n, m, target);

    /* --- 6. STAMPA RISULTATI --- */
    printf("Risultato atteso dall'esempio: 3\n");
    printf("Risultato ottenuto: %d\n", risultato);

    /* --- 7. PULIZIA MEMORIA (Fondamentale!) --- */
    // L'ordine di liberazione è inverso all'allocazione:
    // Prima liberiamo le righe (i "figli")...
    for (int i = 0; i < n; i++) {
        free(A[i]); // Libero l'array di char della riga i
    }
    // ...poi liberiamo l'array di puntatori (il "padre")
    free(A);

    return 0;
}