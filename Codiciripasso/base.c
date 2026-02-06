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



// ======================================================================================
// 1. QSORT (L'Arma Segreta)
// ======================================================================================
// Se l'esame dice "è consentito utilizzare chiamate a funzioni standard quali ordinamento"
// NON SCRIVERE IL BUBBLE SORT A MANO! Usa questo.

// Funzione di confronto per INTERI (Crescente)
int cmp_int(const void *a, const void *b) {
    // Cast a puntatore intero e dereferenziazione
    int _a = *(int *)a;
    int _b = *(int *)b;
    return _a - _b; // Se negativo a < b, se 0 a == b, se positivo a > b
}

// Funzione di confronto per STRINGHE (Crescente)
int cmp_str(const void *a, const void *b) {
    // Qui a e b sono puntatori a puntatori a char (char **)
    char *_a = *(char **)a;
    char *_b = *(char **)b;
    return strcmp(_a, _b);
}

void esempio_ordinamento() {
    int numeri[] = {10, 5, 8, 1, 99};
    // Sintassi: array, numero elementi, dimensione singolo elemento, funzione compare
    qsort(numeri, 5, sizeof(int), cmp_int);

    char *nomi[] = {"Zorro", "Anna", "Luca"};
    qsort(nomi, 3, sizeof(char *), cmp_str);
}

// ======================================================================================
// 2. MODELLI DI VISITA ALBERI (Impara a memoria!)
// ======================================================================================
// Se ti chiedono di visitare un albero, usa uno di questi tre schemi.
// Non inventare cicli strani.

typedef struct tnode {
    int key;
    struct tnode *l, *r;
} *link;

// A. PRE-ORDER (Radice -> Sinistra -> Destra)
// Utile per: copiare alberi, stampare struttura
void pre_order(link h) {
    if (h == NULL) return;
    
    printf("%d ", h->key); // Faccio cose con la radice
    pre_order(h->l);
    pre_order(h->r);
}

// B. IN-ORDER (Sinistra -> Radice -> Destra)
// FONDAMENTALE PER I BST: Li visita in ordine crescente!
void in_order(link h) {
    if (h == NULL) return;
    
    in_order(h->l);
    printf("%d ", h->key); // Se è un BST, stampa i numeri ordinati
    in_order(h->r);
}

// C. POST-ORDER (Sinistra -> Destra -> Radice)
// Utile per: CANCELLARE l'albero (liberi i figli prima del padre) o calcoli dal basso (es. altezza)
void post_order(link h) {
    if (h == NULL) return;
    
    post_order(h->l);
    post_order(h->r);
    printf("%d ", h->key); // Elaboro la radice per ultima
}

// ======================================================================================
// 3. CONVERSIONI E RICERCA STRINGHE (Utility)
// ======================================================================================

void utility_stringhe() {
    char *frase = "Il codice 12345 è segreto";
    
    // -- TROVARE UNA SOTTOSTRINGA (strstr) --
    char *trovato = strstr(frase, "codice"); 
    // Ritorna puntatore alla prima occorrenza o NULL
    if (trovato != NULL) {
        printf("Trovato!");
    }

    // -- TROVARE UN CARATTERE (strchr) --
    char *c = strchr(frase, 'z'); // Cerca 'z'
    if (c == NULL) printf("Non c'è la z");

    // -- STRINGA -> NUMERO (atoi, atol, atof) --
    char *num_str = "42";
    int n = atoi(num_str); // ASCII to Integer (ritorna 42)
    
    // -- NUMERO -> STRINGA (sprintf) --
    char buffer[50];
    int valore = 1990;
    sprintf(buffer, "Anno: %d", valore); // Stampa DENTRO la stringa buffer
}

// ======================================================================================
// 4. MATEMATICA BASE (Senza impazzire)
// ======================================================================================

void matematica() {
    int x = -5;
    int abs_x = abs(x); // Valore assoluto (5)
    
    // Ricorda di includere <math.h> e linkare con -lm se usi gcc da terminale
    // float p = pow(2, 3); // 2 alla 3a = 8.0
    // float s = sqrt(9);   // Radice quadrata = 3.0
}

// ======================================================================================
// 5. SWITCH-CASE (Alternativa ordinata agli if-else infiniti)
// ======================================================================================

void menu(char scelta) {
    switch (scelta) {
        case 'a':
        case 'A':
            // Fa questo per 'a' O 'A'
            printf("Azione A");
            break; // FONDAMENTALE: se lo scordi esegue anche i casi sotto!
        
        case 'b':
            printf("Azione B");
            break;
            
        default:
            printf("Scelta non valida");
            break;
    }
}

// ======================================================================================
// 6. TYPEDEF COMPLESSI (Visti nei testi d'esame)
// ======================================================================================

// Puntatore a funzione (usato raramente ma se esce fa paura)
// "fun" è un tipo: puntatore a funzione che prende due int e ritorna int
typedef int (*fun_ptr)(int, int);

// Esempio d'uso
int somma(int a, int b) { return a + b; }

void usa_funzione() {
    fun_ptr f = somma;
    int ris = f(2, 3); // Chiama somma(2,3) -> 5
}

// ======================================================================================
// 1. BOOLEANI E MACRO (Se non puoi usare <stdbool.h>)
// ======================================================================================
// Spesso nei vecchi compiti si definisce così:
typedef int Boolean;
#define TRUE 1
#define FALSE 0

// Oppure macro per massimo e minimo (utilissime)
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

// ======================================================================================
// 2. MODIFICARE PUNTATORI DENTRO FUNZIONI (Il Triplo Salto Mortale)
// ======================================================================================
// PROBLEMA: Se passi "head" a una funzione e la modifichi, fuori non cambia.
// SOLUZIONE: Devi passare l'indirizzo di head (&head), quindi ricevi un doppio puntatore.

typedef struct node { int val; struct node *next; } Node, *link;

// SBAGLIATO (La lista rimarrà vuota nel main)
void insert_sbagliata(link h, int v) {
    h = malloc(sizeof(Node)); // Modifico solo la copia locale di h!
    h->val = v;
}

// GIUSTO (Uso link *hptr, quindi Node **hptr)
void insert_in_testa(link *hptr, int v) {
    // *hptr è il vero puntatore alla testa
    link nuovo = malloc(sizeof(Node));
    nuovo->val = v;
    nuovo->next = *hptr; // Il nuovo punta alla vecchia testa
    *hptr = nuovo;       // La testa diventa il nuovo
}

// Esempio di chiamata nel main:
// link head = NULL;
// insert_in_testa(&head, 10); // NOTA LA &

// ======================================================================================
// 3. ALGORITMI LISTE: INSERIMENTO IN CODA (Senza puntatore tail)
// ======================================================================================
// Richiede di scorrere tutto. Se sbagli il while, crasha.

link insert_in_coda(link h, int v) {
    link nuovo = malloc(sizeof(Node));
    nuovo->val = v;
    nuovo->next = NULL;

    if (h == NULL) {
        return nuovo; // Se era vuota, la nuova testa è lui
    }

    link x = h;
    // SCORRO FINO ALL'ULTIMO (non fino a NULL!)
    while (x->next != NULL) {
        x = x->next;
    }
    // Ora x è l'ultimo nodo
    x->next = nuovo;
    return h; // Ritorno sempre la testa originale
}

// ======================================================================================
// 4. ALGORITMI LISTE: CANCELLAZIONE (Il pattern prev/curr)
// ======================================================================================
// Questo è lo snippet da imparare a memoria per rimuovere nodi.

link cancella_nodo(link h, int valore_da_togliere) {
    link curr = h;
    link prev = NULL;

    while (curr != NULL) {
        if (curr->val == valore_da_togliere) {
            // HO TROVATO IL NODO
            if (prev == NULL) {
                // Caso speciale: sto togliendo la testa
                h = curr->next;
            } else {
                // Caso normale: salto il nodo
                prev->next = curr->next;
            }
            free(curr); // Libero memoria
            return h;   // Esco subito (se devo cancellarne solo uno)
        }
        // AVANZO
        prev = curr;
        curr = curr->next;
    }
    return h;
}

// ======================================================================================
// 5. NAVIGAZIONE GRIGLIE (Matrici come mappe)
// ======================================================================================
// Tipico esercizio: "Trova il cammino nel labirinto" o "conta le isole".
// Devi guardare Nord, Sud, Est, Ovest senza uscire dalla matrice.

void naviga_griglia(int **mat, int R, int C, int r, int c) {
    // Vettori di spostamento (Nord, Sud, Ovest, Est)
    int dr[] = {-1, 1, 0, 0}; // delta righe
    int dc[] = {0, 0, -1, 1}; // delta colonne

    for (int i = 0; i < 4; i++) {
        int nr = r + dr[i]; // nuova riga
        int nc = c + dc[i]; // nuova colonna

        // CONTROLLO BORDI (Fondamentale!)
        if (nr >= 0 && nr < R && nc >= 0 && nc < C) {
            // Se sono qui, la cella mat[nr][nc] esiste ed è valida
            if (mat[nr][nc] == 1) { // Esempio: se c'è un cammino
                // Fai qualcosa (es. ricorsione)
            }
        }
    }
}

// ======================================================================================
// 6. INPUT SICURO (Stringhe con spazi)
// ======================================================================================
// fscanf si ferma allo spazio. gets è vietata. Usa fgets.

void leggi_riga_intera() {
    char buffer[100];
    
    // Legge fino a 99 caratteri o a capo (\n), da standard input (stdin)
    if (fgets(buffer, 100, stdin) != NULL) {
        // ATTENZIONE: fgets include il '\n' finale se c'è spazio.
        // Trucco per toglierlo:
        buffer[strcspn(buffer, "\n")] = 0; 
    }
}

// ======================================================================================
// 7. HASH TABLE (Stub basilare)
// ======================================================================================
// Se capita una hash table a liste di trabocco (chaining).

typedef struct hnode {
    char *key;
    int val;
    struct hnode *next;
} HNode;

typedef struct hash_tbl {
    int size;      // Dimensione tabella (es. M)
    HNode **heads; // Array di puntatori alle teste delle liste
} *HashTable;

HashTable init_hash(int M) {
    HashTable t = malloc(sizeof(struct hash_tbl));
    t->size = M;
    t->heads = calloc(M, sizeof(HNode*)); // Calloc mette tutto a NULL
    return t;
}

// Funzione di hash classica (metodo di Horner per stringhe)
int hash_p(char *v, int M) {
    int h = 0, base = 127;
    for (; *v != '\0'; v++)
        h = (base * h + *v) % M;
    return h;
}

// ======================================================================================
// 8. PATTERN WRAPPER RICORSIVO (Per non impazzire con i parametri)
// ======================================================================================

// Funzione "Braccio" (quella che fa il lavoro sporco)
void recursive_worker(link h, int *count, int k) {
    if (h == NULL) return;
    
    // Logica...
    if (h->val == k) (*count)++; // Uso puntatore per aggiornare contatore globale
    
    recursive_worker(h->next, count, k);
}

// Funzione "Mente" (quella chiamata dal main)
int conta_occorrenze(link head, int k) {
    int count = 0;
    recursive_worker(head, &count, k);
    return count;
}