#include <stdio.h>
#include <stdlib.h>

/* Definiamo la dimensione del piano cartesiano come costante */
#define DIM 100

/* ============================================================================
 * FUNZIONE: areaTot
 * ============================================================================
 * SCOPO:
 * Calcola l'area totale coperta dall'unione di più rettangoli su una griglia
 * 100x100. Legge le coordinate da un file e gestisce le sovrapposizioni
 * "accendendo" le celle di una matrice.
 *
 * PARAMETRI:
 * - fp: FILE *
 * Puntatore a un file GIÀ APERTO in modalità lettura ("r"). Il file contiene
 * righe nel formato: x1 y1 x2 y2 (coordinate basso-sx e alto-dx).
 *
 * RITORNA:
 * - int: Il numero totale di celle uniche coperte dai rettangoli (l'area).
 *
 * CONCETTI CHIAVE:
 * 1. Discretizzazione: Il piano continuo viene trattato come una griglia discreta.
 * Un rettangolo che va da x=1 a x=4 copre le celle 1, 2 e 3.
 * Quindi i cicli for andranno da x1 (incluso) a x2 (escluso).
 * 2. Matrice di Flag: Usiamo una matrice 100x100 inizializzata a 0.
 * Ogni volta che un rettangolo copre una cella [i][j], impostiamo
 * matrice[i][j] = 1. Se una cella era già 1, resta 1.
 * Questo risolve automaticamente il problema delle intersezioni.
 * ============================================================================
 */
int areaTot(FILE *fp) {

    /* --- 1. ALLOCAZIONE E INIZIALIZZAZIONE GRIGLIA --- */
    /*
     * Vogliamo rappresentare il piano 100x100.
     * Anche se 100x100 è piccolo e potrebbe stare sullo stack (int griglia[100][100]),
     * allocarlo dinamicamente è un ottimo esercizio di gestione della memoria.
     *
     * Usiamo calloc invece di malloc.
     * PERCHÉ? calloc inizializza automaticamente tutta la memoria a 0.
     * È fondamentale perché 0 significa "cella vuota".
     */
    
    // Allochiamo un puntatore per simulare una matrice bidimensionale linearizzata
    // Oppure, più didattico per i doppi puntatori, allochiamo le righe.
    // Qui userò l'approccio "array appiattito" (1D che simula 2D) per variazione didattica,
    // oppure il classico int** per coerenza con le lezioni precedenti.
    // Manteniamo la coerenza con int**:
    
    /* Allocazione array di puntatori (le righe) */
    int **griglia = (int **)malloc(DIM * sizeof(int *));
    if (griglia == NULL) {
        printf("Errore malloc righe\n");
        return -1;
    }

    /* Allocazione delle colonne per ogni riga */
    for (int i = 0; i < DIM; i++) {
        /* calloc(numero_elementi, dimensione_elemento)
           Setta i bit a zero. Fondamentale qui. */
        griglia[i] = (int *)calloc(DIM, sizeof(int));
        if (griglia[i] == NULL) {
            printf("Errore calloc colonne\n");
            // Nota: Qui servirebbe un ciclo di free per pulire ciò che è stato allocato prima
            return -1;
        }
    }

    /* --- 2. LOGICA DI LETTURA E MAPPATURA --- */
    int x1, y1, x2, y2;

    /* fscanf restituisce il numero di campi letti con successo.
       Finché ne legge 4 (x1, y1, x2, y2), continuiamo il ciclo. */
    while (fscanf(fp, "%d %d %d %d", &x1, &y1, &x2, &y2) == 4) {
        
        /* Controlli di sicurezza sui bordi (buona norma difensiva) */
        if (x1 < 0) x1 = 0;
        if (y1 < 0) y1 = 0;
        if (x2 > DIM) x2 = DIM;
        if (y2 > DIM) y2 = DIM;

        /* Dettaglio Sintattico: I CICLI NEIDIFICATI
           Iteriamo attraverso l'area del rettangolo.
           IMPORTANTE: i < x2 (strettamente minore).
           Se il rettangolo va da 1 a 2, ha larghezza 1. Copre solo la cella 1.
           Quindi il ciclo deve fare solo i=1. Se facessimo i <= x2, coprirebbe 1 e 2. */
        for (int i = x1; i < x2; i++) {       // Scorre le X
            for (int j = y1; j < y2; j++) {   // Scorre le Y
                
                /* Accedo alla cella di memoria puntata da griglia[i] all'offset [j]
                   e la imposto a 1 (occupato). Non importa se era già 1. */
                griglia[i][j] = 1;
            }
        }
    }

    /* --- 3. CONTEGGIO AREA --- */
    int areaComplessiva = 0;

    /* Scorriamo tutta la matrice per contare quanti "1" ci sono. */
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            if (griglia[i][j] == 1) {
                areaComplessiva++;
            }
        }
    }

    /* --- 4. GESTIONE MEMORIA (CLEANUP) --- */
    /* Prima di uscire, dobbiamo liberare la memoria allocata con malloc/calloc
       per evitare memory leaks. L'ordine è inverso all'allocazione. */
    
    for (int i = 0; i < DIM; i++) {
        free(griglia[i]); // Libera la singola riga (array di int)
    }
    free(griglia); // Libera l'array di puntatori

    return areaComplessiva;
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * SCOPO:
 * Crea un file temporaneo con i dati dell'esempio, chiama la funzione e verifica.
 * Esempio:
 * Rettangolo 1: (1,2) -> (4,4). Larghezza 3, Altezza 2. Area=6.
 * Rettangolo 2: (3,3) -> (5,8). Larghezza 2, Altezza 5. Area=10.
 * Intersezione: X da 3 a 4 (Largh 1), Y da 3 a 4 (Alt 1). Area intersecata = 1.
 * Totale atteso = 6 + 10 - 1 = 15.
 * ============================================================================
 */
int main() {
    /* Nome del file temporaneo */
    const char *filename = "rettangoli_test.txt";

    /* --- 1. CREAZIONE FILE DATI --- */
    FILE *f_out = fopen(filename, "w");
    if (f_out == NULL) {
        perror("Errore creazione file");
        return 1;
    }
    /* Scriviamo i dati dell'esempio */
    fprintf(f_out, "1 2 4 4\n");
    fprintf(f_out, "3 3 5 8\n");
    fclose(f_out);

    /* --- 2. APERTURA FILE IN LETTURA --- */
    FILE *f_in = fopen(filename, "r");
    if (f_in == NULL) {
        perror("Errore apertura file");
        return 1;
    }

    /* --- 3. CHIAMATA ALLA FUNZIONE --- */
    printf("Calcolo area totale dai rettangoli nel file...\n");
    int risultato = areaTot(f_in);

    /* --- 4. STAMPA E CHIUSURA --- */
    printf("Risultato ottenuto: %d\n", risultato);
    printf("Risultato atteso: 15\n");

    if (risultato == 15) {
        printf("-> TEST SUPERATO.\n");
    } else {
        printf("-> TEST FALLITO.\n");
    }

    fclose(f_in);
    
    /* (Opzionale) Rimuove il file temporaneo */
    // remove(filename); 

    return 0;
}