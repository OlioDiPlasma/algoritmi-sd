#include <stdio.h>
#include <stdlib.h> // Necessario per malloc, calloc, free

/*
 * Scopo: Calcola la differenza tra due vettori ordinati (a \ b).
 * Crea dinamicamente un nuovo vettore 'c' contenente gli elementi
 * presenti in 'a' ma non presenti in 'b'.
 *
 * Parametri:
 * int *a: Puntatore al primo vettore (minuendo). Ordinato, senza ripetizioni.
 * int na: Dimensione del vettore 'a'.
 * int *b: Puntatore al secondo vettore (sottraendo). Ordinato, senza ripetizioni.
 * int nb: Dimensione del vettore 'b'.
 * int **c_ptr: Puntatore all'indirizzo dove verrà memorizzato il puntatore
 * al vettore risultato 'c'. Questo è un **doppio puntatore**.
 * int *nc_ptr: Puntatore all'intero dove verrà memorizzata la dimensione 'nc'
 * del vettore risultato.
 *
 * Concetti Chiave:
 *
 * 1. Doppio Puntatore (**c_ptr): Il vettore 'c' viene creato *all'interno*
 * della funzione `diffVett` e deve essere "restituito" al chiamante (`main`).
 * Poiché in C si può restituire un solo valore (qui usiamo 'int' come
 * tipo di ritorno per un eventuale codice di errore, ma tecnicamente
 * potremmo usare 'void'), passiamo un puntatore all'area di memoria che
 * nel chiamante (es. nel `main`) tiene il puntatore a 'c'.
 * Questo permette a `diffVett` di *modificare l'indirizzo di memoria*
 * a cui punta il puntatore 'c' del chiamante. Se usassimo 'int *c',
 * la modifica sarebbe locale alla funzione.
 *
 * 2. Algoritmo Merge-Like (Pointer Movement): Sfruttiamo l'ordinamento
 * dei vettori. Utilizziamo due indici (`i` per 'a' e `j` per 'b') che
 * avanzano in modo coordinato. Non usiamo una ricerca binaria o una ricerca
 * sequenziale per ogni elemento, il che riduce la complessità.
 *
 * 3. Allocazione Dinamica in Due Fasi:
 * a) Calcolo: Si scorrono i vettori per determinare l'esatta dimensione 'nc'.
 * b) Allocazione: Si usa `malloc` o `calloc` solo dopo aver saputo 'nc'.
 */
int diffVett(int *a, int na, int *b, int nb, int **c_ptr, int *nc_ptr) {
    // Controllo di base per input nulli o dimensioni non valide
    if (!a || na <= 0 || !c_ptr || !nc_ptr) {
        // Assegno 0 a dimensione e puntatore risultato in caso di fallimento.
        *nc_ptr = 0;
        *c_ptr = NULL;
        return -1; // Ritorna un codice di errore
    }

    /* --- 1. CALCOLO DELLA DIMENSIONE NECESSARIA (nc) --- */
    // L'algoritmo richiede prima di conoscere la dimensione esatta 'nc'
    // per allocare in modo ottimale il vettore risultato.

    int i = 0; // Indice per il vettore 'a'
    int j = 0; // Indice per il vettore 'b'
    int nc = 0; // Contatore degli elementi nella differenza (dimensione finale)

    // Eseguo la scansione parallela dei due vettori (Algoritmo Merge-Like)
    while (i < na && j < nb) {
        // Accedo alla cella di memoria puntata da (a + i) e (b + j)
        if (*(a + i) < *(b + j)) {
            // Se l'elemento di 'a' è minore di quello di 'b',
            // significa che l'elemento di 'a' è *non presente* in 'b'
            // (a causa dell'ordinamento) e fa parte della differenza.
            nc++;
            i++; // Passo al prossimo elemento di 'a'
        } else if (*(a + i) > *(b + j)) {
            // Se l'elemento di 'b' è minore di quello di 'a',
            // significa che l'elemento di 'b' non è rilevante per la differenza
            // e lo salto per cercare un potenziale match più avanti in 'b'.
            j++; // Passo al prossimo elemento di 'b'
        } else { // *(a + i) == *(b + j)
            // L'elemento è presente in entrambi i vettori. Non è nella differenza.
            i++; // Salto l'elemento in 'a'
            j++; // Salto l'elemento in 'b'
        }
    }

    // Se la scansione di 'b' è terminata (j == nb) ma ci sono ancora
    // elementi in 'a' (i < na), tutti i rimanenti elementi di 'a' non sono
    // presenti in 'b' e devono essere inclusi.
    nc += (na - i);

    // Salvo la dimensione calcolata nel puntatore fornito
    *nc_ptr = nc; // Accedo alla cella di memoria puntata da nc_ptr per scriverci 'nc'

    /* --- 2. ALLOCAZIONE DINAMICA E POPOLAMENTO DI 'c' --- */

    if (nc == 0) {
        // Se la dimensione è zero, assegno NULL al puntatore risultato
        // (buona pratica) e il lavoro è finito.
        *c_ptr = NULL;
        return 0;
    }

    // Allocazione:
    // **c_ptr punta al puntatore che conterrà l'indirizzo di 'c'.
    // *c_ptr è l'indirizzo del nuovo blocco di memoria, ovvero il puntatore 'c'.
    // `malloc(nc * sizeof(int))` richiede uno spazio pari a 'nc' volte la
    // dimensione in byte di un intero (`sizeof(int)`).
    *c_ptr = (int *)malloc(nc * sizeof(int));

    /* --- 3. GESTIONE MEMORIA: CONTROLLO MALLOC --- */
    if (*c_ptr == NULL) {
        // Se malloc fallisce (es. memoria esaurita), è un errore grave.
        *nc_ptr = 0;
        fprintf(stderr, "Errore: Allocazione di memoria fallita per il vettore c.\n");
        return -2;
    }

    /* --- 4. POPOLAMENTO DEL VETTORE RISULTATO --- */
    // Reimposto gli indici per la seconda passata (popolamento effettivo)
    i = 0;
    j = 0;
    int k = 0; // Indice per il vettore risultato 'c'

    // Seconda scansione parallela, analoga alla prima
    while (i < na && j < nb) {
        if (*(a + i) < *(b + j)) {
            // Trovato elemento in 'a' ma non in 'b'.
            // Inserisco l'elemento nel risultato 'c' (*c_ptr è il puntatore a 'c')
            // Accedo alla cella di memoria puntata da (*c_ptr + k)
            *((*c_ptr) + k) = *(a + i);
            k++;
            i++;
        } else if (*(a + i) > *(b + j)) {
            j++;
        } else { // *(a + i) == *(b + j)
            i++;
            j++;
        }
    }

    // Aggiungo gli eventuali elementi rimanenti di 'a'
    while (i < na) {
        // Inserisco l'elemento rimanente nel risultato 'c'
        *((*c_ptr) + k) = *(a + i);
        k++;
        i++;
    }

    // La funzione ha avuto successo
    return 0;
}


/* ****************************************************** */
/* ********** PROGRAMMA PRINCIPALE (MAIN) DI TEST ********* */
/* ****************************************************** */

void stampaVettore(const char *nome, int *vett, int n) {
    printf("Vettore %s (dimensione %d): [", nome, n);
    for (int i = 0; i < n; i++) {
        printf("%d", *(vett + i)); // Stampa l'elemento dereferenziando (vett + i)
        if (i < n - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main() {
    /* --- 1. DATI DI PROVA HARDCODED --- */
    // Vettori di input: devono essere ordinati e senza ripetizioni
    int a[] = {1, 3, 5, 7, 9, 11, 13, 15};
    int na = 8; // sizeof(a) / sizeof(a[0]);

    int b[] = {3, 5, 8, 11, 12, 14};
    int nb = 6; // sizeof(b) / sizeof(b[0]);

    // Variabili per il risultato (vanno dichiarate nel chiamante)
    int *c = NULL; // Puntatore al vettore risultato (inizializzato a NULL)
    int nc = 0;   // Dimensione del vettore risultato (inizializzato a 0)

    printf("--- Esecuzione Test Differenza tra Vettori ---\n");
    stampaVettore("a", a, na);
    stampaVettore("b", b, nb);

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    // Nota l'uso dell'operatore & (indirizzo di) per passare:
    // - l'indirizzo del puntatore 'c' (&c) -> int **
    // - l'indirizzo della dimensione 'nc' (&nc) -> int *
    int result = diffVett(a, na, b, nb, &c, &nc);

    printf("\n--- Risultato della Chiamata ---\n");

    if (result == 0) {
        // La funzione ha avuto successo.
        // Il nuovo puntatore a 'c' e la dimensione 'nc' sono stati aggiornati
        // direttamente nella memoria puntata da &c e &nc.
        stampaVettore("c (a \\ b)", c, nc);

        /* --- 3. GESTIONE MEMORIA: LIBERAZIONE --- */
        // Dobbiamo deallocare lo spazio creato dinamicamente con `malloc`
        // all'interno della funzione `diffVett`.

        // Concetto Chiave: FREE
        // Quando non serve più, liberiamo la memoria puntata da 'c'.
        // Questo è cruciale per prevenire **Memory Leaks** (perdite di memoria).
        if (c != NULL) {
            free(c);
            printf("\nMEMORIA: Blocco di memoria puntato da 'c' liberato con free().\n");
            c = NULL; // Buona pratica: impostare il puntatore a NULL dopo la free
        }
    } else {
        printf("ERRORE NELLA FUNZIONE: Codice %d.\n", result);
        printf("MEMORIA: Non c'è memoria da liberare.\n");
    }

    printf("--- Fine del Programma ---\n");

    return 0;
}