#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * FUNZIONE: visualizza_sottovettori_max
 * ======================================================================================
 * SCOPO:
 * Identifica e stampa a video tutti i sottovettori (sequenze contigue) composti da
 * elementi non nulli che hanno la lunghezza massima possibile all'interno di V.
 *
 * PARAMETRI:
 * - int *V: Il puntatore al primo elemento del vettore di interi.
 * - int N: La dimensione totale del vettore.
 *
 * CONCETTI CHIAVE:
 * - Algoritmo a due passate:
 * 1. Calcoliamo la lunghezza massima (max_len) per sapere cosa cercare.
 * 2. Ripercorriamo l'array per stampare solo le sequenze che hanno quella lunghezza.
 * - Gestione dei bordi: Una sequenza finisce quando incontriamo uno 0 OPPURE
 * quando finisce l'array (i == N). Gestiremo entrambi i casi.
 * ======================================================================================
 */
void visualizza_sottovettori_max(int *V, int N) {
    
    /* --- 1. PRIMA PASSATA: TROVARE LA LUNGHEZZA MASSIMA --- */
    
    int max_len = 0;      // Memorizza la lunghezza massima trovata finora
    int current_len = 0;  // Contatore per la sequenza corrente
    
    for (int i = 0; i < N; i++) {
        /*
         * DETTAGLIO SINTATTICO:
         * V[i] è zucchero sintattico per *(V + i).
         * Accediamo al valore intero alla posizione i-esima rispetto all'indirizzo base V.
         */
        if (V[i] != 0) {
            // Se il numero non è nullo, la sequenza continua
            current_len++;
        } else {
            // Trovato uno 0: la sequenza si interrompe.
            // Controlliamo se la sequenza appena chiusa è un nuovo record.
            if (current_len > max_len) {
                max_len = current_len;
            }
            // Resettiamo il contatore per la prossima sequenza
            current_len = 0;
        }
    }
    
    /* CASO LIMITE: Se l'array finisce con numeri non nulli (es: 1 2 3),
       il ciclo finisce senza entrare nell'else. Dobbiamo fare un ultimo controllo. */
    if (current_len > max_len) {
        max_len = current_len;
    }

    // Se max_len è 0, significa che il vettore è tutto zeri. Non c'è nulla da stampare.
    if (max_len == 0) {
        printf("Nessuna sequenza non nulla trovata.\n");
        return;
    }

    printf("Lunghezza massima individuata: %d. Ecco le sequenze:\n", max_len);

    /* --- 2. SECONDA PASSATA: STAMPARE LE SEQUENZE --- */
    
    current_len = 0; // Azzeriamo il contatore per riutilizzarlo

    /* Nota: Usiamo un ciclo che va da 0 fino a N incluso.
       Trattiamo l'indice N come un "fine virtuale" per forzare il controllo
       anche dell'ultima sequenza. */
    for (int i = 0; i <= N; i++) {
        
        /* Verifichiamo se siamo ancora dentro una sequenza valida.
           La sequenza continua se:
           1. Non siamo arrivati alla fine dell'array (i < N)
           2. E il valore corrente non è zero (V[i] != 0) */
        if (i < N && V[i] != 0) {
            current_len++;
        } 
        else {
            /* Siamo su uno 0 oppure alla fine del vettore (i==N).
               La sequenza corrente è terminata. Controlliamo quanto era lunga. */
            
            if (current_len == max_len) {
                /* --- LOGICA DI STAMPA --- */
                printf("[ ");
                /* Dobbiamo tornare indietro per stampare gli elementi.
                   La sequenza è finita all'indice 'i' (escluso), ed è lunga 'max_len'.
                   Quindi inizia a: i - max_len. */
                for (int k = i - max_len; k < i; k++) {
                    printf("%d ", V[k]);
                }
                printf("]\n");
            }
            
            // Reset del contatore per la prossima sequenza
            current_len = 0;
        }
    }
}

/*
 * ======================================================================================
 * FUNZIONE: main
 * ======================================================================================
 * SCOPO:
 * Main di test. Alloca dinamicamente un vettore per simulare l'input,
 * chiama la funzione e libera la memoria.
 * ======================================================================================
 */
int main() {
    /* --- 1. DEFINIZIONE DATI --- */
    // Usiamo i dati dell'esempio: 1 3 4 0 1 0 9 4 2 0
    // Lunghezza totale N = 10
    int dati_esempio[] = {1, 3, 4, 0, 1, 0, 9, 4, 2, 0};
    int N = 10;

    /* --- 2. ALLOCAZIONE DINAMICA (SIMULAZIONE) --- */
    /* Anche se potremmo passare 'dati_esempio', allochiamo dinamicamente
       per esercitarci con la gestione della memoria come richiesto. */
       
    /* malloc restituisce un puntatore generico (void*), che castiamo a (int*)
       sizeof(int) ci dice quanti byte occupa un intero (di solito 4).
       Quindi chiediamo: 10 * 4 = 40 bytes. */
    int *V = (int *)malloc(N * sizeof(int));

    /* Controllo fondamentale: malloc può fallire se la RAM è piena. */
    if (V == NULL) {
        perror("Errore malloc"); // Stampa errore su stderr
        return 1;
    }

    /* Copiamo i dati dall'array stack all'array heap */
    for (int i = 0; i < N; i++) {
        V[i] = dati_esempio[i];
    }

    /* --- 3. ESECUZIONE --- */
    printf("--- INIZIO TEST ---\n");
    printf("Vettore in input: ");
    for(int i=0; i<N; i++) printf("%d ", V[i]);
    printf("\n\n");

    visualizza_sottovettori_max(V, N);

    /* --- 4. GESTIONE MEMORIA (FREE) --- */
    /* Ogni malloc deve avere una free corrispondente.
       Se non lo facciamo, creiamo un Memory Leak (la memoria resta occupata
       fino alla chiusura del programma). */
    free(V);
    
    /* Buona norma: dopo la free, impostare il puntatore a NULL per evitare
       di usarlo per sbaglio (dangling pointer). */
    V = NULL;

    printf("\n--- FINE TEST ---\n");
    return 0;
}