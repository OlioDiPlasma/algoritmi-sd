#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Necessaria per strlen()

/*
 * ======================================================================================
 * NOME FUNZIONE: mat_search
 * ======================================================================================
 * SCOPO:
 * Cerca quante volte una specifica stringa 's' compare all'interno di una matrice
 * di caratteri 'mat'. La ricerca deve avvenire in due direzioni:
 * 1. Orizzontale (da sinistra a destra).
 * 2. Verticale (dall'alto verso il basso).
 * La funzione restituisce il numero totale di occorrenze trovate.
 *
 * PARAMETRI:
 * - mat: (char **) La matrice di caratteri. È un puntatore a puntatori (array di stringhe).
 * - r:   (int)     Numero di righe della matrice.
 * - c:   (int)     Numero di colonne della matrice.
 * - s:   (char *)  La stringa da cercare (pattern).
 *
 * CONCETTI CHIAVE:
 * 1. Rappresentazione della Matrice (char **):
 * In C, una matrice dinamica è spesso gestita come un array di puntatori.
 * 'mat' punta al primo elemento di un array di puntatori alle righe.
 * 'mat[i]' è il puntatore all'inizio della riga i-esima.
 * 'mat[i][j]' è il carattere alla riga i, colonna j.
 *
 * 2. Limiti dei Cicli (Bounds Checking):
 * Quando cerchiamo una stringa di lunghezza L in una riga di lunghezza C,
 * non possiamo partire dopo l'indice (C - L). Se lo facessimo, usciremmo
 * fuori dai bordi della matrice leggendo memoria sporca (Segmentation Fault).
 * Quindi il ciclo interno deve fermarsi a: indice <= dimensione - lunghezza.
 * ======================================================================================
 */
int mat_search(char **mat, int r, int c, char *s) {
    
    /* --- 1. PREPARAZIONE --- */
    int count = 0;              // Contatore delle occorrenze trovate
    int len = (int)strlen(s);   // Lunghezza della stringa da cercare
    
    // Se la stringa è più lunga delle dimensioni della matrice, è inutile cercare.
    if (len > r && len > c) {
        return 0;
    }

    /* --- 2. RICERCA ORIZZONTALE (Per ogni riga) --- */
    // Scorro tutte le righe da 0 a r-1
    for (int i = 0; i < r; i++) {
        // Scorro le colonne.
        // ATTENZIONE: Mi fermo quando non c'è più spazio sufficiente per la stringa.
        // Esempio: Colonna lung 5, cerco "foo" (lung 3). Posso partire solo dagli indici 0, 1, 2.
        // Indice massimo start: c - len.
        for (int j = 0; j <= c - len; j++) {
            
            // Verifico se c'è un match a partire da mat[i][j]
            int match = 1; // Assumo che sia vero, provo a smentirlo
            
            for (int k = 0; k < len; k++) {
                // Confronto il carattere k-esimo della stringa con la cella della matrice.
                // mat[i][j + k]: mi sposto orizzontalmente sulla stessa riga
                if (mat[i][j + k] != s[k]) {
                    match = 0; // Trovata discrepanza
                    break;     // Esco dal ciclo interno inutile continuare
                }
            }
            
            // Se match è rimasto 1, abbiamo trovato la stringa intera
            if (match == 1) {
                count++;
            }
        }
    }

    /* --- 3. RICERCA VERTICALE (Per ogni colonna) --- */
    // Scorro tutte le colonne da 0 a c-1
    for (int j = 0; j < c; j++) {
        // Scorro le righe.
        // ATTENZIONE: Anche qui, mi fermo quando non c'è abbastanza spazio verticale.
        // Indice massimo start: r - len.
        for (int i = 0; i <= r - len; i++) {
            
            int match = 1;
            
            for (int k = 0; k < len; k++) {
                // Confronto il carattere k-esimo della stringa con la cella matrice.
                // mat[i + k][j]: mi sposto verticalmente, cambiando riga ma tenendo la colonna fissa
                if (mat[i + k][j] != s[k]) {
                    match = 0;
                    break;
                }
            }
            
            if (match == 1) {
                count++;
            }
        }
    }

    return count;
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Scopo: 
 * Creare la matrice esatta dell'esempio fornito nell'immagine, popolarla,
 * invocare la funzione mat_search e verificare che il risultato sia 3.
 * Gestisce rigorosamente l'allocazione e la deallocazione della memoria.
 * ======================================================================================
 */
int main() {
    /* --- 1. DEFINIZIONE DATI DI PROVA --- */
    int R = 4; // Righe nell'esempio
    int C = 5; // Colonne nell'esempio
    char *target = "foo";

    // Dati estratti dall'immagine dell'esercizio:
    // Riga 0: x f o o x
    // Riga 1: y o x z f
    // Riga 2: x o 2 f o
    // Riga 3: g 4 x a o
    // Nota: Ho analizzato l'immagine: le occorrenze sono:
    // 1. Orizzontale R0, start C1 ("foo")
    // 2. Verticale C1, start R0 ("f" in R0, "o" in R1, "o" in R2)
    // 3. Verticale C4, start R1 ("f" in R1, "o" in R2, "o" in R3)
    
    char dati[4][5] = {
        {'x', 'f', 'o', 'o', 'x'},
        {'y', 'o', 'x', 'z', 'f'},
        {'x', 'o', '2', 'f', 'o'},
        {'g', '4', 'x', 'a', 'o'}
    };

    /* --- 2. ALLOCAZIONE DINAMICA DELLA MATRICE (Heap) --- */
    // Passo 1: Alloco l'array di puntatori alle righe
    // sizeof(char *) perché ogni elemento è un puntatore
    char **matrice = (char **)malloc(R * sizeof(char *));
    if (matrice == NULL) {
        perror("Errore malloc righe");
        exit(1);
    }

    // Passo 2: Alloco ogni singola riga
    for (int i = 0; i < R; i++) {
        matrice[i] = (char *)malloc(C * sizeof(char));
        if (matrice[i] == NULL) {
            perror("Errore malloc colonne");
            exit(1);
        }
    }

    /* --- 3. POPOLAMENTO DELLA MATRICE --- */
    printf("--- Matrice di Test ---\n");
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            matrice[i][j] = dati[i][j];
            printf("%c ", matrice[i][j]);
        }
        printf("\n");
    }

    /* --- 4. CHIAMATA ALLA FUNZIONE --- */
    printf("\nCerco la stringa: \"%s\"\n", target);
    int risultato = mat_search(matrice, R, C, target);

    /* --- 5. STAMPA RISULTATI --- */
    printf("Occorrenze trovate: %d\n", risultato);
    printf("Risultato atteso dall'esempio: 3\n");

    /* --- 6. PULIZIA MEMORIA (FREE) --- */
    // Regola d'oro: Free in ordine inverso rispetto alla Malloc.
    // Prima libero le singole righe (i "figli").
    for (int i = 0; i < R; i++) {
        free(matrice[i]); 
    }
    // Poi libero l'array dei puntatori (il "padre").
    free(matrice);

    return 0;
}