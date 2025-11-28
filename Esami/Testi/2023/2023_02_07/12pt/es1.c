/* * Inclusione delle librerie standard:
 * <stdio.h>:  Serve per le funzioni di Input/Output (es. printf).
 * <stdlib.h>: Serve per la gestione della memoria dinamica (malloc, free).
 */
#include <stdio.h>
#include <stdlib.h>

/**
 * FUNZIONE f
 * Scopo: Creare una nuova matrice di double contenente le medie dei vicini.
 * * Parametri:
 * - int **M:        Puntatore alla matrice di input (interi). È un puntatore a puntatori.
 * - int r:          Numero di righe della matrice.
 * - int c:          Numero di colonne della matrice.
 * - double ***M_prime: Triplo puntatore per l'output.
 * PERCHÉ TRIPLO?  Noi vogliamo che la funzione crei una matrice (che è un double**) 
 * e la "restituisca" al main. In C i parametri passano per valore. Se passassimo 
 * solo double**, modificheremmo una copia locale. Passando l'indirizzo del puntatore 
 * (double***), possiamo modificare direttamente la variabile dichiarata nel main.
 */
void f(int **M, int r, int c, double ***M_prime) {
    
    /* --- 1. ALLOCAZIONE DELLA MEMORIA --- */
    
    /* * Allocazione dell'array di puntatori alle righe (la "colonna vertebrale" della matrice).
     * *M_prime: Accediamo alla variabile del main dereferenziando il triplo puntatore.
     * malloc(r * sizeof(double *)): Chiediamo memoria per 'r' puntatori.
     */
    *M_prime = (double **)malloc(r * sizeof(double *));
    
    /* * Ora, per ogni riga, dobbiamo allocare lo spazio per le colonne (i dati veri e propri).
     */
    for (int i = 0; i < r; i++) {
        /*
         * (*M_prime)[i]: Accediamo all'i-esima riga della nuova matrice.
         * malloc(c * sizeof(double)): Chiediamo memoria per 'c' numeri decimali (double).
         */
        (*M_prime)[i] = (double *)malloc(c * sizeof(double));
    }

    /* --- 2. LOGICA DI CALCOLO (Sliding Window) --- */

    /* Ciclo esterno: scorre tutte le righe 'i' da 0 a r-1 */
    for (int i = 0; i < r; i++) {
        /* Ciclo interno: scorre tutte le colonne 'j' da 0 a c-1 */
        for (int j = 0; j < c; j++) {
            
            /* * Variabili di accumulo per la media.
             * somma: conterrà la somma dei valori dei vicini validi (inizializzata a 0.0 per i decimali).
             * contatore: conterrà il numero di vicini validi trovati (4, 6 o 9).
             */
            double somma = 0.0;
            int contatore = 0;

            /* * DOPPIO CICLO PER I VICINI
             * Immagina una finestra 3x3 centrata su M[i][j].
             * 'x' scorre le righe: riga sopra (i-1), riga corrente (i), riga sotto (i+1).
             * 'y' scorre le colonne: colonna sinistra (j-1), corrente (j), destra (j+1).
             */
            for (int x = i - 1; x <= i + 1; x++) {
                for (int y = j - 1; y <= j + 1; y++) {
                    
                    /* * CONTROLLO BORDI (Boundary Check)
                     * Prima di leggere M[x][y], dobbiamo essere sicuri che x e y esistano.
                     * x >= 0 && x < r: verifica che non siamo usciti sopra o sotto.
                     * y >= 0 && y < c: verifica che non siamo usciti a sinistra o destra.
                     */
                    if (x >= 0 && x < r && y >= 0 && y < c) {
                        /* Se l'indice è valido, aggiungiamo il valore alla somma */
                        somma += M[x][y];
                        /* Incrementiamo il contatore dei vicini trovati */
                        contatore++;
                    }
                    /* Se l'if è falso (es. siamo su un angolo e cerchiamo un vicino inesistente), non facciamo nulla */
                }
            }

            /* * CALCOLO E ASSEGNAZIONE
             * Calcoliamo la media (somma / contatore).
             * Scriviamo il risultato nella cella [i][j] della nuova matrice allocata.
             * Le parentesi (*M_prime) sono obbligatorie per la precedenza degli operatori.
             */
            (*M_prime)[i][j] = somma / contatore;
        }
    }
}

/* --- MAIN DI TEST --- */
int main() {
    /* Definiamo le dimensioni della matrice */
    int r = 3;
    int c = 3;

    /* * 1. ALLOCAZIONE MATRICE DI INPUT
     * Dobbiamo simulare una matrice dinamica (int **) perché la funzione 'f' accetta quella.
     * Non possiamo usare int M[3][3] statico perché non è compatibile con int**.
     */
    
    /* Allocazione righe (array di puntatori a interi) */
    int **M = (int **)malloc(r * sizeof(int *));
    
    /* Allocazione colonne per ogni riga */
    for(int i = 0; i < r; i++) {
        M[i] = (int *)malloc(c * sizeof(int));
    }

    /* * 2. RIEMPIMENTO DATI (HARDCODED)
     * Inseriamo manualmente i valori dell'esempio dell'esercizio.
     */
    
    /* Riga 0: 1, 2, 1 */
    M[0][0] = 1; M[0][1] = 2; M[0][2] = 1;
    /* Riga 1: 2, 0, 2 */
    M[1][0] = 2; M[1][1] = 0; M[1][2] = 2;
    /* Riga 2: 1, 2, 1 */
    M[2][0] = 1; M[2][1] = 2; M[2][2] = 1;

    /* * 3. STAMPA MATRICE ORIGINALE
     * Serve solo per verificare che i dati siano stati inseriti correttamente.
     */
    printf("Matrice Originale (M):\n");
    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
            /* %d stampa un intero, \t inserisce una tabulazione per allineare */
            printf("%d\t", M[i][j]);
        }
        /* \n va a capo alla fine di ogni riga della matrice */
        printf("\n");
    }
    printf("\n"); /* Riga vuota estetica */

    /* * 4. PREPARAZIONE VARIABILE RISULTATO
     * Dichiariamo un puntatore doppio che conterrà la matrice delle medie.
     * Al momento non punta a nulla (contiene spazzatura), sarà la funzione 'f' a riempirlo.
     */
    double **M_risultato;

    /* * 5. CHIAMATA ALLA FUNZIONE
     * Passiamo M, r, c normalmente.
     * Passiamo &M_risultato (l'indirizzo di M_risultato) affinché la funzione 
     * possa allocare la memoria e agganciarla a questa variabile.
     */
    f(M, r, c, &M_risultato);

    /* * 6. STAMPA DEL RISULTATO
     * Iteriamo sulla matrice appena creata dalla funzione.
     */
    printf("Matrice delle Medie (M'):\n");
    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
            /* %.2f indica di stampare un float/double con esattamente 2 cifre decimali */
            printf("%.2f\t", M_risultato[i][j]); 
        }
        printf("\n");
    }

    /* * 7. PULIZIA DELLA MEMORIA (FREE)
     * In C non esiste il Garbage Collector. Tutta la memoria allocata con malloc 
     * deve essere liberata con free per evitare Memory Leaks.
     */
    for(int i = 0; i < r; i++) { 
        free(M[i]);          /* Libera la riga i della matrice M */
        free(M_risultato[i]); /* Libera la riga i della matrice M_risultato */
    }
    free(M);            /* Libera il vettore dei puntatori di M */
    free(M_risultato);  /* Libera il vettore dei puntatori di M_risultato */

    /* Restituisce 0 al sistema operativo per dire "tutto ok, programma finito senza errori" */
    return 0;
}