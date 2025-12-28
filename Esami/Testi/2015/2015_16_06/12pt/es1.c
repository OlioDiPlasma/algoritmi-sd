#include <stdio.h>
#include <stdlib.h>

/*
 * ======================================================================================
 * NOME FUNZIONE: mul
 * ======================================================================================
 * SCOPO:
 * Implementa la moltiplicazione "in colonna" (algoritmo classico) tra due numeri interi
 * molto grandi, memorizzati come array di cifre singole.
 * * Esempio logico: v1={3,2}, v2={4,3} -> Rappresentano 32 * 43.
 * Il risultato viene allocato dinamicamente e restituito tramite un parametro di output.
 *
 * PARAMETRI:
 * * v1 : (int *) Puntatore al primo array di cifre (moltiplicando).
 * * v2 : (int *) Puntatore al secondo array di cifre (moltiplicatore).
 * n  : (int)   Numero di cifre contenute in v1 e v2.
 * **pv : (int **) DOPPIO PUNTATORE al vettore risultato.
 *
 * CONCETTI CHIAVE:
 * 1. Doppio Puntatore (**pv):
 * La funzione deve creare un NUOVO array (malloc) e dire al main dove si trova.
 * In C, i parametri sono passati per valore (copia). Se passassimo solo `int *pv`,
 * modificheremmo solo la copia locale del puntatore.
 * Passando l'indirizzo del puntatore (`int **`), possiamo scrivere all'indirizzo
 * originale nel main.
 * È come dire: "Non darmi la casa (il valore), dammi l'indirizzo della casa (puntatore),
 * anzi, dimmi dove è scritto l'indirizzo sulla mappa (doppio puntatore)".
 *
 * 2. Dimensione 2n:
 * Moltiplicando due numeri di N cifre, il risultato ha al massimo 2*N cifre.
 * Esempio: 99 * 99 = 9801 (2 cifre * 2 cifre = 4 cifre).
 *
 * 3. Logica "Posizionale":
 * Quando moltiplichiamo v1[i] * v2[j], il risultato contribuisce alle posizioni
 * [i + j] e [i + j + 1] del vettore finale.
 * ======================================================================================
 */
void mul(int *v1, int *v2, int n, int **pv) {

    /* --- 1. CALCOLO DIMENSIONE E ALLOCAZIONE --- */
    
    int dim_res = 2 * n; // La dimensione del risultato è il doppio dell'input

    /* * GESTIONE MEMORIA (CALLOC vs MALLOC):
     * Usiamo calloc invece di malloc. 
     * calloc(numero_elementi, dimensione_elemento)
     * Perché? calloc inizializza tutti i bit a 0. 
     * Poiché dovremo fare delle somme cumulative (+=) nelle celle del risultato,
     * è vitale partire da zero e non da valori "spazzatura" della RAM.
     *
     * Sintassi critica: *pv = ...
     * Dereferenziamo il doppio puntatore una volta per accedere alla variabile 
     * puntatore del main e assegnarle il nuovo indirizzo.
     */
    *pv = (int *)calloc(dim_res, sizeof(int));

    /* Controllo difensivo: sempre verificare se la RAM è piena */
    if (*pv == NULL) {
        printf("Errore critico: Memoria esaurita.\n");
        return;
    }

    /* Per comodità, usiamo un puntatore locale 'res' per non scrivere sempre (*pv)[k] */
    int *res = *pv;

    /* --- 2. LOGICA DI MOLTIPLICAZIONE (Nested Loops) --- */
    
    /* * Scorriamo i due vettori da destra verso sinistra (dalle unità verso le decine/centinaia),
     * proprio come si fa nella moltiplicazione su carta.
     * i = indice per v1
     * j = indice per v2
     */
    for (int i = n - 1; i >= 0; i--) {
        for (int j = n - 1; j >= 0; j--) {
            
            /* Calcoliamo il prodotto delle singole cifre */
            int prodotto = v1[i] * v2[j];

            /* * CALCOLO INDICI RISULTATO:
             * Le posizioni nel vettore risultato corrispondenti a v1[i] e v2[j] sono:
             * p1 (posizione più significativa, per il riporto)
             * p2 (posizione meno significativa, per l'unità)
             * * Esempio con n=3 (indici 0,1,2): 
             * Ultima cifra * Ultima cifra (indici 2 e 2) finisce in fondo al risultato (indice 5).
             * 2 + 2 + 1 = 5.
             */
            int p1 = i + j;     // Indice per le decine (riporto)
            int p2 = i + j + 1; // Indice per le unità

            /* --- 3. SOMMA E GESTIONE RIPORTO --- */
            
            /* * Sommiamo il prodotto alla cella corrente (che potrebbe contenere già valori
             * da iterazioni precedenti).
             */
            int somma = prodotto + res[p2];

            /* * Aggiorniamo la cella delle unità (p2) con il modulo 10.
             * Esempio: se somma è 14, scriviamo 4.
             */
            res[p2] = somma % 10;

            /* * Aggiungiamo il riporto alla cella precedente (p1).
             * Esempio: se somma è 14, aggiungiamo 1 alla posizione p1.
             * Notare il += perché p1 potrebbe a sua volta avere già un valore.
             */
            res[p1] += somma / 10;
        }
    }
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Scopo: Testare la funzione con l'esempio del testo (032 * 243).
 * ======================================================================================
 */
int main() {
    /* --- 1. DEFINIZIONE DATI DI PROVA --- */
    /* * Testo dell'esercizio:
     * v1 = 0 3 2 (rappresenta 32)
     * v2 = 2 4 3 (rappresenta 243)
     * n = 3
     */
    int n = 3;
    
    /* Usiamo array statici per l'input */
    int v1[] = {0, 3, 2}; 
    int v2[] = {2, 4, 3};

    /* * Puntatore per il risultato.
     * Inizialmente non punta a nulla (NULL).
     * La funzione 'mul' deciderà dove farlo puntare.
     */
    int *risultato = NULL;

    printf("--- INIZIO TEST MOLTIPLICAZIONE ---\n");
    printf("Moltiplico: ");
    for(int k=0; k<n; k++) printf("%d", v1[k]);
    printf(" x ");
    for(int k=0; k<n; k++) printf("%d", v2[k]);
    printf("\n");

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    /* * DETTAGLIO SINTATTICO CRUCIALE:
     * Passiamo &risultato (l'indirizzo del puntatore).
     * Quindi il parametro 'pv' riceverà un 'int **'.
     */
    mul(v1, v2, n, &risultato);

    /* --- 3. STAMPA DEL RISULTATO --- */
    /* La dimensione del risultato è 2*n, quindi 6 */
    printf("Risultato : ");
    int dim_res = 2 * n;
    
    /* Controllo sicurezza: se malloc ha fallito, risultato è ancora NULL */
    if (risultato != NULL) {
        /*
         * Algoritmo opzionale per saltare gli zeri iniziali (leading zeros),
         * tranne se il numero è proprio 0.
         */
        int startPrinting = 0; 
        for (int i = 0; i < dim_res; i++) {
            // Se troviamo un numero diverso da 0, iniziamo a stampare sempre
            if (risultato[i] != 0) startPrinting = 1;
            
            // Se siamo alla fine e non abbiamo stampato nulla (es. risultato 0), stampa l'ultimo 0
            if (i == dim_res - 1) startPrinting = 1;

            // Stampiamo tutto o solo se abbiamo superato gli zeri iniziali?
            // Per fedeltà all'esercizio stampiamo TUTTO come nel testo (007776).
            printf("%d", risultato[i]);
        }
        printf("\n");
        printf("Atteso    : 007776 (poiche' 32 * 243 = 7776)\n");
    } else {
        printf("Errore: Il risultato è NULL.\n");
    }

    /* --- 4. LIBERAZIONE MEMORIA (FREE) --- */
    /*
     * GESTIONE MEMORIA:
     * La memoria è stata allocata dentro 'mul' con calloc, ma appartiene allo heap.
     * Dobbiamo liberarla noi nel main quando non serve più.
     */
    free(risultato);
    risultato = NULL; /* Buona norma anti-dangling pointer */

    return 0;
}