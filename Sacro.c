/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////CALCOLO COMBINATORIO


/*
1. Modello Binario (Sottoinsiemi / Power Set / Combinazioni)

Logica: "Prendo o Lascio?"  "Seleziono"
Uso: Trovare un gruppo di elementi (es. "Zaino", "Somma uguale a K"). 
Caratteristica: Non c'è il ciclo for. 
L'albero si biforca sempre in 2.

Sottoinsiemi: un sottoinsieme con pruning
Powerset: insieme delle parti, tutti i possibili sottoinsiemi generabili da N elementi
Combinazioni: sottoinsieme di dimensione fissa k

Hai un elenco di 10 progetti aziendali. 
Ogni progetto ha un Costo e un Guadagno. 
Hai un Budget massimo di 1000€. Obiettivo: 
Seleziona il sottoinsieme di progetti che massimizza il guadagno totale senza sforare il budget.

 */

/*
 * index: indice dell'oggetto che stiamo valutando (da 0 a N-1)
 * curr_sol: vettore che contiene gli elementi presi finora
 * count: quanti elementi ho preso finora
 * val: vettore input con i valori (es. pesi, numeri)
 * N numero totale di oggetti da prendere o lasciare
 */
void solve_binaria(int index, int *curr_sol, int count, int N, int *val) {

    // --- PRUNING (Opzionale) ---
    // Esempio: Se ho già preso troppi elementi o superato un peso massimo
    // if (count > k || peso_attuale > max_peso) return;


    // --- BASE CASE ---
    if (index == N) {
        // Ho deciso per tutti gli N oggetti. Controllo se la soluzione è valida.
        if (check_validita(curr_sol, count)) {
            salva_soluzione(curr_sol, count);
        }
        return;
    }

    // --- RAMO 1: PRENDO l'elemento 'index' ---
    curr_sol[count] = val[index]; // Lo aggiungo al sacco
    solve_binaria(index + 1, curr_sol, count + 1, N, val);

    // --- RAMO 2: LASCIO l'elemento 'index' ---
    // Non faccio nulla su curr_sol, al prossimo "PRENDO" sovrascriverò questa posizione.
    // Passo al prossimo oggetto (index + 1) ma il count non aumenta.
    solve_binaria(index + 1, curr_sol, count, N, val);
}

///////////////////////////////////////////////////////
/*
2. Modello a Slot (Permutazioni / Disposizioni)
Logica: "Chi metto in questa posizione?" "Ordinamento/Sequenza"
Uso: Anagrammi, N-Regine, Classifiche, Commesso Viaggiatore (TSP). 
Caratteristica: C'è il ciclo for su tutti gli elementi. 
Serve mark[] per non ripetere.

Permutazioni : in che ordine metto Tutti questi oggetti
Disposizioni : Chi metto al k° posto?


Hai 5 città (A, B, C, D, E) e una matrice con le distanze tra loro. 
Obiettivo: Trova l'ordine di visita (es. A -> C -> E -> B -> D -> A) 
che minimizza i km totali percorsi, visitando ogni città una sola volta.

*/

/*
 * pos: la posizione (slot) che stiamo riempiendo adesso (0, 1, ... k-1)
 * curr_sol: vettore soluzione che stiamo costruendo
 * mark: vettore booleano (0/1) per ricordare chi è già stato usato
 * k: numero totale di slot da riempire (spesso k = N nelle permutazioni)
 * N numero totale di candidati disponibili da inserire negli slot
 */
void solve_slot(int pos, int *curr_sol, int *mark, int N, int k, int *val) {

    // --- BASE CASE ---
    if (pos == k) {
        // Ho riempito tutti i k slot. La soluzione è pronta.
        salva_soluzione(curr_sol, k);
        return;
    }

    // VARIANTE 1: RIPETIZIONE INFINITA (PIN)
    // -> Rimuovi completamente l'array 'mark' e l'if(mark == 0).

    // VARIANTE 2: ANAGRAMMI (Input con duplicati: A, A, B)
    // 1. Ordina il vettore 'val' nel main (qsort).
    // 2. Aggiungi questa riga DENTRO il ciclo for, prima del mark:
    // Da inserire nel ciclo for, subito dopo if(mark[i]==0)
    // Serve array ordinato all'origine!
    //if (i > 0 && val[i] == val[i-1] && mark[i-1] == 0) continue;

    // --- CICLO DELLE SCELTE ---
    // Per lo slot 'pos', provo a metterci OGNI elemento 'i' disponibile
    for (int i = 0; i < N; i++) {
        
        if (mark[i] == 0) { // Se l'elemento 'i' NON è ancora stato usato
            
            // 1. DO (Faccio la mossa)
            mark[i] = 1;          // Lo segno come usato
            curr_sol[pos] = val[i]; // Lo metto nello slot
            
            // 2. RECURSE (Vado avanti)
            // Nota: passo 'pos + 1' perché ora devo riempire il prossimo slot
            solve_slot(pos + 1, curr_sol, mark, N, k, val);
            
            // 3. BACKTRACK (Disfaccio la mossa)
            // Fondamentale: libero l'elemento 'i' per poterlo usare in altre combinazioni
            mark[i] = 0; 
        }
    }
}

/*Nota: Se l'esercizio è "Disposizioni con ripetizione" (posso riusare i numeri), basta togliere tutto ciò che riguarda mark.*/


//////////////////////////////////

/*
3. Modello Partizioni (Palle nelle Scatole)
Logica: "Metto in una scatola vecchia o apro una nuova?" "Raggruppamento"
Uso: Suddividere un insieme in gruppi (es. Squadre, Bin Packing). 
Caratteristica: Ciclo for dinamico (fino a n_scatole) + 1 chiamata fuori ciclo.


Hai N file da salvare e un numero imprecisato di Hard Disk. Ogni disco ha capienza 1TB.
Obiettivo: Raggruppa i file in modo da usare il minor numero di dischi possibile.
*/

/*
 * index: l'oggetto che devo sistemare adesso (da 0 a N-1)
 * sol: qui sol[index] = numero della scatola dove ho messo l'oggetto index
 * n_scatole: quante scatole ho aperto/utilizzato finora
 * N numero totale di oggetti da sistemare nelle scatole
 */
void solve_partizioni(int index, int *sol, int n_scatole, int N) {

    // --- BASE CASE ---
    if (index == N) {
        // Ho sistemato tutti gli oggetti.
        stampa_partizione(sol, N, n_scatole);
        return;
    }

    // --- OPZIONE A: Metto in una scatola GIÀ ESISTENTE ---
    // Provo a metterlo nella scatola 0, poi nella 1... fino all'ultima aperta
    for (int i = 0; i < n_scatole; i++) {
        sol[index] = i; // Assegno l'oggetto 'index' alla scatola 'i'
        
        // Ricorsione: passo al prossimo oggetto (index+1), 
        // il numero di scatole aperte (n_scatole) NON cambia
        solve_partizioni(index + 1, sol, n_scatole, N);
    }

    // --- OPZIONE B: Apro una NUOVA SCATOLA ---
    // Metto l'oggetto nella prima scatola libera (che ha indice 'n_scatole')
    sol[index] = n_scatole;
    
    // Ricorsione: passo al prossimo oggetto (index+1),
    // il numero di scatole aperte AUMENTA di 1
    solve_partizioni(index + 1, sol, n_scatole + 1, N);
}


///////////////////////////////////////////////////////
/*
4. Modello Griglia 2D (Labirinto / Flood Fill / DFS)

Logica: "Dove mi sposto ora?" (Nord, Sud, Est, Ovest)
Uso: Uscire da un labirinto, trovare percorsi, contare isole (macchie d'olio), parole nel Boggle.
Caratteristica: Invece di un ciclo su N elementi, c'è un ciclo su 4 (o 8) direzioni.
Fondamentale: Matrice "visitato" per non girare in tondo.

Spostamenti (Offset):
Nord: r-1, c      Sud: r+1, c
Ovest: r, c-1     Est: r, c+1

Hai una matrice R x C che rappresenta una mappa (0 = libero, 1 = muro).
Parti da (start_r, start_c).
Obiettivo: Trova un percorso valido per arrivare a (dest_r, dest_c).
*/

/*
 * r, c: coordinate della cella in cui mi trovo ora
 * step: contatore dei passi fatti (lunghezza percorso)
 * mat: la matrice del labirinto (input)
 * visited: matrice di supporto (0/1) per segnare dove sono già passato
 * R, C: dimensioni totali della griglia
 */
void solve_griglia(int r, int c, int step, int **mat, int **visited, int R, int C) {

    // --- BASE CASE 1: Arrivo a destinazione ---
    if (r == R-1 && c == C-1) { // Esempio: devo arrivare in basso a destra
        // Ho trovato un percorso!
        // salva_soluzione o stampa...
        return;
    }
    
    // --- VETTORI SPOSTAMENTO (Nord, Est, Sud, Ovest) ---
    // Usare questi array evita di scrivere 4 if giganti
    int dr[] = {-1, 0, 1, 0}; 
    int dc[] = { 0, 1, 0, -1};

    /* VARIANTE SCACCHI (CAVALLO)
    Se il problema riguarda un Cavallo degli scacchi, cambia solo i vettori dr/dc.
    Il resto del codice rimane IDENTICO (a parte R e C che sono 8).
 
    int dr[] = {-2, -2, -1, -1,  1,  1,  2,  2};
    int dc[] = {-1,  1, -2,  2, -2,  2, -1,  1};
    */

    // --- CICLO DELLE DIREZIONI ---
    for (int i = 0; i < 4; i++) {
        
        int next_r = r + dr[i];
        int next_c = c + dc[i];

        // --- CHECK VALIDITÀ (Il "Pruning" spaziale) ---
        // 1. Sono dentro la matrice?
        // 2. Non è un muro? (mat != 1)
        // 3. Non ci sono già passato? (visited == 0)
        if (next_r >= 0 && next_r < R && next_c >= 0 && next_c < C &&
            mat[next_r][next_c] != 1 && 
            visited[next_r][next_c] == 0) {

            // 1. DO (Segno il passo)
            visited[next_r][next_c] = 1; 
            
            // 2. RECURSE (Mi sposto nella nuova cella)
            solve_griglia(next_r, next_c, step + 1, mat, visited, R, C);

            // 3. BACKTRACK (Libero la cella)
            // Necessario se cerchi TUTTI i percorsi o il percorso MIGLIORE.
            // Se cerchi solo "esiste un percorso" (Flood Fill), puoi toglierlo.
            visited[next_r][next_c] = 0;
        }
    }
}

/* Nota Varianti:
   - Diagonali: aggiungi le 4 combinazioni ai vettori dr/dc (diventano size 8).
   - Flood Fill (Colora area): togli il passo 3 (Backtrack). Una volta colorato, resta colorato.
*/



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// MATRICE E LISTA DI ADIACENZA

/* 
5.  MATRICE DI ADIACENZA
Logica: "Tabella di verità"
Uso: Grafi piccoli (N < 500) o densi.
Caratteristica: Memoria O(N^2). Accesso arco O(1). Iterazione vicini O(N).

Struttura: mat[u][v] == 1 se c'è arco.
*/

#define MAX_N 100 

void dfs_matrix(int u, int N, int mat[MAX_N][MAX_N], int *visited) {
    
    visited[u] = 1; // Visito
    
    // --- CICLO VICINI ---
    // Scorro TUTTA la riga per cercare chi è connesso a 'u'
    for (int v = 0; v < N; v++) {
        if (mat[u][v] != 0 && visited[v] == 0) {
            dfs_matrix(v, N, mat, visited);
        }
    }
}


/*
6.  LISTA DI ADIACENZA
LISTA DI ADIACENZA
Logica: "Rubrica Telefonica"
Uso: Grafi grandi (N > 1000) o sparsi.
Caratteristica: Memoria O(N+E). Iterazione vicini efficiente.

Struttura: Array di puntatori `head[N]`, ognuno punta a una Linked List.
*/

struct Node {
    int dest;
    struct Node* next;
};

void dfs_list(int u, struct Node** adj, int *visited) {

    visited[u] = 1;
    
    // --- CICLO VICINI ---
    // Uso un puntatore temporaneo 't' per scorrere la lista
    struct Node* t = adj[u];
    
    while (t != NULL) {
        int v = t->dest;
        
        if (visited[v] == 0) {
            dfs_list(v, adj, visited);
        }
        t = t->next; // Prossimo nodo nella lista
    }
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// VARI ADT

// A. NODO PER LISTE DI ADIACENZA (GRAFI)
struct Node {
    int dest;          // Destinazione arco
    int weight;        // Peso (opzionale)
    struct Node* next; // Prossimo vicino
};

// B. NODO PER ALBERI BINARI (BST)
typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// C. NODO LISTA CONCATENATA SEMPLICE
typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;

// --- UTILITY: Crea nuovo nodo grafo ---
struct Node* newNodeGraph(int dest) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

/*
   Logica: Layer-by-layer. NON ricorsivo.
   Uso: Cammino Minimo (grafi non pesati).
*/
void bfs(int start, struct Node** adj, int N, int *visited) {
    // Coda statica semplice
    int queue[MAX_N];
    int head = 0, tail = 0;

    // 1. Inserisco sorgente
    visited[start] = 1;
    queue[tail++] = start;

    while (head < tail) { // Finché coda non vuota
        // 2. Estraggo
        int u = queue[head++];
        // printf("Visito %d\n", u);

        // 3. Vicini
        struct Node* t = adj[u];
        while (t != NULL) {
            int v = t->dest;
            if (visited[v] == 0) {
                visited[v] = 1;
                // dist[v] = dist[u] + 1; // Se serve distanza
                queue[tail++] = v;    // Accodo
            }
            t = t->next;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
// 11. LISTE AVANZATE (Intersezione & Filtro)
/////////////////////////////////////////////////////////////////////////////////////////
/*
   Uso: Confrontare due liste ordinate, contare elementi comuni, cancellare nodi.
   Fonte: Temi d'esame Settembre 2025 (Intersezione), Giugno 2025 (Cancellazione).
*/

// A. INTERSEZIONE DUE LISTE ORDINATE (Conta nodi comuni)
// Logica: "Two Pointers". Avanzo chi è più piccolo. Se uguali, conto e avanzo entrambi.
int countCommon(ListNode *h1, ListNode *h2) {
    int cnt = 0;
    while (h1 != NULL && h2 != NULL) {
        if (h1->val < h2->val) {
            h1 = h1->next;
        } else if (h2->val < h1->val) {
            h2 = h2->next;
        } else {
            // Trovato elemento comune
            cnt++;
            h1 = h1->next;
            h2 = h2->next;
        }
    }
    return cnt;
}

// B. CANCELLAZIONE CON CONDIZIONE (es. lunghezza stringa > K)
// Logica: Serve puntatore 'prev' e 'curr'. Gestire testa a parte.
/* Nota: Supponiamo ListNode contenga 'char *str'.
   int listDelete(ListNode **headRef, int threshold) {
       ListNode *curr = *headRef;
       ListNode *prev = NULL;
       int deleted = 0;

       while (curr != NULL) {
           if (strlen(curr->str) > threshold) { // Condizione cancellazione
               ListNode *temp = curr;
               
               if (prev == NULL) { // Sto cancellando la testa
                   *headRef = curr->next;
                   curr = *headRef;
               } else { // Cancello nel corpo
                   prev->next = curr->next;
                   curr = curr->next; // Prev resta dov'è
               }
               // free(temp->str); // Se allocata dinamicamente
               free(temp);
               deleted++;
           } else {
               prev = curr;
               curr = curr->next;
           }
       }
       return deleted;
   }
*/

/////////////////////////////////////////////////////////////////////////////////////////
// 12. ALBERI AVANZATI (Validazione & Distanze)
/////////////////////////////////////////////////////////////////////////////////////////
/*
   Uso: Verificare proprietà BST, calcolare distanze tra nodi (LCA).
   Fonte: Temi d'esame Agosto 2025 (BST Check), Settembre 2025 (Distanza).
*/

// A. VERIFICA PROPRIETÀ BST (Check Order) // Un albero è BST se ogni nodo è > min e < max ereditati dal padre.
int isBST_util(TreeNode* node, int min, int max) {
    if (node == NULL) return 1;

    // Se il valore viola i limiti correnti -> Falso
    if (node->val <= min || node->val >= max) return 0;

    // Scendo a SX: il max diventa il mio valore
    // Scendo a DX: il min diventa il mio valore
    return isBST_util(node->left, min, node->val) &&
           isBST_util(node->right, node->val, max);
}
// Wrapper: chiamare con isBST_util(root, INT_MIN, INT_MAX);

// B. DISTANZA TRA DUE NODI (LCA - Lowest Common Ancestor) // Distanza(u, v) = Dist(root, u) + Dist(root, v) - 2*Dist(root, LCA(u,v))
// Oppure calcolo diretto nel BST:
int BSTdist(TreeNode* root, int k1, int k2) {
    if (root == NULL) return -1; // Non trovati

    // Se entrambi sono più piccoli, il percorso è a sinistra
    if (k1 < root->val && k2 < root->val) 
        return BSTdist(root->left, k1, k2);
    
    // Se entrambi sono più grandi, il percorso è a destra
    if (k1 > root->val && k2 > root->val) 
        return BSTdist(root->right, k1, k2);

    // Se siamo qui, 'root' è l'LCA (Lowest Common Ancestor), oppure uno dei due è root.
    // Ora calcolo la distanza da root a k1 e da root a k2 separatamente
    // (Nota: serve funzione ausiliaria distanceFromNode(root, k))
    // return distanceFromNode(root, k1) + distanceFromNode(root, k2);
    return 0; // Placeholder
}

/////////////////////////////////////////////////////////////////////////////////////////
// 13. GRAFI AVANZATI (Path Check & Simmetria)
/////////////////////////////////////////////////////////////////////////////////////////
/*
   Uso: Verificare cammini Hamiltoniani, Simmetrie.
   Fonte: Temi d'esame Agosto 2025, Giugno 2025.
*/

// A. VERIFICA CAMMINO VALIDO (es. Hamiltoniano) // Input: array 'path' di 'len' vertici.
int checkPath(struct Node** adj, int *path, int len, int N) {
    // 1. Check nodi distinti (se Hamiltoniano) -> usare array visited[N]
    
    // 2. Check esistenza archi consecutivi
    for (int i = 0; i < len - 1; i++) {
        int u = path[i];
        int v = path[i+1];
        
        // Cerco v nella lista di u
        struct Node* t = adj[u];
        int found = 0;
        while(t != NULL) {
            if (t->dest == v) { found = 1; break; }
            t = t->next;
        }
        if (!found) return 0; // Arco mancante
    }
    return 1;
}

// B. VERIFICA SIMMETRIA (Grafo orientato -> non orientato)
// Per ogni arco u->v deve esistere v->u
int checkSymmetry(struct Node** adj, int N) {
    for (int u = 0; u < N; u++) {
        struct Node* t = adj[u];
        while (t != NULL) {
            int v = t->dest;
            // Verifico se esiste arco di ritorno v->u
            struct Node* t2 = adj[v];
            int found = 0;
            while (t2 != NULL) {
                if (t2->dest == u) { found = 1; break; }
                t2 = t2->next;
            }
            if (!found) return 0; // Simmetria violata
            t = t->next;
        }
    }
    return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 14. COMBINATORIA EXTRA (Partitioning & Intervals)
/////////////////////////////////////////////////////////////////////////////////////////
/*
   Uso: "Best Split" (Dividere in due gruppi per minimizzare differenza somme),
        Overlap temporali (risorse minime).
   Fonte: Tema d'esame 21/02/2025.
*/

// A. BEST SPLIT (Minimizzare diff somme S1 e S2)
// È un MODELLO BINARIO dove cerco di avvicinarmi a (Totale / 2).
/*
 * target: TotaleSomma / 2
 * bestDiff: puntatore a variabile inizializzata a INT_MAX
 */
void solve_partition(int idx, int currentSum, int *val, int N, int target, int *bestDiff) {
    
    // Pruning: Se ho già superato il target, la differenza aumenta, inutile continuare
    if (currentSum > target) {
        // Ma controllo se questo scostamento è il migliore finora
        int diff = currentSum - target; // Approssimazione
        if (diff < *bestDiff) *bestDiff = diff;
        return;
    }

    if (idx == N) {
        int diff = target - currentSum;
        if (diff < *bestDiff) *bestDiff = diff;
        return;
    }

    // Ramo 1: Aggiungo al set 1
    solve_partition(idx + 1, currentSum + val[idx], val, N, target, bestDiff);
    
    // Ramo 2: Aggiungo al set 2 (quindi non sommo a currentSum)
    solve_partition(idx + 1, currentSum, val, N, target, bestDiff);
}

// B. INTERVAL OVERLAP (Attività / Minibus) // Calcolare max risorse simultanee.
// Metodo veloce: "Sweep Line" (Array Start/End ordinati).
/*
   1. Creare array eventi: +1 per Start, -1 per End.
   2. Ordinare eventi per tempo.
   3. Scorrere e tenere traccia del 'counter'. Il max valore raggiunto dal counter è la risposta.
   
   Alternativa banale (O(N^2)): Per ogni intervallo, contare quanti altri si sovrappongono.
*/

/////////////////////////////////////////////////////////////////////////////////////////
// 15. GRAFI: PROPRIETÀ STRUTTURALI (Cricca, Eulero, Grado)
/////////////////////////////////////////////////////////////////////////////////////////
/*
   Uso: Verifiche "statiche" su sottoinsiemi di vertici o proprietà globali.
   Fonte: Appelli Gennaio/Febbraio/Settembre 2024.
*/

// A. VERIFICA CRICCA (CLIQUE)
// Una cricca è un sottoinsieme di vertici in cui TUTTI sono collegati con TUTTI.
// Input: array 'nodes' di dimensione 'k' contenente gli indici dei vertici candidati.
int checkClique(int *nodes, int k, struct Node** adj, int N) {
    // Doppio ciclo: ogni coppia (u, v) deve avere un arco
    for (int i = 0; i < k; i++) {
        for (int j = i + 1; j < k; j++) {
            int u = nodes[i];
            int v = nodes[j];
            
            // Verifico adiacenza u->v (o v->u se non orientato)
            struct Node* t = adj[u];
            int found = 0;
            while (t != NULL) {
                if (t->dest == v) { found = 1; break; }
                t = t->next;
            }
            if (!found) return 0; // Manca un arco -> Non è una cricca
        }
    }
    return 1;
}

// B. VERIFICA GRAFO EULERIANO (Ciclo Euleriano)
// Condizione (Grafo Non Orientato): Il grafo è connesso e TUTTI i nodi hanno grado PARI.
// Se chiede "Cammino Euleriano": Esattamente 0 o 2 nodi hanno grado dispari.
int checkEulerian(struct Node** adj, int N) {
    for (int u = 0; u < N; u++) {
        int degree = 0;
        struct Node* t = adj[u];
        while (t != NULL) {
            degree++;
            t = t->next;
        }
        // Se trovo un grado dispari -> Niente Ciclo Euleriano
        if (degree % 2 != 0) return 0; 
    }
    // Nota: servirebbe anche check connettività (DFS), spesso data per assunta.
    return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 16. ALBERI: BILANCIAMENTO (AVL) E RANGE QUERY
/////////////////////////////////////////////////////////////////////////////////////////
/*
   Uso: Esercizi specifici su BST.
   Fonte: Appelli Febbraio 2024.
*/

// A. VERIFICA BILANCIAMENTO AVL (Altezza)
// Un albero è AVL se per OGNI nodo: abs(altezza_sx - altezza_dx) <= 1.
// Ritorna l'altezza se bilanciato, oppure -1 se non lo è.
int checkAVL(TreeNode* root) {
    if (root == NULL) return 0; // Altezza 0 (o -1 a seconda della convenzione)

    int h_left = checkAVL(root->left);
    if (h_left == -1) return -1; // Sottoalbero SX non bilanciato

    int h_right = checkAVL(root->right);
    if (h_right == -1) return -1; // Sottoalbero DX non bilanciato

    if (abs(h_left - h_right) > 1) return -1; // Questo nodo viola AVL

    // Ritorna altezza corrente
    return 1 + (h_left > h_right ? h_left : h_right);
}

// B. CONTA NODI IN INTERVALLO [A, B]
// Sfrutta la proprietà BST per non visitare tutto l'albero.
int BSTcountInRange(TreeNode* root, int A, int B) {
    if (root == NULL) return 0;

    // Caso 1: Il nodo è nell'intervallo -> Conto e vado su entrambi i lati
    if (root->val >= A && root->val <= B) {
        return 1 + BSTcountInRange(root->left, A, B) 
                 + BSTcountInRange(root->right, A, B);
    }
    
    // Caso 2: Il nodo è troppo piccolo (< A) -> Vado solo a destra (per cercare numeri più grandi)
    if (root->val < A) {
        return BSTcountInRange(root->right, A, B);
    }

    // Caso 3: Il nodo è troppo grande (> B) -> Vado solo a sinistra
    return BSTcountInRange(root->left, A, B);
}

/////////////////////////////////////////////////////////////////////////////////////////
// 17. LISTE: SOTTOLISTE (Pattern Matching)
/////////////////////////////////////////////////////////////////////////////////////////
/*
   Uso: Verificare se la lista L1 è contenuta (consecutivamente) in L2.
   Fonte: Appello Gennaio 2024.
*/

// Verifica se L1 è sottolista di L2
int isSubList(ListNode* l1, ListNode* l2) {
    if (l1 == NULL) return 1; // Lista vuota è sempre sottolista
    if (l2 == NULL) return 0; // L1 non è vuota ma L2 sì

    ListNode *p1 = l1;
    ListNode *p2 = l2;
    ListNode *start = l2; // Punto di ripartenza in L2

    while (start != NULL) {
        p2 = start;
        p1 = l1;
        
        // Provo a matchare L1 partendo da 'start'
        while (p1 != NULL && p2 != NULL && p1->val == p2->val) {
            p1 = p1->next;
            p2 = p2->next;
        }

        if (p1 == NULL) return 1; // Ho finito L1 -> Trovata!
        
        start = start->next; // Avanzo il punto di partenza
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 18. COMBINATORIA: DOMINATING SET (Validità Copertura)
/////////////////////////////////////////////////////////////////////////////////////////
/*
   Uso: Problemi da 18pt tipo "Seleziona minimo numero di nodi per coprire tutti gli altri".
   Logica: Usare Modello Binario (Prendo/Lascio nodo) + Questa funzione check.
   Fonte: Appello 13/02/2024 (Esercizio 4).
*/

// Verifica se il sottoinsieme 'sol' (vettore booleano 0/1) è un Dominating Set
// Dominating Set: Ogni nodo del grafo o è in 'sol', oppure è adiacente a uno in 'sol'.
int checkDominatingSet(int *sol, struct Node** adj, int N) {
    
    for (int u = 0; u < N; u++) {
        // Se il nodo 'u' è già selezionato, è coperto.
        if (sol[u] == 1) continue;

        // Se non è selezionato, DEVE avere almeno un vicino selezionato.
        int covered = 0;
        struct Node* t = adj[u];
        while (t != NULL) {
            if (sol[t->dest] == 1) {
                covered = 1;
                break;
            }
            t = t->next;
        }
        
        if (!covered) return 0; // Nodo 'u' scoperto -> Fail
    }
    return 1;
}

