/* ======================================================================================
 * GUIDA COMPLETA ALGORITMI E STRUTTURE DATI - ESAME 12/18 PUNTI
 * ======================================================================================
 * File unico con TUTTE le strutture essenziali organizzate per tipologia di problema
 * Basato su esami passati: 2024-2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ======================================================================================
 * INDICE RAPIDO
 * ======================================================================================
 * 1. FONDAMENTI C (Puntatori, Stringhe, Memoria)
 * 2. STRUTTURE DATI (Liste, BST, Grafi, Hash)
 * 3. BACKTRACKING (5 Modelli Principali)
 * 4. PROBLEMI LISTE (12pt)
 * 5. PROBLEMI BST (12pt)
 * 6. PROBLEMI VETTORI/STRINGHE (12pt)
 * 7. PROBLEMI GRAFI (18pt)
 * 8. ADT E FILE I/O
 * 9. PATTERN ESAMI PASSATI
 * ====================================================================================== */


/* ======================================================================================
 * 1. FONDAMENTI C - MEMORIA E PUNTATORI
 * ====================================================================================== */

// --- PUNTATORI: LA GERARCHIA DEGLI ASTERISCHI ---
void fondamenti_puntatori() {
    int val = 10;
    int *p1 = &val;         // Livello 1: puntatore a intero
    int **p2 = &p1;         // Livello 2: puntatore a puntatore (matrici 2D)
    int ***p3 = &p2;        // Livello 3: matrice 3D o passare matrice per riferimento
}

// --- ALLOCAZIONE DINAMICA ---
// Matrice 2D (Il pattern più comune)
int **malloc2d(int R, int C) {
    int **mat = malloc(R * sizeof(int *));
    for (int i = 0; i < R; i++) {
        mat[i] = calloc(C, sizeof(int)); // calloc mette a 0
    }
    return mat;
}

// Liberazione matrice 2D
void free2d(int **mat, int R) {
    for (int i = 0; i < R; i++) {
        free(mat[i]);
    }
    free(mat);
}

// --- STRINGHE (SEMPRE UN CASINO!) ---
void stringhe_base() {
    // DICHIARAZIONE
    char s1[] = "Modificabile";     // Stack, posso modificare
    char *s2 = "Solo lettura";      // String literal, NON modificare!
    char *s3 = strdup("Dinamica");  // Malloc interna, ricorda free(s3)!
    
    // CONFRONTO (MAI usare ==)
    if (strcmp(s1, s2) == 0) { /* uguali */ }
    if (strcmp(s1, s2) < 0)  { /* s1 prima di s2 */ }
    
    // COPIA
    char dest[100];
    strcpy(dest, s1);
    
    // CONCATENAZIONE
    strcat(dest, s2);
    
    // LUNGHEZZA
    int len = strlen(s1); // ESCLUSO \0
}

// --- ARRAY DI STRINGHE (char **) ---
char **carica_dizionario(int N) {
    char **dict = malloc(N * sizeof(char *));
    
    for (int i = 0; i < N; i++) {
        char buffer[100];
        scanf("%s", buffer);
        dict[i] = strdup(buffer); // Alloca automaticamente
    }
    return dict;
}


/* ======================================================================================
 * 2. STRUTTURE DATI - DEFINIZIONI ESSENZIALI
 * ====================================================================================== */

// --- LISTA CONCATENATA ---
typedef struct node {
    int val;
    char *key;
    struct node *next;
} Node, *link;

// --- ALBERO BINARIO (BST) ---
typedef struct bst_node {
    int val;
    char *key;
    struct bst_node *l;
    struct bst_node *r;
} BSTnode, *BSTlink;

// Wrapper BST (ADT di prima classe)
typedef struct {
    BSTlink root;
    int size;
} BST_struct;
typedef BST_struct *BST;

// --- GRAFO (Lista di Adiacenza) ---
typedef struct gnode {
    int v;              // Vertice destinazione
    int weight;         // Peso
    struct gnode *next;
} GNode, *Glink;

typedef struct {
    int V;          // Numero vertici
    int E;          // Numero archi
    Glink *ladj;    // Array di liste
} Graph_struct;
typedef Graph_struct *Graph;

// Inizializzazione grafo
Graph GRAPHinit(int V) {
    Graph G = malloc(sizeof(Graph_struct));
    G->V = V;
    G->E = 0;
    G->ladj = calloc(V, sizeof(Glink)); // Tutto a NULL
    return G;
}

// Inserisci arco (in testa)
void GRAPHinsertE(Graph G, int u, int v, int w) {
    Glink nuovo = malloc(sizeof(GNode));
    nuovo->v = v;
    nuovo->weight = w;
    nuovo->next = G->ladj[u];
    G->ladj[u] = nuovo;
    G->E++;
}

// --- HASH TABLE (Chaining) ---
typedef struct hnode {
    char *key;
    int val;
    struct hnode *next;
} HNode;

typedef struct {
    int size;
    HNode **heads;
} HashTable_struct;
typedef HashTable_struct *HashTable;

// Funzione hash (Horner)
int hash(char *v, int M) {
    int h = 0, base = 127;
    for (; *v != '\0'; v++)
        h = (base * h + *v) % M;
    return h;
}


/* ======================================================================================
 * 3. BACKTRACKING - I 5 MODELLI FONDAMENTALI
 * ====================================================================================== */

/* -------------------------------------------------------------------------------------
 * MODELLO 1: BINARIO (Prendo/Lascio)
 * Uso: Zaino, Sottoinsieme con somma K, Power Set
 * Pattern: Nessun ciclo for, albero sempre biforcuto
 * ------------------------------------------------------------------------------------- */
void binario(int pos, int *sol, int count, int N, int *val, int *bestSol, int *bestCount) {
    // PRUNING (es: peso superato)
    // if (peso_corrente > max_peso) return;
    
    // BASE CASE
    if (pos >= N) {
        // Verifica validità e aggiorna ottimo
        if (count > *bestCount) {
            *bestCount = count;
            memcpy(bestSol, sol, count * sizeof(int));
        }
        return;
    }
    
    // PRENDO l'elemento pos
    sol[count] = val[pos];
    binario(pos + 1, sol, count + 1, N, val, bestSol, bestCount);
    
    // LASCIO l'elemento pos
    binario(pos + 1, sol, count, N, val, bestSol, bestCount);
}

/* -------------------------------------------------------------------------------------
 * MODELLO 2: SLOT (Permutazioni/Disposizioni)
 * Uso: Anagrammi, N-Regine, TSP, Classifiche
 * Pattern: Ciclo for + array mark[] per non ripetere
 * ------------------------------------------------------------------------------------- */
void slot(int pos, int *sol, int *mark, int N, int k) {
    // BASE CASE
    if (pos >= k) {
        // Stampa o salva soluzione
        for (int i = 0; i < k; i++) printf("%d ", sol[i]);
        printf("\n");
        return;
    }
    
    // CICLO SCELTE
    for (int i = 0; i < N; i++) {
        if (mark[i] == 0) {
            // DO
            mark[i] = 1;
            sol[pos] = i;
            
            // RECURSE
            slot(pos + 1, sol, mark, N, k);
            
            // BACKTRACK
            mark[i] = 0;
        }
    }
}

/* Varianti Slot:
   - Anagrammi con duplicati: Ordina input, aggiungi dopo if(mark[i]==0):
     if (i > 0 && val[i] == val[i-1] && mark[i-1] == 0) continue;
   
   - Disposizioni con ripetizione (PIN): Togli tutto su mark[]
*/

/* -------------------------------------------------------------------------------------
 * MODELLO 3: PARTIZIONI (Palle in Scatole)
 * Uso: Squadre, Bin Packing, Raggruppamento
 * Pattern: Ciclo su scatole esistenti + 1 chiamata per nuova scatola
 * ------------------------------------------------------------------------------------- */
void partizioni(int pos, int *sol, int n_scatole, int N) {
    // BASE CASE
    if (pos >= N) {
        // Stampa partizione
        return;
    }
    
    // Provo SCATOLE ESISTENTI
    for (int i = 0; i < n_scatole; i++) {
        sol[pos] = i;
        partizioni(pos + 1, sol, n_scatole, N);
    }
    
    // Apro NUOVA SCATOLA
    sol[pos] = n_scatole;
    partizioni(pos + 1, sol, n_scatole + 1, N);
}

/* -------------------------------------------------------------------------------------
 * MODELLO 4: GRIGLIA 2D (Labirinto/Flood Fill)
 * Uso: Maze, Crucipuzzle, Conta Isole, Boggle
 * Pattern: Ciclo su 4 direzioni + matrice visited
 * ------------------------------------------------------------------------------------- */
void griglia(int r, int c, int **mat, int **visited, int R, int C) {
    // BASE CASE (es: arrivo destinazione)
    if (r == R-1 && c == C-1) {
        // Trovato!
        return;
    }
    
    // Vettori spostamento (N, E, S, O)
    int dr[] = {-1, 0, 1, 0};
    int dc[] = {0, 1, 0, -1};
    
    for (int i = 0; i < 4; i++) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        
        // CHECK VALIDITÀ
        if (nr >= 0 && nr < R && nc >= 0 && nc < C &&
            mat[nr][nc] != 1 && visited[nr][nc] == 0) {
            
            // DO
            visited[nr][nc] = 1;
            
            // RECURSE
            griglia(nr, nc, mat, visited, R, C);
            
            // BACKTRACK
            visited[nr][nc] = 0;
        }
    }
}

/* Varianti Griglia:
   - Diagonali: Aggiungi 4 direzioni ai vettori (size 8)
     int dr[] = {-1, 0, 1, 0,  -1, 1, 1, -1};
     int dc[] = {0, 1, 0, -1,   1, 1, -1, -1};
   
   - Cavallo Scacchi:
     int dr[] = {-2, -2, -1, -1, 1, 1, 2, 2};
     int dc[] = {-1, 1, -2, 2, -2, 2, -1, 1};
   
   - Flood Fill: Togli backtrack (una volta visitato resta visitato)
*/

/* -------------------------------------------------------------------------------------
 * MODELLO 5: GRAFO DFS (Visita in Profondità)
 * Uso: Cammini, Componenti Connesse, Cicli
 * Pattern: Scorre lista adiacenza + visited
 * ------------------------------------------------------------------------------------- */
void dfs(Graph G, int u, int *visited) {
    visited[u] = 1;
    
    // Scorro i vicini
    for (Glink t = G->ladj[u]; t != NULL; t = t->next) {
        int v = t->v;
        if (visited[v] == 0) {
            dfs(G, v, visited);
            // Backtrack se cerco TUTTI i cammini:
            // visited[v] = 0;
        }
    }
}


/* ======================================================================================
 * 4. PROBLEMI LISTE - PATTERN 12 PUNTI
 * ====================================================================================== */

/* -------------------------------------------------------------------------------------
 * PATTERN A: MERGE/INTERSEZIONE (Due liste ordinate)
 * Esami: Feb 2024 (merge con somma), Sett 2025 (intersezione)
 * ------------------------------------------------------------------------------------- */
link mergeLists(link h1, link h2) {
    link head = NULL, tail = NULL;
    
    while (h1 != NULL && h2 != NULL) {
        int cmp = strcmp(h1->key, h2->key); // O: h1->val - h2->val
        
        if (cmp < 0) {
            // h1 è minore
            // UNIONE: copia h1
            // INTERSEZIONE: ignora
            h1 = h1->next;
        }
        else if (cmp > 0) {
            // h2 è minore
            // UNIONE: copia h2
            // INTERSEZIONE: ignora
            h2 = h2->next;
        }
        else {
            // UGUALI - QUI SUCCEDE LA MAGIA
            // Feb 2024: Somma valori
            int somma = h1->val + h2->val;
            // insertTail(&head, &tail, h1->key, somma);
            
            h1 = h1->next;
            h2 = h2->next;
        }
    }
    
    // UNIONE: Aggiungi rimanenti
    // while (h1 != NULL) { ... }
    // while (h2 != NULL) { ... }
    
    return head;
}

/* -------------------------------------------------------------------------------------
 * PATTERN B: CANCELLAZIONE/FILTRO (Rimuovi duplicati)
 * Esami: Gen 2026 (rimuovi duplicati), Giugno 2025 (filtra lunghezza)
 * ------------------------------------------------------------------------------------- */
link removeDuplicates(link head) {
    link curr = head;
    link prev = NULL;
    link temp;
    
    while (curr != NULL) {
        // Condizione di rimozione (es: duplicato, lunghezza < K, valore pari)
        int rimuovi = 0; // Logica personalizzata
        
        if (rimuovi) {
            temp = curr;
            
            if (prev == NULL) {
                // Rimuovo testa
                head = curr->next;
                curr = head;
            } else {
                // Rimuovo nel corpo
                prev->next = curr->next;
                curr = prev->next;
            }
            
            // IMPORTANTE: Libera memoria
            if (temp->key) free(temp->key);
            free(temp);
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
    return head;
}

/* -------------------------------------------------------------------------------------
 * PATTERN C: INSERIMENTO IN CODA (Helper)
 * ------------------------------------------------------------------------------------- */
void insertTail(link *head, link *tail, int val) {
    link nuovo = malloc(sizeof(Node));
    nuovo->val = val;
    nuovo->next = NULL;
    
    if (*head == NULL) {
        *head = *tail = nuovo;
    } else {
        (*tail)->next = nuovo;
        *tail = nuovo;
    }
}


/* ======================================================================================
 * 5. PROBLEMI BST - PATTERN 12 PUNTI
 * ====================================================================================== */

/* -------------------------------------------------------------------------------------
 * PATTERN A: FOGLIA PIÙ PROFONDA + STAMPA PERCORSO
 * Esami: Luglio 2024
 * ------------------------------------------------------------------------------------- */
void findDeepest(BSTlink root, int depth, int *maxDepth, BSTlink *bestLeaf) {
    if (root == NULL) return;
    
    // È foglia?
    if (root->l == NULL && root->r == NULL) {
        if (depth > *maxDepth || 
           (depth == *maxDepth && strcmp(root->key, (*bestLeaf)->key) > 0)) {
            *maxDepth = depth;
            *bestLeaf = root;
        }
    }
    
    findDeepest(root->l, depth + 1, maxDepth, bestLeaf);
    findDeepest(root->r, depth + 1, maxDepth, bestLeaf);
}

// FOGLIA -> RADICE (Senza array)
int printUp(link h, int target) {
    if (h == NULL) return 0;
    
    // Se trovo il target o se un figlio mi dice che l'ha trovato
    if (h->val == target || printUp(h->l, target) || printUp(h->r, target)) {
        printf("%d <- ", h->val); // STAMPO DOPO (mentre risalgo)
        return 1;
    }
    return 0;
}

// RADICE -> FOGLIA (Serve un vettore)
void printDown(link h, int target, int *path, int len) {
    if (h == NULL) return;

    // 1. Mi segno nel taccuino
    path[len] = h->val;

    // 2. Controllo se sono io il target
    if (h->val == target) {
        // Stampo tutto il vettore fin qui
        for (int i = 0; i <= len; i++) printf("%d -> ", path[i]);
        printf("TROVATO\n");
        return;
    }

    // 3. Scendo portandomi dietro il taccuino aggiornato
    printDown(h->l, target, path, len + 1);
    printDown(h->r, target, path, len + 1);
}

/* -------------------------------------------------------------------------------------
 * PATTERN B: CONTA NODI CON PROPRIETÀ
 * Esami: Gen 2025 (nodi con 2 figli), Gen 2026 (range A-B)
 * ------------------------------------------------------------------------------------- */
// Conta nodi con 2 figli
int countDoubleNodes(BSTlink root) {
    if (root == NULL) return 0;
    
    int count = 0;
    if (root->l != NULL && root->r != NULL) count = 1;
    
    return count + countDoubleNodes(root->l) + countDoubleNodes(root->r);
}

// Conta nodi in range [A, B] con PRUNING
int countRange(BSTlink root, int A, int B) {
    if (root == NULL) return 0;
    
    // PRUNING: Nodo troppo piccolo -> Cerca solo a destra
    if (root->val < A) 
        return countRange(root->r, A, B);
    
    // PRUNING: Nodo troppo grande -> Cerca solo a sinistra
    if (root->val > B) 
        return countRange(root->l, A, B);
    
    // Nodo nell'intervallo -> Conta 1 + entrambi i lati
    return 1 + countRange(root->l, A, B) + countRange(root->r, A, B);
}

/* -------------------------------------------------------------------------------------
 * PATTERN C: VERIFICA PROPRIETÀ BST
 * Esami: Maggio 2025
 * ------------------------------------------------------------------------------------- */
int checkBST(BSTlink h, int min, int max) {
    if (h == NULL) return 1;
    
    // Violazione
    if (h->val <= min || h->val >= max) return 0;
    
    // Controllo ricorsivo restringendo range
    return checkBST(h->l, min, h->val) && 
           checkBST(h->r, h->val, max);
}

/* -------------------------------------------------------------------------------------
 * PATTERN D: LINEARIZZAZIONE (BST -> Array)
 * Esami: Feb 2024
 * ------------------------------------------------------------------------------------- */
void bstToArray(BSTlink root, Node *arr, int *index) {
    if (root == NULL) return;
    
    // In-Order: SX, ROOT, DX (array ordinato)
    bstToArray(root->l, arr, index);
    
    arr[*index].val = root->val;
    arr[*index].key = strdup(root->key);
    (*index)++;
    
    bstToArray(root->r, arr, index);
}


/* ======================================================================================
 * 6. PROBLEMI VETTORI/STRINGHE - PATTERN 12 PUNTI
 * ====================================================================================== */

/* -------------------------------------------------------------------------------------
 * PATTERN A: SOTTOSEQUENZA
 * Esami: Gen 2025, Gen 2026
 * ------------------------------------------------------------------------------------- */
// A è sottosequenza di B?
int isSubSequence(int *A, int nA, int *B, int nB) {
    int i = 0, j = 0;
    
    while (i < nA && j < nB) {
        if (A[i] == B[j]) {
            i++; // Match, avanzo target
        }
        j++; // Avanzo sempre sorgente
    }
    
    return (i == nA); // True se ho trovato tutti
}

/* -------------------------------------------------------------------------------------
 * PATTERN B: DIFFERENZA VETTORI ORDINATI
 * Esami: Sett 2024
 * ------------------------------------------------------------------------------------- */
int *diffVett(int *a, int na, int *b, int nb, int *nc) {
    int *c = malloc(na * sizeof(int));
    *nc = 0;
    int i = 0, j = 0;
    
    while (i < na) {
        if (j >= nb || a[i] < b[j]) {
            // a[i] non è in b
            c[(*nc)++] = a[i];
            i++;
        }
        else if (a[i] == b[j]) {
            // Skip (è in entrambi)
            i++; j++;
        }
        else {
            // a[i] > b[j]
            j++;
        }
    }
    
    return realloc(c, (*nc) * sizeof(int));
}

/* -------------------------------------------------------------------------------------
 * PATTERN C: RIMOZIONE CARATTERI (Backtracking)
 * Esami: Gen 2026
 * ------------------------------------------------------------------------------------- */
int strContains(char *s1, char *s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    
    if (len1 < len2) return 0;
    
    // Backtracking: Prova combinazioni di rimozione
    // (Codice semplificato - versione completa richiede ricorsione)
    
    return 0; // Placeholder
}


/* ======================================================================================
 * 7. PROBLEMI GRAFI - PATTERN 18 PUNTI
 * ====================================================================================== */

/* -------------------------------------------------------------------------------------
 * PATTERN A: CAMMINO HAMILTONIANO (Verifica)
 * Esami: Maggio 2025
 * ------------------------------------------------------------------------------------- */
int checkHamiltonian(Graph G, int *path, int n) {
    if (n != G->V) return 0; // Deve contenere tutti i vertici (né più, né meno)
    
    int *visited = calloc(G->V, sizeof(int));
    if (visited == NULL) return 0; // Controllo allocazione (buona norma)

    for (int i = 0; i < n - 1; i++) {
        int u = path[i];
        int v = path[i + 1];
        
        // 1. Verifica esistenza arco u -> v
        int found = 0;
        for (Glink t = G->ladj[u]; t != NULL; t = t->next) {
            if (t->v == v) {
                found = 1;
                break;
            }
        }
        if (!found) {
            free(visited);
            return 0; // Arco mancante
        }
        
        // 2. Verifica duplicati per il vertice corrente 'u'
        if (visited[u]) {
            free(visited);
            return 0; // Vertice ripetuto
        }
        visited[u] = 1;
    }
    
    // --- FIX: CONTROLLO DELL'ULTIMO VERTICE ---
    // L'ultimo vertice (path[n-1]) non è mai stato 'u', quindi dobbiamo
    // controllare ora se era già presente nel cammino.
    if (visited[path[n-1]]) {
        free(visited);
        return 0; // L'ultimo vertice è un duplicato di uno precedente
    }
    // ------------------------------------------

    free(visited);
    return 1;
}

/* -------------------------------------------------------------------------------------
 * PATTERN B: ALBERO GENEALOGICO (DAG)
 * Esami: Gen 2026
 * ------------------------------------------------------------------------------------- */
// Trova parenti di grado <= k
typedef struct {
    char nome[100];
    int giorno, mese, anno;
} Persona;

int relationshipK(Graph G, char *name1, int d1, char *name2, int d2, int k) {
    // 1. Trova indici delle persone nella tabella simboli
    // 2. DFS da p1 verso p2 con limite profondità k
    // 3. Se non trovato, cerca antenato comune
    
    return 0; // Placeholder
}


/* ======================================================================================
 * 8. ADT E FILE I/O
 * ====================================================================================== */

/* -------------------------------------------------------------------------------------
 * PATTERN A: CARICAMENTO DA FILE
 * ------------------------------------------------------------------------------------- */
typedef struct {
    char nome[50];
    int valore;
} Item;

typedef struct {
    Item *vettore;
    int n;
} Tabella_struct;
typedef Tabella_struct *Tabella;

Tabella loadData(char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) return NULL;
    
    Tabella t = malloc(sizeof(Tabella_struct));
    
    // Pattern: Leggi N, poi cicla N volte
    fscanf(f, "%d", &t->n);
    t->vettore = malloc(t->n * sizeof(Item));
    
    for (int i = 0; i < t->n; i++) {
        fscanf(f, "%s %d", t->vettore[i].nome, &t->vettore[i].valore);
    }
    
    fclose(f);
    return t;
}

/* -------------------------------------------------------------------------------------
 * PATTERN B: DEEP COPY (Strutture Complesse)
 * Esami: Feb 2025
 * ------------------------------------------------------------------------------------- */
Tabella deepCopy(Tabella orig) {
    if (orig == NULL) return NULL;
    
    Tabella copia = malloc(sizeof(Tabella_struct));
    copia->n = orig->n;
    copia->vettore = malloc(copia->n * sizeof(Item));
    
    for (int i = 0; i < copia->n; i++) {
        strcpy(copia->vettore[i].nome, orig->vettore[i].nome);
        copia->vettore[i].valore = orig->vettore[i].valore;
    }
    
    return copia;
}


/* ======================================================================================
 * 9. PATTERN ESAMI PASSATI - CHECKLIST VELOCE
 * ====================================================================================== */

/*
LISTE (12pt):
✓ Merge/Intersezione liste ordinate → while(h1 && h2), confronto chiavi
✓ Rimuovi duplicati → pattern prev/curr, gestisci testa separatamente
✓ Filtra per proprietà → stessa logica rimozione

BST (12pt):
✓ Foglia più profonda → DFS con depth, confronta a foglie
✓ Conta nodi con proprietà → DFS, somma 1 se condizione vera
✓ Range [A,B] → PRUNING: se val < A vai a destra, se val > B vai a sinistra
✓ Verifica BST → Passa min/max, restringi range ricorsivamente
✓ Linearizzazione → In-Order (SX, ROOT, DX)
✓ Stampa percorso → Ritorna 1 se trovato, stampa durante risalita

VETTORI (12pt):
✓ Sottosequenza → Due puntatori i,j
✓ Differenza ordinati → Merge-like, prendi se non in secondo
✓ Backtracking stringhe → Combinazioni di rimozione caratteri

GRAFI (18pt):
✓ Cammino Hamiltoniano → Verifica archi + no duplicati
✓ DAG parenti → DFS con limite profondità + antenato comune
✓ Crucipuzzle → Griglia 2D, 3 direzioni (O, V, D)

BACKTRACKING (18pt):
✓ Wrapper → Alloca sol, bestSol, mark, chiama ricorsiva
✓ Pruning → Controlla PRIMA di scendere (peso, costo)
✓ Check terminale → Verifica validità soluzione completa
✓ Modello giusto:
  - Prendo/Lascio? → Binario
  - Ordine conta? → Slot (con mark)
  - Raggruppo? → Partizioni
  - Griglia? → 4 direzioni
  - Grafo? → DFS su liste adiacenza
*/


/* ======================================================================================
 * 10. WRAPPER TIPO 18 PUNTI
 * ====================================================================================== */

int solveWrapper(int N, int *val) {
    // 1. Alloca soluzioni
    int *sol = malloc(N * sizeof(int));
    int *bestSol = malloc(N * sizeof(int));
    int *mark = calloc(N, sizeof(int));
    
    int bestMetric = -1; // O +infinito se minimizzo
    
    // 2. Chiama ricorsiva
    solve_recursive(0, N, val, sol, bestSol, mark, &bestMetric);
    
    // 3. Usa bestSol...
    
    // 4. Libera memoria
    free(sol);
    free(bestSol);
    free(mark);
    
    return bestMetric;
}

void solve_recursive(int pos, int N, int *val, int *sol, int *bestSol, 
                     int *mark, int *bestMetric) {
    
    // A. PRUNING
    // if (costo_parziale > *bestMetric) return;
    
    // B. TERMINAZIONE
    if (pos >= N) {
        int metric = calcola(sol, N);
        if (metric > *bestMetric) {
            *bestMetric = metric;
            memcpy(bestSol, sol, N * sizeof(int));
        }
        return;
    }
    
    // C. CICLO SCELTE
    for (int i = 0; i < N; i++) {
        if (mark[i] == 0) {
            // DO
            mark[i] = 1;
            sol[pos] = val[i];
            
            // RECURSE
            solve_recursive(pos + 1, N, val, sol, bestSol, mark, bestMetric);
            
            // BACKTRACK
            mark[i] = 0;
        }
    }
    
    // D. (Opzionale) Caso "non prendo"
    // solve_recursive(pos + 1, N, val, sol, bestSol, mark, bestMetric);
}


/* ======================================================================================
 * FINE GUIDA - BUONO STUDIO!
 * ====================================================================================== */