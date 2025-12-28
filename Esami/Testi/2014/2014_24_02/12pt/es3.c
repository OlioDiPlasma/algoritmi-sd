#include <stdio.h>
#include <stdlib.h>

/* Definiamo l'alfabeto come costante globale per chiarezza */
const char VOCALI[] = {'A', 'E', 'I', 'O', 'U'};
const int NUM_VOCALI = 5;

/* ============================================================================
 * FUNZIONE HELPER: controllaValidita
 * ============================================================================
 * SCOPO:
 * Verifica se una stringa completa soddisfa il vincolo dell'esercizio:
 * "ogni vocale deve comparire almeno una volta".
 *
 * PARAMETRI:
 * - char *sol: La stringa completa generata dalla ricorsione.
 * - int n:     La lunghezza della stringa.
 *
 * RITORNO:
 * - 1 (true) se la stringa contiene tutte e 5 le vocali.
 * - 0 (false) altrimenti.
 * ============================================================================
 */
int controllaValidita(char *sol, int n) {
    /* Array di contatori (flag) per le 5 vocali. Inizializzato a 0.
       Indice 0='A', 1='E', 2='I', 3='O', 4='U' */
    int presenti[5] = {0}; 
    int count_distinte = 0;

    /* Scorro tutta la stringa generata */
    for (int i = 0; i < n; i++) {
        char c = sol[i];
        
        /* Mappo il carattere all'indice dell'array 'presenti' */
        for (int j = 0; j < NUM_VOCALI; j++) {
            if (c == VOCALI[j]) {
                if (presenti[j] == 0) {
                    presenti[j] = 1; // Segno che ho trovato questa vocale
                    count_distinte++;
                }
                break; // Trovata, passo al prossimo char della stringa
            }
        }
    }

    /* Se ho trovato tutte e 5 le vocali distinte, la stringa è valida */
    return (count_distinte == 5);
}

/* ============================================================================
 * FUNZIONE RICORSIVA: generaVocaliRec
 * ============================================================================
 * SCOPO:
 * Riempie ricorsivamente la posizione 'pos' della stringa e richiama se stessa
 * per la posizione successiva. Implementa il "Modello delle Disposizioni con Ripetizione".
 *
 * PARAMETRI:
 * - int pos:    L'indice della cella che stiamo riempiendo in QUESTA chiamata.
 * - int n:      La lunghezza totale da raggiungere.
 * - char *sol:  Il vettore (buffer) dove stiamo costruendo la stringa.
 * - int *count: Puntatore a un contatore per numerare le soluzioni trovate.
 *
 * CONCETTI CHIAVE:
 * 1. Spazio delle Soluzioni:
 * Ad ogni livello di ricorsione, abbiamo 5 scelte possibili (A, E, I, O, U).
 * La funzione prova tutte e 5 le strade grazie al ciclo 'for'.
 *
 * 2. Base della Ricorsione:
 * Quando 'pos' raggiunge 'n', la stringa è piena. Non stampiamo ciecamente,
 * ma chiamiamo la funzione filtro (controllaValidita).
 * ============================================================================
 */
void generaVocaliRec(int pos, int n, char *sol, int *count) {
    
    /* --- 1. CASO BASE (TERMINAZIONE) --- */
    /* Abbiamo riempito le celle da 0 a n-1. 'pos' è arrivato a n. */
    if (pos == n) {
        
        /* Controlliamo se la stringa generata rispetta il vincolo */
        if (controllaValidita(sol, n)) {
            (*count)++; // Incremento il numero di soluzioni valide trovate
            
            /* Stampa formattata della soluzione */
            printf("%d) ", *count);
            for (int i = 0; i < n; i++) {
                printf("%c", sol[i]);
            }
            printf("\n");
        }
        return; // Torno al livello precedente (Backtracking implicito)
    }

    /* --- 2. PASSO RICORSIVO (ESPANSIONE) --- */
    /* Dobbiamo decidere cosa mettere nella cella sol[pos].
       Abbiamo 5 opzioni: le 5 vocali. Le proviamo tutte. */
    for (int i = 0; i < NUM_VOCALI; i++) {
        
        /* A. SCELTA: Metto l'i-esima vocale nella posizione corrente */
        sol[pos] = VOCALI[i];

        /* B. RICORSIONE: Chiedo di completare la stringa dalla pos+1 in poi */
        generaVocaliRec(pos + 1, n, sol, count);

        /* C. BACKTRACKING:
           Non serve nessuna operazione esplicita di "undo" qui, perché
           alla prossima iterazione del ciclo 'for', sol[pos] verrà
           semplicemente sovrascritta dalla vocale successiva. */
    }
}

/* ============================================================================
 * FUNZIONE WRAPPER: generaVocali
 * ============================================================================
 * SCOPO:
 * Prepara le strutture dati necessarie (allocazione memoria) e lancia la ricorsione.
 *
 * PARAMETRI:
 * - int N: Lunghezza della stringa richiesta dall'utente.
 *
 * GESTIONE MEMORIA:
 * Allocazione dinamica del vettore 'sol' nello Heap.
 * Usiamo 'sizeof(char)' per rigore didattico.
 * ============================================================================
 */
void generaVocali(int N) {
    /* Controllo validità input come da specifica (N >= 5) */
    if (N < 5) {
        printf("Errore: N deve essere >= 5 per contenere tutte le vocali.\n");
        return;
    }

    /* --- 1. ALLOCAZIONE --- */
    /* Alloco un array di N caratteri.
       Nota: Non serve il +1 per il '\0' se stampiamo carattere per carattere
       nel ciclo for, ma se volessimo usarlo come stringa C standard (printf %s),
       dovremmo allocare N+1 e mettere '\0' alla fine.
       Qui usiamo l'approccio algoritmico puro (array di char). */
    char *sol = (char *)malloc(N * sizeof(char));
    
    if (sol == NULL) {
        printf("Errore di allocazione memoria.\n");
        return;
    }

    int count = 0; // Contatore soluzioni

    /* --- 2. LANCIO RICORSIONE --- */
    printf("Generazione stringhe lunghezza %d con tutte le 5 vocali...\n", N);
    generaVocaliRec(0, N, sol, &count);

    if (count == 0) {
        printf("Nessuna soluzione trovata (Impossibile con N=%d).\n", N);
    } else {
        printf("Totale soluzioni trovate: %d\n", count);
    }

    /* --- 3. PULIZIA MEMORIA --- */
    free(sol);
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: Testare la funzione con un N piccolo (es. 5) per vedere se genera
 * le permutazioni corrette, e un N leggermente maggiore.
 * ============================================================================
 */
int main() {
    /* CASO DI TEST 1: N=5
     * Ci aspettiamo solo le permutazioni semplici di A,E,I,O,U.
     * (5! = 120 soluzioni attese).
     */
    int N_test = 5;
    generaVocali(N_test);

    printf("\n-----------------------------------\n");

    /* CASO DI TEST 2: N=6 (Opzionale, genera molte stringhe)
     * Questo serve per verificare che gestisca le ripetizioni.
     */
    // N_test = 6;
    // generaVocali(N_test);

    return 0;
}