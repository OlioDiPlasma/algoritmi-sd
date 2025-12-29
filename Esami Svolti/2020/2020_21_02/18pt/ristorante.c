#include "ristorante.h"

/* --- 1. CARICAMENTO DATI --- */

/*
 * Scopo: Leggere il file di testo e caricare i piatti in un vettore dinamico.
 * File di input: "piatti.txt" strutturato con N nella prima riga, poi i dati.
 * Parametri:
 * - filename: nome del file.
 * - N: puntatore a intero per "restituire" al main il numero di piatti letti.
 * Concetti Chiave:
 * - Malloc: Dobbiamo allocare il vettore a runtime perché non conosciamo N a priori.
 */
Piatto* leggiPiatti(char *filename, int *N) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore apertura file %s\n", filename);
        exit(1);
    }

    // Leggo la prima riga per sapere quanti piatti ci sono
    fscanf(fp, "%d", N);

    // ALLOCAZIONE VETTORE
    // sizeof(Piatto) * (*N): Calcolo i byte totali necessari.
    Piatto *vettore = (Piatto*)malloc(sizeof(Piatto) * (*N));
    if (vettore == NULL) {
        printf("Errore malloc\n");
        exit(1);
    }

    // Lettura dei record
    for (int i = 0; i < *N; i++) {
        // %s si ferma al primo spazio bianco, perfetto per il formato richiesto
        fscanf(fp, "%s %s %s %f", 
               vettore[i].nome, 
               vettore[i].portata, 
               vettore[i].tipologia, 
               &vettore[i].costo);
    }

    fclose(fp);
    return vettore;
}

/* --- 2. LOGICA DI CONFRONTO (CORE) --- */

/*
 * Scopo: Confrontare due Menu secondo le regole complesse del testo.
 * Regole:
 * 1. Prezzo minore vince.
 * 2. A parità di prezzo, confronto lessicografico degli indici dei piatti.
 * Parametri:
 * - m1, m2: i due menu da confrontare.
 * - elenco: serve solo se dovessimo accedere ai nomi, ma qui bastano gli indici 
 * perché il testo dice "Un piatto x è minore di y se compare prima nel file".
 * Quindi basta confrontare gli indici numerici!
 * Ritorna:
 * < 0 se m1 < m2
 * > 0 se m1 > m2
 * 0 se uguali
 */
int MENUcompare(Menu m1, Menu m2, Piatto *elenco) {
    // Criterio 1: Prezzo
    // Usiamo una piccola soglia (epsilon) per i float, o confronto diretto se semplice
    if (m1.costo_tot < m2.costo_tot) return -1;
    if (m1.costo_tot > m2.costo_tot) return 1;

    // Criterio 2: Ordine dei piatti (Lessicografico sugli indici)
    // Siccome generiamo combinazioni ordinate, gli indici dentro m1.indici sono già ordinati
    // dal più piccolo al più grande. Li confrontiamo posizione per posizione.
    for (int i = 0; i < m1.n_piatti; i++) {
        if (m1.indici[i] < m2.indici[i]) return -1;
        if (m1.indici[i] > m2.indici[i]) return 1;
    }

    return 0;
}

/* --- 3. GESTIONE BST (LIBRERIA) --- */

BSTNode* bstInsert(BSTNode *root, Menu m, Piatto *elenco) {
    // Caso base: albero vuoto (o foglia raggiunta), creo il nodo
    if (root == NULL) {
        BSTNode *nuovo = (BSTNode*)malloc(sizeof(BSTNode));
        nuovo->val = m; // Copia la struttura (shallow copy dei puntatori, ma profonda dei valori int/float)
        
        // ATTENZIONE: m.indici è un puntatore. Se copio solo il puntatore, 
        // tutti i nodi punteranno allo stesso vettore temporaneo della ricorsione!
        // Devo duplicare l'array degli indici per questo nodo specifico.
        nuovo->val.indici = (int*)malloc(sizeof(int) * m.n_piatti);
        for(int k=0; k<m.n_piatti; k++) {
            nuovo->val.indici[k] = m.indici[k];
        }

        nuovo->left = NULL;
        nuovo->right = NULL;
        return nuovo;
    }

    // Navigazione per trovare la posizione
    if (MENUcompare(m, root->val, elenco) < 0) {
        root->left = bstInsert(root->left, m, elenco);
    } else {
        // Se è maggiore o uguale, va a destra
        root->right = bstInsert(root->right, m, elenco);
    }
    return root;
}

/* --- 4. ALGORITMO RICORSIVO (COMBINAZIONI CON RIPETIZIONE LIMITATA) --- */

/*
 * Scopo: Funzione ricorsiva reale (Modello del Calcolo Combinatorio).
 * Tipo: Combinazioni con ripetizione (l'ordine non conta, possiamo riprendere lo stesso elemento).
 * Vincolo "Bis": Un piatto può comparire max 2 volte.
 * Parametri:
 * - pos: livello della ricorsione (da 0 a P).
 * - start: indice del vettore piatti da cui iniziare a cercare (per evitare permutazioni).
 * - sol: vettore temporaneo degli indici scelti finora.
 * - current_cost: costo accumulato.
 * - elenco, N, P: dati del problema.
 * - root: doppio puntatore alla radice del BST (per inserire man mano).
 * - conteggi: array di appoggio per contare quante volte ho preso un piatto (per gestire il "bis").
 */
void r_generate(int pos, int start, int *sol, float current_cost, 
                Piatto *elenco, int N, int P, BSTNode **root, int *conteggi) {
    
    // Base Case: Abbiamo riempito il menu con P piatti
    if (pos == P) {
        Menu m;
        m.n_piatti = P;
        m.indici = sol; // Passiamo il vettore temporaneo (verrà copiato nella insert)
        m.costo_tot = current_cost;
        
        // Inserimento nel BST (passo il valore di root dereferenziato)
        *root = bstInsert(*root, m, elenco);
        return;
    }

    // Iterazione
    // i parte da 'start' perché l'ordine non conta (Combinazioni) -> 1,2 è uguale a 2,1.
    // Imponendo i >= start evitiamo 2,1 se abbiamo già fatto 1,2.
    for (int i = start; i < N; i++) {
        
        // Gestione Vincolo "Bis": Possiamo prendere il piatto 'i' solo se l'abbiamo preso meno di 2 volte.
        if (conteggi[i] < 2) {
            
            // PRENDO
            sol[pos] = i;
            conteggi[i]++; // Segno che l'ho preso
            
            // RICORSIONE
            // Passo 'i' come nuovo start, NON 'i+1', perché posso riprendere lo stesso piatto (ripetizione)
            r_generate(pos + 1, i, sol, current_cost + elenco[i].costo, elenco, N, P, root, conteggi);
            
            // BACKTRACKING (LASCIO)
            // Fondamentale: ripristino lo stato per la prossima iterazione del for
            conteggi[i]--; 
        }
    }
}

// Wrapper che inizializza le strutture per la ricorsione
BSTNode* generaMenu(Piatto *elenco, int N, int P) {
    BSTNode *root = NULL;
    
    // Allocazione vettore soluzione temporaneo
    int *sol = (int*)malloc(sizeof(int) * P);
    
    // Allocazione vettore conteggi (calloc inizializza a 0)
    // Serve per tracciare il vincolo "bis"
    int *conteggi = (int*)calloc(N, sizeof(int));

    // Avvio ricorsione
    // pos=0, start=0, cost=0
    r_generate(0, 0, sol, 0.0, elenco, N, P, &root, conteggi);

    free(sol);
    free(conteggi);
    return root;
}

/* --- 5. VISITA E STAMPA --- */

/*
 * Scopo: Attraversare il BST in-order (Sinistra -> Centro -> Destra).
 * Risultato: I menu verranno stampati ordinati dal "più piccolo" al "più grande"
 * secondo la logica di MENUcompare.
 */
void stampaBST(BSTNode *root, Piatto *elenco) {
    if (root == NULL) return;

    // 1. Visita figlio sinistro (i minori)
    stampaBST(root->left, elenco);

    // 2. Elaborazione nodo corrente
    printf("MENU (Costo: %.2f):\n", root->val.costo_tot);
    for (int i = 0; i < root->val.n_piatti; i++) {
        int idx = root->val.indici[i];
        // Stampo la quaterna come richiesto
        printf("\t%s %s %s %.2f\n", 
               elenco[idx].nome, 
               elenco[idx].portata, 
               elenco[idx].tipologia, 
               elenco[idx].costo);
    }
    printf("----------------------------------\n");

    // 3. Visita figlio destro (i maggiori)
    stampaBST(root->right, elenco);
}

void freeBST(BSTNode *root) {
    if (root == NULL) return;
    freeBST(root->left);
    freeBST(root->right);
    // Libero il vettore indici dentro il menu
    free(root->val.indici); 
    // Libero il nodo
    free(root);
}