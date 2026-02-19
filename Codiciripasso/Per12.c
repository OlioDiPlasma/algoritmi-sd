#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 /* ---------------------------------------------------------------------------------
 * SEZIONE 00: Mini calcolo combinatorio
 * --------------------------------------------------------------------------------- */


 // PERMUTAZIONI (es. Anagrammi, Cammino Hamiltoniano) L'ordine conta? SÌ. Posso ripetere? NO
void permuta(int pos, int n, int *sol, int *mark, ... ) {
    if (pos >= n) { /* Soluzione trovata */ return; }

    for (int i = 0; i < n; i++) {
        if (mark[i] == 0) { // NON l'ho ancora usato
            mark[i] = 1;
            sol[pos] = i; // Prendo l'elemento i
            permuta(pos + 1, n, sol, mark, ...);
            mark[i] = 0; // Backtrack
        }
    }
}



// DISPOSIZIONI (es. Numeri in Base B) L'ordine conta? SÌ. Posso ripetere? SÌ
void disposizioni(int pos, int cifreDaRiempire, int Base, int *sol) {
    if (pos >= cifreDaRiempire) { /* Stampo numero */ return; }

    for (int i = 0; i < Base; i++) {
        // Nessun check su mark! Posso riusare 'i' quante volte voglio
        sol[pos] = i;
        disposizioni(pos + 1, cifreDaRiempire, Base, sol);
    }
}

// COMBINAZIONI (es. Sottoinsiemi, Subset Sum) L'ordine conta? NO. Posso ripetere? NO
// start: indice da cui iniziare a pescare nel vettore originale
void combinazioni(int pos, int start, int n, int *sol, int k) {
    if (pos >= k) { /* Ho scelto k elementi */ return; }

    for (int i = start; i < n; i++) {
        sol[pos] = i;
        // Passo i + 1 alla ricorsione per prendere solo i successivi
        combinazioni(pos + 1, i + 1, n, sol, k);
    }
}


// PARTIZIONI (es. Minibus, Assegnamento) Devo dividere TUTTI gli oggetti in GRUPPI?
// pos: indice del passeggero corrente
void partizioni(int pos, int n_passeggeri, int k_bus, int *assegnamenti) {
    if (pos >= n_passeggeri) { /* Tutti assegnati */ return; }

    for (int bus = 0; bus < k_bus; bus++) {
        assegnamenti[pos] = bus; // Metto passeggero 'pos' sul 'bus'
        partizioni(pos + 1, n_passeggeri, k_bus, assegnamenti);
    }
}

/* ---------------------------------------------------------------------------------
 * SEZIONE 0: BST, NODO WRAPPER
 * --------------------------------------------------------------------------------- */

// 1. PRIMA definisci la struttura del nodo
struct node {
    Item item;          
    struct node *l;     // Figlio sinistro
    struct node *r;     // Figlio destro
};

// 2. POI crei l'alias per il puntatore al nodo (comodissimo)
typedef struct node *link;

// 3. INFINE definisci la struttura dell'Albero (il Wrapper)
struct binary_search_tree {
    link root;          // La radice
    int size;           // (Facoltativo) Numero di nodi
};

// 4. E l'alias per il puntatore all'Albero
typedef struct binary_search_tree *BST;


/* =================================================================================
 * MASTER FILE - PREPARAZIONE 12 PUNTI (2024-2025)
 * =================================================================================
 * Questo file contiene gli scheletri per:
 * 1. LISTE: Merge, Intersezione, Cancellazione (Filtro).
 * 2. BST: Visite, Verifica proprietà, Cammini, Linearizzazione.
 * 3. VETTORI/STRINGHE: Sottosequenza, Differenza insiemistica.
 * 4. MEMORIA DINAMICA: Deep Copy di ADT complessi.
 * ================================================================================= */

// Struttura generica per LISTE e BST (adatta i campi in base all'esame!)
typedef struct node *link;
struct node {
    int val;            // Spesso c'è un intero
    char *key;          // Spesso c'è una stringa (allocata dinamicamente o statica)
    link next;          // Per le LISTE
    link left, right;   // Per i BST
};

/* ---------------------------------------------------------------------------------
 * SEZIONE 1: LISTE CONCATENATE
 * Uscito in: Feb 2024 (Merge somma), Giugno 2025 (Cancellazione), Sett 2025 (Intersezione)
 * --------------------------------------------------------------------------------- */

// PATTERN A: MERGE / INTERSEZIONE DI DUE LISTE ORDINATE
// Logica: Scorro h1 e h2 contemporaneamente.
// Se chiedono "UNIONE con SOMMA" (Feb 2024): Sommo se uguali, prendi il minore se diversi.
// Se chiedono "INTERSEZIONE" (Sett 2025): Prendo solo se uguali.
link mergeLists(link h1, link h2) {
    link head = NULL, tail = NULL; // Lista risultato
    
    while (h1 != NULL && h2 != NULL) {
        int cmp = strcmp(h1->key, h2->key); // O confronto tra interi: h1->val - h2->val

        if (cmp < 0) {
            // h1 < h2. 
            // Se è UNIONE: copio h1 e avanzo h1.
            // Se è INTERSEZIONE: ignoro h1 e avanzo h1.
            // Esempio UNIONE:
            // insertTail(&head, &tail, h1->key, h1->val); 
            h1 = h1->next;
        } 
        else if (cmp > 0) {
            // h2 < h1. Simmetrico a sopra.
            // insertTail(&head, &tail, h2->key, h2->val);
            h2 = h2->next;
        } 
        else {
            // CASO FONDAMENTALE: CHIAVI UGUALI (cmp == 0)
            // Feb 2024 chiedeva: somma i valori.
            int somma = h1->val + h2->val;
            
            // Inserisci nella nuova lista
            // insertTail(&head, &tail, h1->key, somma);
            
            // Avanza ENTRAMBI
            h1 = h1->next;
            h2 = h2->next;
        }
    }
    // Code rimanenti (Solo per UNIONE/MERGE, non per Intersezione)
    // while (h1 != NULL) { ... copia h1 ... h1 = h1->next; }
    // while (h2 != NULL) { ... copia h2 ... h2 = h2->next; }
    
    return head;
}

// PATTERN B: FILTRAGGIO / CANCELLAZIONE (Giugno 2025)
// Logica: "Rimuovi nodi che soddisfano una condizione (es. lunghezza stringa)"
// TRUCCO: Usare un puntatore a puntatore (link *headRef) o gestire il caso testa a parte.
link filterList(link head, int soglia) {
    link curr = head;
    link prev = NULL;
    link temp;

    while (curr != NULL) {
        // Condizione esame: es. "strlen(curr->key) < soglia" o "curr->val dispari"
        if (curr->val < soglia) {
            temp = curr; // Salva per deallocare
            
            if (prev == NULL) {
                // Sto cancellando la TESTA
                head = curr->next;
                curr = head;
            } else {
                // Sto cancellando nel CORPO
                prev->next = curr->next;
                curr = prev->next;
            }
            
            // IMPORTANTE: Se c'è memoria dinamica nel nodo (es. stringa), liberala prima!
            // free(temp->key); 
            free(temp);
        } else {
            // Avanzo solo se non ho cancellato
            prev = curr;
            curr = curr->next;
        }
    }
    return head;
}

/* ---------------------------------------------------------------------------------
 * SEZIONE 2: ALBERI BINARI DI RICERCA (BST)
 * Uscito in: Luglio 2024 (Foglia profonda), Gen 2025 (Max nodi doppi), Feb 2024 (Linearizza)
 * --------------------------------------------------------------------------------- */

// PATTERN A: VISITA RICORSIVA CON AGGIORNAMENTO DI UN MASSIMO/CONTATORE
// Risolve: "Trova foglia più profonda", "Conta nodi con 2 figli", "Verifica bilanciamento"
void bst_dfs(link root, int depth, int *maxDepth, int *countProprieta) {
    if (root == NULL) return;

    // Esempio Logica: Conta nodi con 2 figli (Gen 2025)
    if (root->left != NULL && root->right != NULL) {
        (*countProprieta)++;
    }

    // Esempio Logica: Aggiorna profondità massima (Luglio 2024)
    if (root->left == NULL && root->right == NULL) { // È una foglia
        if (depth > *maxDepth) {
            *maxDepth = depth;
            // Se devi salvare la chiave della foglia migliore:
            // strcpy(bestKeyBuffer, root->key);
        }
    }

    bst_dfs(root->left, depth + 1, maxDepth, countProprieta);
    bst_dfs(root->right, depth + 1, maxDepth, countProprieta);
}

// PATTERN B: STAMPA PERCORSO A RITROSO (Luglio 2024)
// Logica: La funzione ritorna 1 (true) se il nodo fa parte del cammino cercato
int printPathToNode(link root, char *targetKey) {
    if (root == NULL) return 0;

    // Se ho trovato il nodo, inizio a risalire ritornando 1
    if (strcmp(root->key, targetKey) == 0) {
        printf("%s ", root->key); // Stampa il target
        return 1;
    }

    // Cerca a SX o DX
    if (printPathToNode(root->left, targetKey) || printPathToNode(root->right, targetKey)) {
        // Se una delle chiamate sotto ha ritornato 1, allora IO sono nel percorso
        printf("<- %s ", root->key); // Stampa me stesso mentre risalgo
        return 1;
    }

    return 0; // Non trovato in questo ramo
}

// PATTERN C: LINEARIZZAZIONE (BST -> ARRAY) (Feb 2024)
// Visita In-Order (SX, Root, DX) per avere array ordinato
void bstToArray(link root, struct node *vector, int *index) {
    if (root == NULL) return;
    
    bstToArray(root->left, vector, index);
    
    // Copia nel vettore
    vector[*index].val = root->val;
    // vector[*index].key = strdup(root->key); // Se stringhe dinamiche
    (*index)++;
    
    bstToArray(root->right, vector, index);
}

/* ---------------------------------------------------------------------------------
 * SEZIONE 3: VETTORI, STRINGHE E MEMORIA (ADT)
 * Uscito in: Jan 2025 (Sottosequenza), Feb 2025 (Editori/Deep Copy), Luglio 2024 (DiffVett)
 * --------------------------------------------------------------------------------- */

// PATTERN A: SOTTOSEQUENZA / DIFFERENZA (2 vettori)
// Risolve: "a è sottosequenza di b?" (Jan 2025) o "Elementi in a ma non in b" (Luglio 2024)
// Assunzione: Spesso i vettori sono ORDINATI (per la differenza) o si scorrono linearmente.
int isSubSequence(int *A, int nA, int *B, int nB) {
    int i = 0, j = 0;
    // i scorre il "piccolo" (sottosequenza), j scorre il "grande"
    while (i < nA && j < nB) {
        if (A[i] == B[j]) {
            i++; // Trovato match, avanzo target
            j++; // Avanzo sorgente
        } else {
            j++; // Non match, avanzo solo sorgente
        }
    }
    return (i == nA); // Se i è arrivato in fondo, ho trovato tutti gli elementi
}

// PATTERN B: DEEP COPY DI STRUTTURE COMPLESSE (Feb 2025 - Editori)
// Fondamentale: Se una struct contiene un puntatore, devi fare malloc anche per quello!
typedef struct {
    char *nome;
    int *voti;  // Array dinamico interno
    int n_voti;
} Studente;

typedef struct {
    Studente *elenco; // Array dinamico di studenti
    int n_studenti;
} Classe;

Classe *duplicaClasse(Classe *src) {
    // 1. Malloc della struttura contenitore
    Classe *dest = malloc(sizeof(Classe));
    dest->n_studenti = src->n_studenti;

    // 2. Malloc del vettore interno
    dest->elenco = malloc(dest->n_studenti * sizeof(Studente));

    // 3. Ciclo per copiare ogni elemento e le sue sottostrutture (Deep Copy)
    for (int i = 0; i < src->n_studenti; i++) {
        // Copia campi scalari
        dest->elenco[i].n_voti = src->elenco[i].n_voti;

        // Malloc e copia Stringa
        dest->elenco[i].nome = strdup(src->elenco[i].nome); 
        // Nota: se non puoi usare strdup: 
        // dest->elenco[i].nome = malloc(strlen(src->elenco[i].nome)+1);
        // strcpy(dest->elenco[i].nome, src->elenco[i].nome);

        // Malloc e copia Array di interi
        dest->elenco[i].voti = malloc(dest->elenco[i].n_voti * sizeof(int));
        for (int k = 0; k < dest->elenco[i].n_voti; k++) {
            dest->elenco[i].voti[k] = src->elenco[i].voti[k];
        }
    }
    return dest;
}

// Funzione di utilità per liberare tutto (spesso richiesta insieme alla duplicazione)
void freeClasse(Classe *c) {
    if (c == NULL) return;
    for (int i = 0; i < c->n_studenti; i++) {
        free(c->elenco[i].nome);
        free(c->elenco[i].voti);
    }
    free(c->elenco);
    free(c);
}


/* ---------------------------------------------------------------------------------
 * SEZIONE 4: Generazione Combinatoria 
 "Generare sequenze", "Numeri con proprietà", "Uscita dal labirinto".
 Esempio (Feb 2024): "Generare numeri di N cifre in base B tale che la somma sia S".
 * --------------------------------------------------------------------------------- */
// Wrapper
void generaNumeri(int N, int Base) {
    int *sol = calloc(N, sizeof(int)); // Vettore per le cifre
    int sommaAttuale = 0; // Se richiesto dal problema
    
    genera_rec(0, N, Base, sol, sommaAttuale);
    
    free(sol);
}

// Funzione Ricorsiva
void genera_rec(int pos, int N, int Base, int *sol, int sommaAttuale) {
    // 1. CASO BASE (Terminazione)
    if (pos >= N) {
        // Ho riempito il vettore. Controllo la condizione finale (se non già controllata nel pruning)
        if (sommaAttuale % 2 == 0) { // Esempio: stampo solo se somma pari
            for (int k=0; k<N; k++) printf("%d", sol[k]);
            printf("\n");
        }
        return;
    }

    // 2. CICLO DELLE SCELTE (Es. Cifre da 0 a Base-1)
    for (int i = 0; i < Base; i++) {
        
        // PRUNING SEMPLICE (Opzionale, ma aiuta)
        // Es: "Non possono esserci due cifre uguali adiacenti"
        if (pos > 0 && sol[pos-1] == i) continue; 

        // APPLICA SCELTA
        sol[pos] = i;
        
        // RICORSIONE
        genera_rec(pos + 1, N, Base, sol, sommaAttuale + i);
        
        // BACKTRACK (Nessuna azione necessaria qui se sovrascriviamo sol[pos] al prossimo giro)
    }
}


/* ---------------------------------------------------------------------------------
 * SEZIONE 5: ESPLORAZIONE MATRICE / LABIRINTO (DFS)
Esempio (Luglio 2024): "Trovare il cammino minimo tra due celle" o "Verificare se esiste uscita".
 * --------------------------------------------------------------------------------- */

// Definisci direzioni (N, E, S, O)
int dr[] = {-1, 0, 1, 0};
int dc[] = {0, 1, 0, -1};

// Wrapper e Ricorsiva spesso fuse insieme nei 12pt, ma usiamo lo standard:
// Ritorna 1 se trova il cammino, 0 altrimenti
int cercaCammino(int r, int c, int r_dest, int c_dest, int R, int C, int **mat, int **visited) {
    
    // 1. CHECK VALIDITÀ E VISITA
    // Fuori dai bordi? Muro (es. 0)? Già visitato? -> Ritorna 0
    if (r < 0 || r >= R || c < 0 || c >= C || mat[r][c] == 0 || visited[r][c]) 
        return 0;

    // 2. CASO BASE (Trovato target)
    if (r == r_dest && c == c_dest) {
        return 1;
    }

    // 3. MARCA COME VISITATO
    visited[r][c] = 1;

    // 4. PROVA LE 4 DIREZIONI
    int found = 0;
    for (int k = 0; k < 4; k++) {
        if (cercaCammino(r + dr[k], c + dc[k], r_dest, c_dest, R, C, mat, visited)) {
            found = 1;
            break; // Trovato! Interrompo.
        }
    }

    // 5. BACKTRACK (Fondamentale!)
    // Se voglio cercare TUTTI i cammini o il cammino minimo, devo smarcare.
    // Se basta trovarne UNO qualsiasi, spesso non serve smarcare (dipende dal testo).
    // Nel dubbio, per "cammino minimo" o "tutti i cammini", SMARCA.
    visited[r][c] = 0; 

    return found;
}

 
/* ---------------------------------------------------------------------------------
 * SEZIONE 6: PATTERN C: PERMUTAZIONE SEMPLICE (Anagrammi / Stringhe) 
 Esempio: "Date N parole, concatenale per formare la frase più lunga".
 * --------------------------------------------------------------------------------- */
// Simile al Pattern A, ma usiamo un array 'mark' per non riusare le parole
void permutaStringhe(int pos, int N, char **parole, int *mark, char *soluzioneCorrente) {
    
    // Ciclo su tutte le parole disponibili
    for (int i = 0; i < N; i++) {
        if (mark[i] == 0) { // Se parola 'i' non usata
            
            // Check validità (Es. l'ultima lettera della sol. coincide con la prima della parola i?)
            if (pos > 0 && !puoConcatenare(soluzioneCorrente, parole[i])) 
                continue;

            mark[i] = 1;
            // Aggiungi parola alla soluzione...
            
            permutaStringhe(pos + 1, N, parole, mark, soluzioneCorrente);
            
            mark[i] = 0; // Backtrack
            // Rimuovi parola...
        }
    }
}

/* ---------------------------------------------------------------------------------
 * SEZIONE 7: Verifica Cammino su Grafo
 Esempio: Agosto 2025
 * --------------------------------------------------------------------------------- */
// Verifica se una sequenza di vertici è un cammino valido nel grafo
// Matrice di adiacenza (adj) dove adj[u][v] == 1 se c'è arco
int checkPath(int *path, int k, int **adj, int V) {
    for (int i = 0; i < k - 1; i++) {
        int u = path[i];
        int v = path[i+1];
        // Se non c'è arco tra u e v, non è un cammino
        if (adj[u][v] == 0) return 0;
    }
    return 1;
}

 /* ---------------------------------------------------------------------------------
 * SEZIONE 8: Verifica Proprietà BST
 Esempio: Agosto 2025
 * --------------------------------------------------------------------------------- */

 // Ritorna 1 se è un BST valido, 0 altrimenti
// Chiamata iniziale: checkBST(root, -999999, 999999);
int checkBST(link h, int min, int max) {
    if (h == NULL) return 1;

    // Condizione violata
    if (h->val <= min || h->val >= max) return 0;

    // Controllo ricorsivo restringendo l'intervallo
    return checkBST(h->left, min, h->val) && 
           checkBST(h->right, h->val, max);
}





 /* ---------------------------------------------------------------------------------
 * SEZIONE 10: Misto resto
 * --------------------------------------------------------------------------------- */

/* ==============================================================================
 * PARTE 1: TYPEDEF e STRUCT
 * ============================================================================== */

// Definizione classica
struct studente_s {
    char nome[50];
    int matricola;
};

// TYPEDEF: Creo un alias.
// D'ora in poi posso scrivere "Studente" invece di "struct studente_s"
typedef struct studente_s Studente;

// Posso anche definire puntatori specifici (comodo per le Linked List)
typedef Studente *LinkStudente; 


/* ==============================================================================
 * PARTE 2: PROTOTIPI (Come usare i puntatori nelle funzioni)
 * ============================================================================== */

// Funzione che modifica un intero (PUNTATORE SINGOLO)
// Parametro: int *n -> "Dammi l'indirizzo di un intero"
void raddoppia(int *n) {
    *n = (*n) * 2; // Accedo al valore puntato e lo modifico
}

// Funzione che alloca una matrice dentro la funzione (TRIPLO PUNTATORE)
// Perché triplo? 
// 1. Una matrice è int** (doppio puntatore).
// 2. Se voglio MODIFICARE la variabile "matrice" (allocarla) dentro una funzione,
//    devo passare il suo indirizzo.
// 3. Indirizzo di (int**) -> diventa (int***).
void creaMatrice(int ***matrice, int righe, int colonne) {
    
    // Deferenziamento (*matrice) per accedere alla variabile originale nel main
    *matrice = malloc(righe * sizeof(int *)); // Alloco il vettore di righe
    
    for (int i = 0; i < righe; i++) {
        // (*matrice)[i] è la riga i-esima.
        // Uso CALLOC per avere tutto a 0 (molto comodo per matrici)
        (*matrice)[i] = calloc(colonne, sizeof(int)); 
    }
}

// Restituisco int** invece di void
int **creaMatrice(int righe, int colonne) {
    int **mat = malloc(righe * sizeof(int *)); // Creo locale
    
    for (int i = 0; i < righe; i++) {
        mat[i] = calloc(colonne, sizeof(int));
    }
    
    return mat; // Restituisco l'indirizzo al main
}




void creaCubo(int ****cubo, int pagine, int righe, int colonne) {
    // 1. ALLOCO IL VETTORE DELLE PAGINE (La "colonna vertebrale" del cubo)
    // Qui cubo è int ****, quindi *cubo è int ***
    // Dentro sizeof metto int ** perché ogni elemento punta a una matrice 2D
    *cubo = malloc(pagine * sizeof(int **)); 

    for (int i = 0; i < pagine; i++) {
        
        // 2. Per ogni pagina, alloco le RIGHE (come facevi prima)
        (*cubo)[i] = malloc(righe * sizeof(int *));
        
        for (int j = 0; j < righe; j++) {
            
            // 3. Per ogni riga, alloco le COLONNE (i dati veri)
            (*cubo)[i][j] = malloc(colonne * sizeof(int));
        }
    }
}




/* ==============================================================================
 * MAIN: IL LABORATORIO
 * ============================================================================== */
int main() {

    printf("--- 1. MALLOC & STRUCT ---\n");
    // Alloco UNO studente dinamicamente
    // MALLOC: "Dammi X byte sporchi"
    Studente *s1 = malloc(sizeof(Studente)); 
    
    if (s1 == NULL) { printf("Errore memoria"); exit(1); }

    // Accesso ai campi con FRECCIA (->) perché s1 è un puntatore
    s1->matricola = 12345;
    strcpy(s1->nome, "Mario Rossi"); 
    
    printf("Studente: %s, Matr: %d\n", s1->nome, s1->matricola);


    printf("\n--- 2. REALLOC (Vettori Dinamici) ---\n");
    // Alloco un vettore di 2 interi
    int *vettore = malloc(2 * sizeof(int));
    vettore[0] = 10;
    vettore[1] = 20;
    
    printf("Dimensione originale: 2 elementi\n");

    // REALLOC: "Mi serve più spazio, allarga la casa"
    // Se non c'è spazio contiguo, sposta tutto e mi ridà il nuovo indirizzo.
    // Importante: riassegnare sempre a se stesso (vettore = realloc...)
    int *temp = realloc(vettore, 4 * sizeof(int)); 
    
    if (temp != NULL) {
        vettore = temp; // Aggiorno il puntatore solo se è andata bene
        vettore[2] = 30; // Ora ho spazio per questi
        vettore[3] = 40;
        printf("Realloc riuscita: ora ho 4 elementi.\n");
    }


    printf("\n--- 3. MATRICI (DOPPI PUNTATORI) ---\n");
    int R = 3, C = 4;
    
    // Una matrice è un "Puntatore a Puntatori" (int **)
    int **miaMatrice; 

    // Chiamo la funzione per allocarla.
    // Passo &miaMatrice (indirizzo di un int**) -> Ecco il TRIPLO PUNTATORE
    creaMatrice(&miaMatrice, R, C);

    // Uso la matrice normalmente (doppia indicizzazione)
    miaMatrice[1][1] = 99; // Modifico un valore
    
    printf("Matrice creata e inizializzata a 0 (grazie a calloc).\n");
    printf("Valore in [0][0]: %d\n", miaMatrice[0][0]);
    printf("Valore in [1][1]: %d (modificato)\n", miaMatrice[1][1]);


    printf("\n--- 4. PULIZIA (FREE) ---\n");
    // 1. Libero lo studente
    free(s1);

    // 2. Libero il vettore
    free(vettore);

    // 3. Libero la matrice (ATTENZIONE: Si libera al contrario dell'allocazione)
    // Prima le righe...
    for (int i = 0; i < R; i++) {
        free(miaMatrice[i]);
    }
    // ...poi il contenitore delle righe
    free(miaMatrice);

    printf("Memoria pulita. Esame salvo.\n");

    return 0;
}



// [Gennaio 2026] Conta nodi con valore compreso tra A e B (inclusi)
int BST_countRange(link h, int A, int B) {
    if (h == NULL) return 0;

    // Caso 1: Nodo troppo piccolo -> Cerco solo a destra (valori più grandi)
    if (h->val < A) 
        return BST_countRange(h->r, A, B);

    // Caso 2: Nodo troppo grande -> Cerco solo a sinistra (valori più piccoli)
    if (h->val > B) 
        return BST_countRange(h->l, A, B);

    // Caso 3: Nodo nell'intervallo -> Conto 1 e cerco su entrambi i lati
    return 1 + BST_countRange(h->l, A, B) + BST_countRange(h->r, A, B);
}


/* ---------------------------------------------------------------------------------
 * NUOVA SEZIONE: MATRICI (Allocazione ed Esplorazione "Vicini")
 * --------------------------------------------------------------------------------- */

// PATTERN A: Allocazione dinamica di una matrice R x C
int** allocaMatrice(int r, int c) {
    int **m = malloc(r * sizeof(int*));
    for (int i = 0; i < r; i++) {
        m[i] = calloc(c, sizeof(int)); // Inizializza a zero
    }
    return m;
}

// PATTERN B: Esplorazione dei vicini (es. per calcolare medie, o controllare bordi)
void analizzaVicini(int **M, int r, int c, int i, int j) {
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue; // Salto la cella centrale [i][j]
            
            int nx = i + dx;
            int ny = j + dy;
            
            // Controllo limiti di bordo
            if (nx >= 0 && nx < r && ny >= 0 && ny < c) {
                // Fai qualcosa con M[nx][ny]
            }
        }
    }
}


/* ---------------------------------------------------------------------------------
 * NUOVA SEZIONE: ALBERI N-ARI
 * --------------------------------------------------------------------------------- */
#define GRADO 3 // Può variare in base al testo dell'esame

struct n_node {
    int val;
    struct n_node *children[GRADO];
    int n_child; // Numero effettivo di figli (opzionale)
};

// Visita DFS di base su albero N-ario
void ntree_dfs(struct n_node *root, int *counter) {
    if (root == NULL) return;
    
    // Esempio: conta i nodi (o controlla il grado, ecc.)
    (*counter)++;
    
    // Chiamata ricorsiva sui figli
    for (int i = 0; i < GRADO; i++) {
        if (root->children[i] != NULL) {
            ntree_dfs(root->children[i], counter);
        }
    }
}

/* ---------------------------------------------------------------------------------
 * PATTERN AGGIUNTIVO BST: VERIFICA PROPRIETA' BST (2025, 2022)
 * --------------------------------------------------------------------------------- */
int isBSTUtil(link node, int min, int max) {
    if (node == NULL) return 1;
    
    // Controlla che il nodo violi i limiti correnti
    if (node->val < min || node->val > max) return 0;
    
    // Sulla SX il max scende, sulla DX il min sale.
    return isBSTUtil(node->left, min, node->val - 1) &&
           isBSTUtil(node->right, node->val + 1, max);
}

int checkIsBST(BST tree) {
    // Usa costanti climatiche (o INT_MIN / INT_MAX di <limits.h>)
    return isBSTUtil(tree->root, -99999, 99999);
}


/* ---------------------------------------------------------------------------------
 * PATTERN AGGIUNTIVO LISTE: Duplicati Consecutivi e Riorganizzazione
 * --------------------------------------------------------------------------------- */
 
// Rimozione duplicati consecutivi (mantenendo il primo) - Uscito nel 2023
void removeConsecutiveDuplicates(link head) {
    link curr = head;
    while (curr != NULL && curr->next != NULL) {
        if (curr->val == curr->next->val) {
            link temp = curr->next;
            curr->next = curr->next->next;
            free(temp); // Libera nodo se non serve (valuta caso stringhe)
        } else {
            curr = curr->next;
        }
    }
}

// Estrarre un nodo e reinserirlo ordinato (es. dopo aver cambiato chiave - Feb 2025)
int listDoubleItem(link *head, int target) {
    link curr = *head, prev = NULL;
    
    // 1. Cerca ed estrai
    while (curr != NULL && curr->val != target) {
        prev = curr;
        curr = curr->next;
    }
    if (curr == NULL) return 0; // Target non trovato
    
    if (prev == NULL) *head = curr->next;
    else prev->next = curr->next;
    
    // 2. Modifica 
    curr->val = curr->val * 2;
    
    // 3. Reinserisci al posto giusto (inserimento in lista ordinata)
    link p = *head, p_prev = NULL;
    while (p != NULL && p->val < curr->val) {
        p_prev = p;
        p = p->next;
    }
    if (p_prev == NULL) {
        curr->next = *head;
        *head = curr;
    } else {
        curr->next = p;
        p_prev->next = curr;
    }
    return 1;
}

/* ---------------------------------------------------------------------------------
 * NUOVA SEZIONE: BACKTRACKING E COMBINATORIA BASE
 * --------------------------------------------------------------------------------- */

// PATTERN A: Ricorsione su Griglia / Percorso minimo (Luglio 2024)
void minPathRec(char **area, int r, int c, int x, int y, int x1, int y1, int **visited, int dist, int *minDist) {
    // Caso terminale: sono arrivato a (x1, y1)
    if (x == x1 && y == y1) {
        if (dist < *minDist) *minDist = dist;
        return;
    }
    
    visited[x][y] = 1;
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    // Provo le 4 direzioni (Nord, Sud, Ovest, Est)
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        // Verifica confini e che la cella sia calpestabile ('0') e non ancora visitata
        if (nx >= 0 && nx < r && ny >= 0 && ny < c && area[nx][ny] == '0' && !visited[nx][ny]) {
            minPathRec(area, r, c, nx, ny, x1, y1, visited, dist + 1, minDist);
        }
    }
    visited[x][y] = 0; // Backtrack
}

// PATTERN B: Powerset con Vettore Booleano (Subset Sum / Best Split 2025)
// Spesso utilizzato per selezionare sotto-insiemi con un vincolo ottimizante
void powerset(int *val, int n, int *sol, int pos, int *bestSol, int *bestDiff) {
    if (pos >= n) {
        // Valuta soluzione attuale (es. calcola differenza somme per partizione ottima)
        int sum1 = 0, sum2 = 0;
        for (int i = 0; i < n; i++) {
            if (sol[i] == 1) sum1 += val[i];
            else sum2 += val[i];
        }
        int diff = abs(sum1 - sum2);
        
        // Aggiorna soluzione migliore
        if (diff < *bestDiff) {
            *bestDiff = diff;
            for (int i = 0; i < n; i++) bestSol[i] = sol[i];
        }
        return;
    }
    
    // Ramo: non seleziono l'elemento
    sol[pos] = 0;
    powerset(val, n, sol, pos + 1, bestSol, bestDiff);
    
    // Ramo: seleziono l'elemento
    sol[pos] = 1;
    powerset(val, n, sol, pos + 1, bestSol, bestDiff);
}

/* ---------------------------------------------------------------------------------
 * SEZIONE 10: GRAFI SEMPLICI CON LISTE DI ADIACENZA (12pt)
 * --------------------------------------------------------------------------------- */
typedef struct edgeNode *linkEdge;
struct edgeNode { int v; linkEdge next; };
struct graph { int V; int E; linkEdge *ladj; };
typedef struct graph *Graph;

// Pattern A: Verifica cammino su liste di adiacenza (Es: Cammino Hamiltoniano)
int checkPathListAdj(Graph G, int *path, int n) {
    for(int i = 0; i < n - 1; i++) {
        int u = path[i];
        int v = path[i+1];
        int found = 0;
        
        // Scorro la lista di adiacenza del nodo u
        for (linkEdge t = G->ladj[u]; t != NULL; t = t->next) {
            if (t->v == v) { 
                found = 1; 
                break; 
            }
        }
        if (!found) return 0; // L'arco (u -> v) non esiste!
    }
    return 1;
}

// Pattern B: Verifica se il grafo è simmetrico (2025)
int isSymmetric(Graph G) {
    for (int i = 0; i < G->V; i++) {
        for (linkEdge t = G->ladj[i]; t != NULL; t = t->next) {
            int target = t->v;
            int found_reverse = 0;
            // Cerco l'arco di ritorno: target -> i
            for (linkEdge rev = G->ladj[target]; rev != NULL; rev = rev->next) {
                if (rev->v == i) { found_reverse = 1; break; }
            }
            if (!found_reverse) return 0;
        }
    }
    return 1;
}



/* ---------------------------------------------------------------------------------
 * SEZIONE 11: CLONAZIONE (DEEP COPY) ALBERI
 * --------------------------------------------------------------------------------- */
// Funzione ricorsiva di clonazione
link dupTreeRec(link h) {
    if (h == NULL) return NULL;
    
    // 1. Alloco il nuovo nodo
    link n = malloc(sizeof(struct node));
    n->val = h->val; 
    // n->key = strdup(h->key); // (Se la chiave e' stringa dinamica)
    
    // 2. Clono figlio SX e DX
    n->left = dupTreeRec(h->left);
    n->right = dupTreeRec(h->right);
    
    return n;
}

// Wrapper per la clonazione di un albero
BST BSTDup(BST b) {
    BST clone = malloc(sizeof(struct binary_search_tree));
    clone->size = b->size;
    clone->root = dupTreeRec(b->root);
    return clone;
}

/* ---------------------------------------------------------------------------------
 * SEZIONE 12: BFS (VISITA A LIVELLI) SU BST
 * --------------------------------------------------------------------------------- */
// Raccoglie o visita nodi livello per livello senza usare l'ADT Coda
void BSTbfsToArray(link root) {
    if (root == NULL) return;
    
    link queue[100]; // Dimensione sufficiente max
    int head = 0, tail = 0;
    
    queue[tail++] = root; // Enqueue radice
    
    while (head < tail) {
        link curr = queue[head++]; // Dequeue
        
        printf("%d ", curr->val); // O salvataggio in un array di destinazione
        
        // Mette in coda i figli
        if (curr->left != NULL) queue[tail++] = curr->left;
        if (curr->right != NULL) queue[tail++] = curr->right;
    }
}


/* ---------------------------------------------------------------------------------
 * SEZIONE 13: RE-INSERIMENTO ORDINATO (LISTE)
 * --------------------------------------------------------------------------------- */
// Spesso si stacca un nodo e lo si deve rimettere al posto giusto
void listInsertSorted(link *head, link newNode) {
    // Caso 1: Lista vuota o inserimento in TESTA
    if (*head == NULL || (*head)->val >= newNode->val) {
        newNode->next = *head;
        *head = newNode;
        return;
    }
    
    // Caso 2: Ricerca posizione nel CORPO della lista
    link curr = *head;
    while (curr->next != NULL && curr->next->val < newNode->val) {
        curr = curr->next;
    }
    
    // Inserisci newNode tra curr e curr->next
    newNode->next = curr->next;
    curr->next = newNode;
}

/* ---------------------------------------------------------------------------------
 * SEZIONE 14: RICERCA PAROLE IN MATRICE (Word Search / Crucipuzzle)
 * --------------------------------------------------------------------------------- */
// Direzioni: N, NE, E, SE, S, SO, O, NO
int checkWord2D(char **mat, int R, int C, int r, int c, char *word) {
    int dr[] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int dc[] = {0, 1, 1, 1, 0, -1, -1, -1};
    int len = strlen(word);
    
    // Se la prima lettera non combacia, esci
    if (mat[r][c] != word[0]) return 0;
    
    // Prova le 8 direzioni
    for (int dir = 0; dir < 8; dir++) {
        int k;
        int currR = r + dr[dir];
        int currC = c + dc[dir];
        
        for (k = 1; k < len; k++) {
            // Fuori dai bordi
            if (currR < 0 || currR >= R || currC < 0 || currC >= C) break;
            
            // Carattere non coincidente
            if (mat[currR][currC] != word[k]) break;
            
            currR += dr[dir];
            currC += dc[dir];
        }
        if (k == len) return 1; // Parola trovata interamente in questa direzione!
    }
    return 0;
}

/* ---------------------------------------------------------------------------------
 * GRAFI CON LISTE DI ADIACENZA (ADT Graph)
 * --------------------------------------------------------------------------------- */
typedef struct edge { int v; int w; int wt; } Edge;
typedef struct graph_node *graph_link;

struct graph_node { 
    int v; 
    int wt; 
    graph_link next; 
};

struct graph { 
    int V; 
    int E; 
    graph_link *ladj; // Vettore di liste di adiacenza
};
typedef struct graph *Graph;

// Pattern: Verifica se un arco esiste (utile per cammini o grafi simmetrici)
int edgeExists(Graph G, int u, int v) {
    for (graph_link t = G->ladj[u]; t != NULL; t = t->next) {
        if (t->v == v) return 1;
    }
    return 0;
}

// Pattern: Verifica se una sequenza è un cammino valido su lista di adiacenze
int checkPathAdjacencyList(Graph G, int *path, int k) {
    for (int i = 0; i < k - 1; i++) {
        if (!edgeExists(G, path[i], path[i+1])) {
            return 0; // Arco mancante, cammino interrotto
        }
    }
    return 1;
}

/* ---------------------------------------------------------------------------------
 * HEAP (Code a Priorità)
 * --------------------------------------------------------------------------------- */
struct heap {
    Item *A;        // Vettore che implementa lo heap
    int heapsize;   // Numero di elementi attuali
};
typedef struct heap *HEAP;

// Funzioni di navigazione array-tree
int LEFT(int i) { return 2 * i + 1; }
int RIGHT(int i) { return 2 * i + 2; }
int PARENT(int i) { return (i - 1) / 2; }

// Pattern: Ricostruire un Albero Binario (BT) partendo dall'array dell'HEAP
link buildBTfromHeap(HEAP h, int i) {
    if (i >= h->heapsize) return NULL; // Fuori dai limiti
    
    link root = malloc(sizeof(struct node));
    root->item = h->A[i];
    
    root->l = buildBTfromHeap(h, LEFT(i));
    root->r = buildBTfromHeap(h, RIGHT(i));
    
    return root;
}

// PATTERN C: COSTRUZIONE VETTORE DIFFERENZA (Elementi in A ma non in B)
int *diffVett(int *A, int nA, int *B, int nB, int *nC) {
    int *C = malloc(nA * sizeof(int)); // Alloco il caso peggiore (tutti in A)
    int i = 0, j = 0, k = 0;
    
    while (i < nA && j < nB) {
        if (A[i] < B[j]) { 
            C[k++] = A[i++]; // Elemento in A ma non in B: lo aggiungo!
        } 
        else if (A[i] > B[j]) { 
            j++; // Avanzo solo in B
        } 
        else { 
            // Elemento in comune: lo salto
            i++; 
            j++; 
        }
    }
    // Svuoto i rimanenti di A
    while (i < nA) { 
        C[k++] = A[i++]; 
    }
    
    *nC = k; // Salvo la dimensione effettiva nel puntatore di uscita
    C = realloc(C, k * sizeof(int)); // (Opzionale ma pulito) Ridimensiono la memoria strettamente necessaria
    return C;
}

// PATTERN C: SPOSTAMENTO/RIPOSIZIONAMENTO DI UN NODO
// Estrae il nodo "target" dai suoi vicini e lo reinserisce preservando l'ordinamento
void moveNodeSorted(link *head, link target, link targetPrev) {
    if (target == NULL) return;

    // 1. Slaccia il nodo target dalla lista (senza liberare memoria)
    if (targetPrev == NULL) *head = target->next;
    else targetPrev->next = target->next;

    // 2. Modifica il valore del nodo (Es: raddoppia come nell'esame 2025)
    target->val = target->val * 2; 

    // 3. Reinserisci in modo ordinato il nodo "target"
    link curr = *head, prev = NULL;
    while (curr != NULL && curr->val < target->val) {
        prev = curr;
        curr = curr->next;
    }
    
    // Ricollega il target
    target->next = curr;
    if (prev == NULL) *head = target;
    else prev->next = target;
}


/* ---------------------------------------------------------------------------------
Symbol Table / Hash Table
 * --------------------------------------------------------------------------------- */
typedef struct STnode *link;

struct STnode {
    char *key;      // La chiave (es: nome di un vertice o codice fiscale)
    int value;      // Il dato associato
    link next;      // Puntatore per gestire le collisioni (lista)
};

struct symboltable {
    link *heads;    // Vettore di puntatori (le teste delle liste)
    int M;          // Dimensione della tabella
};

typedef struct symboltable *ST;

int hash(char *v, int M) {
    int h = 0, base = 127;
    for ( ; *v != '\0'; v++)
        h = (base * h + *v) % M;
    return h;
}

ST STinit(int M) {
    ST st = malloc(sizeof(*st));
    st->M = M;
    // Creiamo il vettore di teste e puliamolo con calloc (tutte a NULL)
    st->heads = calloc(M, sizeof(link));
    return st;
}

void STinsert(ST st, char *key, int val) {
    int i = hash(key, st->M); // Trovo l'indice
    
    // Creo il nuovo nodo
    link x = malloc(sizeof(*x));
    x->key = strdup(key); // Copia sicura della stringa
    x->value = val;
    
    // Inserimento in testa alla lista i-esima
    x->next = st->heads[i];
    st->heads[i] = x;
}

int STsearch(ST st, char *key) {
    int i = hash(key, st->M); // Vado dritto al cassetto giusto
    
    // Scorro la piccola lista in quel cassetto
    for (link x = st->heads[i]; x != NULL; x = x->next) {
        if (strcmp(x->key, key) == 0) 
            return x->value; // Trovato!
    }
    return -1; // Non presente
}