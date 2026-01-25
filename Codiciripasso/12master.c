#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * SEZIONE 9: Mini calcolo combinatorio
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
