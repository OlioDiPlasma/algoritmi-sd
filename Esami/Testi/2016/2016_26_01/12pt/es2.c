#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * DEFINIZIONE STRUTTURE DATI (ADT Matrice Sparsa)
 * ============================================================================
 * CONCETTO CHIAVE: Lista di Liste
 * Immagina la matrice non come una griglia fissa, ma come uno "scheletro".
 * 1. Abbiamo una lista verticale (RowNode) che rappresenta le righe che contengono dati.
 * Se una riga è tutta zeri, NON esiste in questa lista.
 * 2. Ogni nodo riga punta a una lista orizzontale (ColNode) che contiene i valori
 * delle colonne per quella specifica riga.
 */

/* Nodo della lista orizzontale (COLONNE) */
typedef struct ColNode {
    int col;                // Indice della colonna
    float val;              // Valore del dato (float)
    struct ColNode *next;   // Puntatore al prossimo elemento sulla stessa riga
} ColNode_t;

/* Nodo della lista verticale (RIGHE) */
typedef struct RowNode {
    int row;                // Indice della riga
    ColNode_t *head_cols;   // Puntatore alla testa della lista colonne
    struct RowNode *next;   // Puntatore alla prossima riga non vuota
} RowNode_t;

/* Wrapper (ADT di Prima Categoria) */
typedef struct {
    int nr, nc;             // Dimensioni totali (righe, colonne)
    RowNode_t *head_rows;   // Puntatore alla prima riga presente
} matr_t;

/* ============================================================================
 * FUNZIONE AUSILIARIA: creaColNode
 * ============================================================================
 * SCOPO: Alloca e inizializza un nuovo nodo colonna.
 */
ColNode_t *creaColNode(int c, float d) {
    ColNode_t *new_node = (ColNode_t *)malloc(sizeof(ColNode_t));
    if (new_node == NULL) { printf("Errore malloc col\n"); exit(1); }
    new_node->col = c;
    new_node->val = d;
    new_node->next = NULL;
    return new_node;
}

/* ============================================================================
 * FUNZIONE AUSILIARIA: creaRowNode
 * ============================================================================
 * SCOPO: Alloca e inizializza un nuovo nodo riga.
 */
RowNode_t *creaRowNode(int r) {
    RowNode_t *new_node = (RowNode_t *)malloc(sizeof(RowNode_t));
    if (new_node == NULL) { printf("Errore malloc row\n"); exit(1); }
    new_node->row = r;
    new_node->head_cols = NULL;
    new_node->next = NULL;
    return new_node;
}

/* ============================================================================
 * FUNZIONE PRINCIPALE: MatrWrite
 * ============================================================================
 * SCOPO:
 * Scrive il valore 'd' nella posizione (r, c). Gestisce 4 casi:
 * 1. Inserimento (d!=0, cella vuota)
 * 2. Aggiornamento (d!=0, cella piena)
 * 3. Cancellazione (d==0, cella piena) -> Può causare rimozione riga!
 * 4. Nulla di fatto (d==0, cella vuota)
 *
 * PARAMETRI:
 * - M: Puntatore al wrapper della matrice.
 * - d: Valore float da scrivere.
 * - r: Indice riga.
 * - c: Indice colonna.
 *
 * LOGICA GESTIONE MEMORIA:
 * - Inserimento Ordinato: Dobbiamo cercare la posizione giusta sia per le righe
 * che per le colonne per mantenere la matrice ordinata (fondamentale per le performance).
 * - Pulizia Righe Vuote: Se cancellando un dato una riga diventa vuota,
 * dobbiamo rimuovere anche il nodo RowNode corrispondente.
 * ============================================================================
 */
void MatrWrite(matr_t *M, float d, int r, int c) {
    
    /* --- 1. RICERCA (O CREAZIONE) DELLA RIGA --- */
    /* Dobbiamo trovare la riga 'r' o il punto dove inserirla.
       Usiamo due puntatori per gestire l'inserimento/rimozione in lista singola. */
    RowNode_t *curr_row = M->head_rows;
    RowNode_t *prev_row = NULL;

    /* Scorro finché non trovo la riga o supero il suo indice */
    while (curr_row != NULL && curr_row->row < r) {
        prev_row = curr_row;
        curr_row = curr_row->next;
    }

    /* CASO A: d == 0 (Richiesta di CANCELLAZIONE o NULLA) */
    if (d == 0.0) {
        /* Se la riga non esiste o siamo andati oltre, la cella è già 0. Fine. */
        if (curr_row == NULL || curr_row->row != r) {
            return;
        }

        /* La riga esiste. Cerchiamo la colonna 'c'. */
        ColNode_t *curr_col = curr_row->head_cols;
        ColNode_t *prev_col = NULL;

        while (curr_col != NULL && curr_col->col < c) {
            prev_col = curr_col;
            curr_col = curr_col->next;
        }

        /* Se la colonna esiste, dobbiamo RIMUOVERLA */
        if (curr_col != NULL && curr_col->col == c) {
            /* Sgancio il nodo colonna */
            if (prev_col == NULL) { // Era la testa della lista colonne
                curr_row->head_cols = curr_col->next;
            } else {
                prev_col->next = curr_col->next;
            }
            free(curr_col); // LIBERA MEMORIA

            /* CONTROLLO RIGA VUOTA: Se la riga ora non ha più colonne, va rimossa */
            if (curr_row->head_cols == NULL) {
                if (prev_row == NULL) { // Era la prima riga
                    M->head_rows = curr_row->next;
                } else {
                    prev_row->next = curr_row->next;
                }
                free(curr_row); // LIBERA MEMORIA RIGA
            }
        }
        return;
    }

    /* CASO B: d != 0 (Richiesta di INSERIMENTO o AGGIORNAMENTO) */
    
    /* Se la riga 'r' non esiste, dobbiamo crearla */
    if (curr_row == NULL || curr_row->row != r) {
        RowNode_t *new_row = creaRowNode(r);
        /* Inserimento ordinato della nuova riga */
        new_row->next = curr_row; // curr_row è il successivo (o NULL)
        if (prev_row == NULL) {
            M->head_rows = new_row; // Nuova testa
        } else {
            prev_row->next = new_row;
        }
        /* Ora lavoriamo sulla nuova riga */
        curr_row = new_row;
    }

    /* Ora siamo sicuri di essere sulla riga 'r' corretta.
       Cerchiamo la colonna 'c' o il punto di inserimento. */
    ColNode_t *curr_col = curr_row->head_cols;
    ColNode_t *prev_col = NULL;

    while (curr_col != NULL && curr_col->col < c) {
        prev_col = curr_col;
        curr_col = curr_col->next;
    }

    /* CASO B1: La cella esiste -> AGGIORNAMENTO */
    if (curr_col != NULL && curr_col->col == c) {
        curr_col->val = d;
    }
    /* CASO B2: La cella non esiste -> NUOVO NODO */
    else {
        ColNode_t *new_col = creaColNode(c, d);
        new_col->next = curr_col; // curr_col è il successivo (o NULL)
        
        if (prev_col == NULL) {
            curr_row->head_cols = new_col; // Inserimento in testa alla riga
        } else {
            prev_col->next = new_col;
        }
    }
}

/* ============================================================================
 * MAIN DI TEST e FUNZIONI ACCESSORIE
 * ============================================================================
 */

void inizializzaMatrice(matr_t *M, int r, int c) {
    M->nr = r;
    M->nc = c;
    M->head_rows = NULL;
}

void stampaMatrice(matr_t *M) {
    printf("Matrice Sparsa (%dx%d):\n", M->nr, M->nc);
    RowNode_t *row = M->head_rows;
    while(row != NULL) {
        printf("  Riga %d: [ ", row->row);
        ColNode_t *col = row->head_cols;
        while(col != NULL) {
            printf("(c:%d v:%.1f) ", col->col, col->val);
            col = col->next;
        }
        printf("]\n");
        row = row->next;
    }
    printf("---------------------------\n");
}

/* Funzione per liberare tutta la memoria (Importante!) */
void freeMatrice(matr_t *M) {
    RowNode_t *r = M->head_rows;
    while (r != NULL) {
        ColNode_t *c = r->head_cols;
        while (c != NULL) {
            ColNode_t *temp_c = c;
            c = c->next;
            free(temp_c);
        }
        RowNode_t *temp_r = r;
        r = r->next;
        free(temp_r);
    }
    M->head_rows = NULL;
}

int main() {
    matr_t M;
    inizializzaMatrice(&M, 5, 5);

    printf("--- 1. Inserimenti --- \n");
    MatrWrite(&M, 5.5, 1, 1); // Crea Riga 1, Col 1
    MatrWrite(&M, 3.2, 1, 3); // Aggiungi Col 3 a Riga 1
    MatrWrite(&M, 8.9, 3, 2); // Crea Riga 3, Col 2
    stampaMatrice(&M);

    printf("--- 2. Aggiornamento --- \n");
    MatrWrite(&M, 9.9, 1, 3); // Modifica (1,3) da 3.2 a 9.9
    stampaMatrice(&M);

    printf("--- 3. Cancellazione Elemento --- \n");
    MatrWrite(&M, 0.0, 1, 3); // Rimuovi (1,3). Riga 1 resta perché ha (1,1)
    stampaMatrice(&M);

    printf("--- 4. Cancellazione Riga (Diventa Vuota) --- \n");
    MatrWrite(&M, 0.0, 3, 2); // Rimuovi (3,2). Riga 3 diventa vuota -> rimossa!
    stampaMatrice(&M);

    /* Test Cancellazione su cella non esistente */
    MatrWrite(&M, 0.0, 4, 4); // Non fa nulla
    
    freeMatrice(&M);
    printf("Memoria liberata.\n");
    return 0;
}