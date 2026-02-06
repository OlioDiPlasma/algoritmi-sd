/* * ======================================================================================
 * MASTER CHEATSHEET SINTASSI C - ALGORITMI E STRUTTURE DATI
 * ======================================================================================
 * Guida rapida per: Puntatori, Stringhe, Matrici, Liste, BST, Grafi, File.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Per tolower, toupper, isalpha...

// ======================================================================================
// 1. GERARCHIA DEI PUNTATORI (Il "Terrore" degli asterischi)
// ======================================================================================

void gerarchia_puntatori() {
    
    // --- LIVELLO 0: Variabile ---
    int val = 10;           
    // "Ho un numero."

    // --- LIVELLO 1: Puntatore (*) ---
    int *p1 = &val;         
    // "Ho un indirizzo che porta a un numero."
    // Uso: *p1 = 20; (Modifico val)
    // Utile per: vettori dinamici 1D, passare valori per riferimento.

    // --- LIVELLO 2: Puntatore a Puntatore (**) ---
    int **p2 = &p1;         
    // "Ho l'indirizzo di un puntatore."
    // Uso: Matrici dinamiche 2D, Array di stringhe, modificare un puntatore in una funzione (es. head di lista).
    
    // --- LIVELLO 3: Triplo Puntatore (***) ---
    int ***p3 = &p2;        
    // "Ho l'indirizzo di un puntatore a puntatore."
    // Uso raro: Matrici 3D (Cubo), oppure passare una matrice dinamica (int**) per riferimento a una funzione.
    
    // ESEMPIO DI ALLOCAZIONE MATRICE 3D (CUBO) [R][C][Z]
    int R=5, C=5, Z=5;
    int ***cubo = (int ***)malloc(R * sizeof(int **));
    for(int i=0; i<R; i++) {
        cubo[i] = (int **)malloc(C * sizeof(int *));
        for(int j=0; j<C; j++) {
            cubo[i][j] = (int *)malloc(Z * sizeof(int));
            cubo[i][j][0] = 99; // Accesso
        }
    }
}

// ======================================================================================
// 2. STRINGHE E CARATTERI (Char *, Array, Funzioni)
// ======================================================================================

void gestione_stringhe() {
    
    // --- DICHIARAZIONE ---
    char s_stack[] = "Modificabile";  // Array sullo stack. Posso fare s_stack[0] = 'm';
    char *s_ro     = "Sola Lettura";  // String literal. NON fare s_ro[0] = 'X'; (Crash!)
    char *s_heap   = strdup("Copia"); // Allocata con malloc internamente. Va liberata con free().

    // --- CONFRONTO (MAI USARE ==) ---
    char *a = "Alpha";
    char *b = "Beta";
    
    if (strcmp(a, b) == 0) { /* Sono uguali */ }
    if (strcmp(a, b) < 0)  { /* 'a' viene PRIMA di 'b' nel dizionario */ }
    if (strcmp(a, b) > 0)  { /* 'a' viene DOPO 'b' */ }

    // --- COPIA ---
    char dest[100]; // Assicurarsi che ci sia spazio!
    strcpy(dest, a); // Copia incluso il terminatore \0
    
    // --- CONCATENAZIONE ---
    strcat(dest, b); // Attacca b in fondo a dest -> "AlphaBeta"

    // --- LUNGHEZZA ---
    int len = strlen(a); // Conta caratteri ESCLUSO \0

    // --- SINGOLI CARATTERI (ctype.h) ---
    char c = 'A';
    if (isalpha(c)) {} // È una lettera?
    if (isdigit(c)) {} // È un numero?
    c = tolower(c);    // Diventa 'a'
}

// ======================================================================================
// 3. ARRAY DI STRINGHE (Char **)
// ======================================================================================

void array_di_stringhe() {
    // Esempio: Caricare un dizionario di N parole
    int N = 10;
    
    // 1. Alloco l'array di puntatori (le "righe" del foglio)
    char **dizionario = (char **)malloc(N * sizeof(char *));

    // 2. Alloco ogni singola parola
    for (int i = 0; i < N; i++) {
        // Opzione A: So quanto è lunga la parola (es. buffer di lettura)
        char buffer[100]; 
        // ...fscanf legge in buffer...
        dizionario[i] = strdup(buffer); // Malloc automatica della dimensione giusta
        
        // Opzione B: Allocazione manuale
        // dizionario[i] = (char *)malloc(50 * sizeof(char));
    }

    // Accesso:
    // dizionario[0]     -> è la prima parola (char *)
    // dizionario[0][2]  -> è il 3° carattere della prima parola (char)
}

// ======================================================================================
// 4. STRUCT: LISTE E ALBERI (BST)
// ======================================================================================

// --- LISTA ---
typedef struct node {
    int key;            // Dato
    char *nome;         // Dato stringa
    struct node *next;  // Link
} Node, *link;

// --- ALBERO BINARIO (BST) ---
typedef struct bst_node {
    int item;
    struct bst_node *l; // left
    struct bst_node *r; // right
} BSTnode, *BSTlink;

// --- WRAPPER PER BST (Oggetto di prima classe) ---
typedef struct binary_search_tree {
    BSTlink root;
    int z; // Elemento sentinella (opzionale, spesso NULL)
} *BST;

void allocazione_nodi() {
    // Nuovo nodo lista
    link n = (link)malloc(sizeof(Node));
    n->key = 1;
    n->nome = strdup("Pippo"); // Ricorda strdup per stringhe nelle struct!
    n->next = NULL;

    // Nuovo nodo albero
    BSTlink t = (BSTlink)malloc(sizeof(BSTnode));
    t->item = 10;
    t->l = NULL;
    t->r = NULL;
}

// ======================================================================================
// 5. GRAFI (Graph ADT) - I due modi principali
// ======================================================================================

// TIPO A: MATRICE DI ADIACENZA (Denso, veloce vedere se c'è arco)
typedef struct graph_mat {
    int V;       // Numero vertici
    int E;       // Numero archi
    int **madj;  // Matrice quadrata V*V
} *GraphMat;

void init_grafo_matrice() {
    int V = 5;
    GraphMat G = (GraphMat)malloc(sizeof(struct graph_mat));
    G->V = V;
    
    // Alloco matrice V x V
    G->madj = (int **)malloc(V * sizeof(int *));
    for (int i=0; i<V; i++) {
        G->madj[i] = (int *)calloc(V, sizeof(int)); // calloc mette a 0 (nessun arco)
    }
    // Inserisco arco da 0 a 1
    G->madj[0][1] = 1; 
}

// TIPO B: LISTE DI ADIACENZA (Sparso, efficiente per memoria)
// Serve definire un nodo per la lista (come le liste concatenate semplici)
typedef struct gnode {
    int v;              // Indice del vertice destinazione
    int weight;         // Peso (opzionale)
    struct gnode *next; // Prossimo vicino
} GNode, *Glink;

typedef struct graph_list {
    int V;
    int E;
    Glink *ladj; // VETTORE di puntatori a liste (link *)
} *GraphList;

void init_grafo_liste() {
    int V = 5;
    GraphList G = (GraphList)malloc(sizeof(struct graph_list));
    G->V = V;

    // Alloco il VETTORE principale (colonna vertebrale del grafo)
    // È un array di puntatori, ogni cella è la testa di una lista
    G->ladj = (Glink *)calloc(V, sizeof(Glink)); // Tutto a NULL inizialmente

    // Aggiungere arco 0 -> 1 (Inserzione in testa classica)
    Glink nuovo = (Glink)malloc(sizeof(GNode));
    nuovo->v = 1;
    nuovo->next = G->ladj[0];
    G->ladj[0] = nuovo;
}

// ======================================================================================
// 6. I/O DA FILE (fscanf, fprintf)
// ======================================================================================

void lettura_file() {
    FILE *fp;
    char nome[100];
    int eta;

    // APERTURA
    fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("Errore apertura file\n");
        exit(1);
    }

    // LETTURA: Supponiamo file fatto da: "Mario 30\nLuigi 25..."
    // fscanf ritorna il numero di campi letti correttamente
    while (fscanf(fp, "%s %d", nome, &eta) == 2) {
        printf("Letto: %s ha %d anni\n", nome, eta);
        
        // Se devo salvare la stringa, devo usare strdup!
        // char *nome_salvato = strdup(nome);
    }

    fclose(fp);
}

// ======================================================================================
// 7. TRAPPOLE COMUNI (Per non farti bocciare)
// ======================================================================================

void trappole() {
    // 1. FREE e PUNTATORI
    int *x = malloc(sizeof(int));
    free(x);
    // x ora punta a spazzatura. Se lo riusi crasha.
    // x = NULL; // Buona norma
    
    // 2. DIMENSIONI VETTORI
    int v[10];
    // v[10] = 5; // ERRORE! Gli indici vanno da 0 a 9.
    
    // 3. RETURN DI VARIABILI LOCALI
    /*
    int* sbaglio() {
        int a = 5;
        return &a; // NO! 'a' muore quando la funzione finisce.
    }
    */
   
    // 4. STRUCT E FRECCE
    Node n;
    link p = &n;
    
    n.key = 5;    // Variabile diretta -> uso PUNTO
    p->key = 5;   // Puntatore -> uso FRECCIA
    (*p).key = 5; // Equivalente alla freccia (ma brutto)
}