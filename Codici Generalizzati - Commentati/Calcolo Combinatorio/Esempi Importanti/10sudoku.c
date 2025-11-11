#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXBUFFER 128   // lunghezza massima di una riga del file

// Variabili globali
int num_sol = 0;        // contatore delle soluzioni trovate

// Prototipi
int **acquisisci(char* nomefile, int *dim_ptr);
int controlla(int **schema, int dim, int pos, int val);
void disp_ripet(int **schema, int dim, int pos);

/* =============================================================
 * MAIN
 * ============================================================= */
int main() {
    int **schema, dim, i;
    char nomefile[20];

    // Richiesta del nome del file
    printf("Inserire il nome del file: ");
    scanf("%s", nomefile);

    // Acquisizione della matrice di partenza
    schema = acquisisci(nomefile, &dim);

    // Avvio della ricerca ricorsiva
    disp_ripet(schema, dim, 0);

    printf("\nNumero di soluzioni trovate = %d\n", num_sol);

    // Liberazione della memoria
    for (i = 0; i < dim; i++)
        free(schema[i]);
    free(schema);

    return 0;
}

/* =============================================================
 * disp_ripet(schema, dim, pos)
 * -------------------------------------------------------------
 * Funzione ricorsiva che cerca di riempire la cella 'pos' 
 * della matrice seguendo le regole del Sudoku.
 *
 * pos = indice lineare della cella (0..dim*dim-1)
 * ============================================================= */
void disp_ripet(int **schema, int dim, int pos) {
    int i, j, t;

    // Caso base: tutte le celle riempite → soluzione completa
    if (pos >= dim * dim) {
        num_sol++;
        printf("Soluzione %d:\n", num_sol);
        for (i = 0; i < dim; i++) {
            for (j = 0; j < dim; j++)
                printf("%3d ", schema[i][j]);
            printf("\n");
        }
        printf("\n");
        return;
    }

    // Determinazione della riga e colonna della cella corrente
    i = pos / dim;
    j = pos % dim;

    // Se la cella non è vuota, passo alla successiva
    if (schema[i][j] != 0) {
        disp_ripet(schema, dim, pos + 1);
        return;
    }

    // Provo tutti i valori possibili da 1 a dim
    for (t = 1; t <= dim; t++) {
        schema[i][j] = t;
        if (controlla(schema, dim, pos, t))
            disp_ripet(schema, dim, pos + 1);  // chiamata ricorsiva
        schema[i][j] = 0;  // BACKTRACKING
    }
}

/* =============================================================
 * controlla(schema, dim, pos, val)
 * -------------------------------------------------------------
 * Controlla se il valore 'val' può essere inserito nella cella
 * 'pos' senza violare le regole del Sudoku.
 *
 * Ritorna 1 se valido, 0 altrimenti
 * ============================================================= */
int controlla(int **schema, int dim, int pos, int val) {
    int i, j, r, c;
    int n = (int)sqrt(dim);  // dimensione dei blocchi

    i = pos / dim;
    j = pos % dim;

    // Controllo riga
    for (c = 0; c < dim; c++)
        if (c != j && schema[i][c] == val)
            return 0;

    // Controllo colonna
    for (r = 0; r < dim; r++)
        if (r != i && schema[r][j] == val)
            return 0;

    // Controllo blocco n x n
    for (r = (i/n)*n; r < (i/n)*n + n; r++)
        for (c = (j/n)*n; c < (j/n)*n + n; c++)
            if ((r != i || c != j) && schema[r][c] == val)
                return 0;

    return 1;  // valore valido
}

/* =============================================================
 * acquisisci(nomefile, dim_ptr)
 * -------------------------------------------------------------
 * Legge lo schema iniziale da file e ritorna la matrice NxN
 *
 * Il file deve avere N righe con N valori separati da spazi
 * ============================================================= */
int **acquisisci(char* nomefile, int *dim_ptr) {
    FILE *fp;
    char buf[MAXBUFFER];
    int i, j, dimensione = 0, **schema;

    // Apertura file
    fp = fopen(nomefile, "r");
    if (!fp) {
        printf("Errore apertura file\n");
        exit(1);
    }

    // Prima lettura per determinare la dimensione
    while (fgets(buf, MAXBUFFER, fp) != NULL)
        dimensione++;
    fclose(fp);

    // Allocazione matrice
    schema = (int**)malloc(dimensione * sizeof(int*));
    for (i = 0; i < dimensione; i++)
        schema[i] = (int*)malloc(dimensione * sizeof(int));

    // Seconda lettura per memorizzare i valori
    fp = fopen(nomefile, "r");
    for (i = 0; i < dimensione; i++)
        for (j = 0; j < dimensione; j++)
            fscanf(fp, "%d", &schema[i][j]);
    fclose(fp);

    *dim_ptr = dimensione;
    return schema;
}
