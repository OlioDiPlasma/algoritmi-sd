/* catalogo.c */
#include "catalogo.h"

/* Scopo: Inizializza il negozio azzerando il contatore categorie.
   Concetto: Passiamo il puntatore per modificare la variabile originale.
*/
void init_negozio(Negozio *n) {
    n->num_categorie = 0;
    // Per sicurezza puliamo la memoria (opzionale ma buona norma)
    for(int i=0; i<MAX_CAT; i++) {
        n->elenco[i].root = NULL;
        strcpy(n->elenco[i].id_cat, "");
    }
}

/* --- FUNZIONI DI SUPPORTO BST (Gestione Albero) --- */

/*
   Scopo: Inserisce un prodotto nell'albero mantenendo l'ordine per ID.
   Algoritmo: Ricorsivo.
   Ritorna: Il puntatore alla radice (eventualmente aggiornata).
*/
NodoProd* inserisci_in_bst(NodoProd *root, Prodotto p) {
    /* Caso base: siamo arrivati in una foglia o l'albero e' vuoto */
    if (root == NULL) {
        NodoProd *nuovo = (NodoProd*) malloc(sizeof(NodoProd));
        if (!nuovo) {
            printf("Errore allocazione memoria\n");
            exit(1);
        }
        nuovo->data = p;
        nuovo->left = NULL;
        nuovo->right = NULL;
        return nuovo;
    }

    /* Confronto lessicografico degli ID */
    int cmp = strcmp(p.id, root->data.id);

    if (cmp < 0) {
        /* L'ID e' minore, vado a sinistra */
        root->left = inserisci_in_bst(root->left, p);
    } else if (cmp > 0) {
        /* L'ID e' maggiore, vado a destra */
        root->right = inserisci_in_bst(root->right, p);
    } else {
        /* L'ID esiste gia': aggiorno la disponibilita' come da traccia (Punto 1) */
        root->data.disponibilita += p.disponibilita;
        /* Se fosse un aggiornamento prezzo, lo faremmo qui */
    }
    return root;
}

/*
   Scopo: Cerca un nodo per ID nell'albero (Complessita' Logaritmica).
   Ritorna: Puntatore al dato Prodotto se trovato, NULL altrimenti.
*/
Prodotto* cerca_in_bst(NodoProd *root, char *id) {
    if (root == NULL) return NULL;

    int cmp = strcmp(id, root->data.id);
    
    if (cmp == 0) return &(root->data); // Trovato
    if (cmp < 0) return cerca_in_bst(root->left, id); // Cerca a sx
    return cerca_in_bst(root->right, id); // Cerca a dx
}

/* --- PUNTO 1: INSERIMENTO E GESTIONE CATEGORIE --- */

/*
   Scopo: Gestisce la logica di inserimento nel Negozio.
   Logica: 
   1. Cerca se la categoria esiste gia'.
   2. Se non esiste, la crea (se c'e' spazio).
   3. Inserisce il prodotto nel BST della categoria corretta.
*/
void aggiungi_prodotto(Negozio *n, char *id, char *nome, float prezzo, int disp, char *cat) {
    
    int indice_cat = -1;

    /* Ricerca Lineare della categoria (come da specifica Punto 3: lineare sulle categorie) */
    for (int i = 0; i < n->num_categorie; i++) {
        if (strcmp(n->elenco[i].id_cat, cat) == 0) {
            indice_cat = i;
            break;
        }
    }

    /* Se la categoria non esiste, la creiamo */
    if (indice_cat == -1) {
        if (n->num_categorie >= MAX_CAT) {
            printf("Errore: Numero massimo categorie raggiunto.\n");
            return;
        }
        indice_cat = n->num_categorie;
        strcpy(n->elenco[indice_cat].id_cat, cat);
        n->elenco[indice_cat].root = NULL; // Inizializziamo l'albero vuoto
        n->num_categorie++;
    }

    /* Creiamo l'oggetto temporaneo */
    Prodotto p;
    strcpy(p.id, id);
    strcpy(p.nome, nome);
    p.prezzo = prezzo;
    p.disponibilita = disp;
    strcpy(p.categoria, cat);

    /* Inserimento nell'albero specifico di quella categoria */
    /* NOTA SINTASSI: Aggiorniamo la radice perche' se l'albero era vuoto ora cambia puntatore */
    n->elenco[indice_cat].root = inserisci_in_bst(n->elenco[indice_cat].root, p);
}

/* Funzione per caricare dati da file (NON richiesta esplicitamente ma necessaria per il test) */
void carica_da_file(Negozio *n, char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Impossibile aprire file prodotti.\n");
        return;
    }
    
    char id[COD_LEN], nome[MAX_STR], cat[COD_LEN];
    float prezzo;
    int disp;

    /* Leggiamo finche' ci sono righe valide */
    while(fscanf(f, "%s %s %f %d %s", id, nome, &prezzo, &disp, cat) == 5) {
        aggiungi_prodotto(n, id, nome, prezzo, disp, cat);
    }
    fclose(f);
    printf("Caricamento completato.\n");
}

/* --- PUNTO 2: RICERCA GLOBALE --- */

/*
   Scopo: Cerca un prodotto dato solo il codice.
   Logica: Non sapendo la categoria, dobbiamo provare a cercare in TUTTI gli alberi.
   Complessita': 100 * Log(N_prodotti_per_cat). E' accettabile.
*/
void ricerca_prodotto_globale(Negozio *n, char *id) {
    printf("--- Ricerca Globale ID: %s ---\n", id);
    for (int i = 0; i < n->num_categorie; i++) {
        Prodotto *p = cerca_in_bst(n->elenco[i].root, id);
        if (p != NULL) {
            printf("Trovato in cat '%s': %s, Prezzo: %.2f, Disp: %d\n", 
                   n->elenco[i].id_cat, p->nome, p->prezzo, p->disponibilita);
            return; // Trovato, esco
        }
    }
    printf("Prodotto non trovato.\n");
}

/* --- PUNTO 3: RICERCA MIRATA (CAT + ID) --- */

void ricerca_prodotto_mirata(Negozio *n, char *cat, char *id) {
    printf("--- Ricerca Mirata Cat: %s, ID: %s ---\n", cat, id);
    
    /* 1. Localizzare la categoria (Lineare) */
    int idx = -1;
    for (int i = 0; i < n->num_categorie; i++) {
        if (strcmp(n->elenco[i].id_cat, cat) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("Categoria inesistente.\n");
        return;
    }

    /* 2. Limitare ricerca ai prodotti della categoria (Logaritmica grazie al BST) */
    Prodotto *p = cerca_in_bst(n->elenco[idx].root, id);
    if (p) {
        printf("Prodotto trovato: %s, Prezzo: %.2f\n", p->nome, p->prezzo);
    } else {
        printf("Prodotto non trovato nella categoria specificata.\n");
    }
}

/* --- PUNTO 4: STAMPA CATEGORIA --- */

/* Helper: visita In-Order (Sinistra, Radice, Destra) -> Stampa ordinata per ID */
void stampa_inorder(NodoProd *root) {
    if (root == NULL) return;
    stampa_inorder(root->left);
    printf("ID: %s - Nome: %s\n", root->data.id, root->data.nome);
    stampa_inorder(root->right);
}

/* Struttura di supporto per ordinare per nome (array temporaneo) */
typedef struct {
    Prodotto *p;
} PtrProdotto;

int compare_nomi(const void *a, const void *b) {
    PtrProdotto *pa = (PtrProdotto *)a;
    PtrProdotto *pb = (PtrProdotto *)b;
    return strcmp(pa->p->nome, pb->p->nome);
}

/* Helper: Riempie array da albero per poi ordinare */
void albero_a_array(NodoProd *root, PtrProdotto *arr, int *k) {
    if (root == NULL) return;
    albero_a_array(root->left, arr, k);
    arr[*k].p = &(root->data);
    (*k)++;
    albero_a_array(root->right, arr, k);
}

void stampa_categoria(Negozio *n, char *cat, int ordina_per_nome) {
    printf("--- Stampa Categoria: %s ---\n", cat);
    int idx = -1;
    for(int i=0; i<n->num_categorie; i++) {
        if(strcmp(n->elenco[i].id_cat, cat) == 0) {
            idx = i;
            break;
        }
    }
    if (idx == -1) { printf("Categoria vuota o inesistente.\n"); return; }

    if (!ordina_per_nome) {
        /* Caso semplice: L'albero e' gia' ordinato per ID */
        stampa_inorder(n->elenco[idx].root);
    } else {
        /* Caso complesso: Dobbiamo linearizzare l'albero e usare qsort */
        /* Nota: per semplicita' usiamo un array statico grande, 
           in esame allocare dinamicamente contando prima i nodi e' meglio */
        PtrProdotto temp[1000]; 
        int count = 0;
        albero_a_array(n->elenco[idx].root, temp, &count);
        
        qsort(temp, count, sizeof(PtrProdotto), compare_nomi);
        
        for(int i=0; i<count; i++) {
            printf("Nome: %s - ID: %s\n", temp[i].p->nome, temp[i].p->id);
        }
    }
}

/* --- PUNTO 5: RICERCA PER NOME PARZIALE --- */

/* Helper ricorsivo che cerca in tutto l'albero */
void cerca_pattern_ricorsiva(NodoProd *root, char *base_name) {
    if (root == NULL) return;
    
    /* Visita tutto l'albero (Pre-Order) */
    /* strncmp controlla solo i primi N caratteri */
    if (strncmp(root->data.nome, base_name, strlen(base_name)) == 0) {
        printf("Trovato match: %s (ID: %s)\n", root->data.nome, root->data.id);
    }

    cerca_pattern_ricorsiva(root->left, base_name);
    cerca_pattern_ricorsiva(root->right, base_name);
}

void cerca_per_nome_parziale(Negozio *n, char *pattern) {
    /* Rimuoviamo l'asterisco se presente */
    char clean_pattern[MAX_STR];
    strcpy(clean_pattern, pattern);
    char *ptr = strchr(clean_pattern, '*');
    if (ptr) *ptr = '\0'; // Tronca la stringa all'asterisco

    printf("--- Ricerca pattern '%s*' ---\n", clean_pattern);
    
    // Dobbiamo cercare in TUTTE le categorie
    for(int i=0; i<n->num_categorie; i++) {
        cerca_pattern_ricorsiva(n->elenco[i].root, clean_pattern);
    }
}

/* --- PUNTO 6: VALUTAZIONE ORDINE --- */

typedef struct {
    char id[COD_LEN];
    int qta_richiesta;
    Prodotto *ptr_prod; // Puntatore al prodotto reale in memoria
} RigaOrdine;

void valuta_ordine(Negozio *n, char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { printf("Errore apertura file ordine.\n"); return; }

    int N;
    fscanf(f, "%d", &N); // Leggi numero righe
    
    /* Allocazione array dinamico per l'ordine */
    RigaOrdine *ordine = malloc(sizeof(RigaOrdine) * N);
    
    int fattibile = 1;
    float totale = 0.0;

    printf("--- Valutazione Ordine ---\n");

    /* FASE 1: Verifica Esistenza e Disponibilita' */
    for (int i=0; i<N; i++) {
        fscanf(f, "%s %d", ordine[i].id, &ordine[i].qta_richiesta);
        ordine[i].ptr_prod = NULL;

        // Cerchiamo il prodotto in tutte le categorie
        for (int c=0; c<n->num_categorie; c++) {
            Prodotto *p = cerca_in_bst(n->elenco[c].root, ordine[i].id);
            if (p) {
                ordine[i].ptr_prod = p;
                break;
            }
        }

        if (ordine[i].ptr_prod == NULL) {
            printf("Errore: Prodotto %s non esiste.\n", ordine[i].id);
            fattibile = 0;
        } else if (ordine[i].ptr_prod->disponibilita < ordine[i].qta_richiesta) {
            printf("Errore: Prodotto %s qta insufficiente (Richiesto: %d, Disp: %d).\n", 
                   ordine[i].id, ordine[i].qta_richiesta, ordine[i].ptr_prod->disponibilita);
            fattibile = 0;
        } else {
            totale += (ordine[i].ptr_prod->prezzo * ordine[i].qta_richiesta);
        }
    }

    /* FASE 2: Aggiornamento (Solo se tutto fattibile) */
    if (fattibile) {
        printf("Ordine Accettato! Totale: %.2f euro.\n", totale);
        for(int i=0; i<N; i++) {
            ordine[i].ptr_prod->disponibilita -= ordine[i].qta_richiesta;
            printf("Aggiornato %s: Nuova disp %d\n", ordine[i].id, ordine[i].ptr_prod->disponibilita);
        }
    } else {
        printf("Ordine Rifiutato: nessuna modifica apportata.\n");
    }

    free(ordine);
    fclose(f);
}

/* --- GESTIONE MEMORIA --- */

void libera_albero(NodoProd *root) {
    if (root == NULL) return;
    libera_albero(root->left);
    libera_albero(root->right);
    free(root);
}

void libera_tutto(Negozio *n) {
    for(int i=0; i<n->num_categorie; i++) {
        libera_albero(n->elenco[i].root);
    }
}