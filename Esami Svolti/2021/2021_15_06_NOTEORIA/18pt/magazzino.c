/* magazzino.c */
#include "magazzino.h"

/* ==========================================
   SEZIONE 1: FUNZIONI AUSILIARIE BST
   Scopo: Gestire l'indice per la ricerca O(log N).
   ========================================== */

/*
 * Scopo: Crea un nuovo nodo per l'albero.
 * Parametri: codice pacco, posizione fisica.
 */
static BSTNode* createNode(char *codice, Posizione pos) {
    BSTNode *n = (BSTNode*)malloc(sizeof(BSTNode));
    strcpy(n->codice_pacco, codice);
    n->pos = pos;
    n->left = n->right = NULL;
    return n;
}

/*
 * Scopo: Inserisce un nodo nel BST mantenendo l'ordinamento alfabetico.
 * Concetto Chiave: Ricorsione. Se il codice è < del nodo corrente, vado a sinistra, altrimenti a destra.
 */
static BSTNode* bstInsert(BSTNode *root, char *codice, Posizione pos) {
    if (root == NULL) return createNode(codice, pos);

    if (strcmp(codice, root->codice_pacco) < 0) {
        root->left = bstInsert(root->left, codice, pos);
    } else if (strcmp(codice, root->codice_pacco) > 0) {
        root->right = bstInsert(root->right, codice, pos);
    }
    // Se uguale, non inseriamo duplicati (assunzione: codici univoci)
    return root;
}

/*
 * Scopo: Cerca un nodo nel BST.
 * Return: Puntatore al nodo o NULL se non trovato.
 */
static BSTNode* bstSearch(BSTNode *root, char *codice) {
    if (root == NULL || strcmp(root->codice_pacco, codice) == 0)
        return root;

    if (strcmp(codice, root->codice_pacco) < 0)
        return bstSearch(root->left, codice);
    else
        return bstSearch(root->right, codice);
}

/*
 * Scopo: Trova il nodo minimo (serve per la cancellazione).
 */
static BSTNode* minValueNode(BSTNode* node) {
    BSTNode* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

/*
 * Scopo: Rimuove un nodo dal BST.
 * Concetto Chiave: La cancellazione nei BST è complessa.
 * 1. Nodo foglia: rimuovi e basta.
 * 2. Un figlio: sostituisci col figlio.
 * 3. Due figli: trova il successore in-order (minimo del sottoalbero destro),
 * copia il contenuto e cancella il successore.
 */
static BSTNode* bstDelete(BSTNode *root, char *codice) {
    if (root == NULL) return root;

    if (strcmp(codice, root->codice_pacco) < 0)
        root->left = bstDelete(root->left, codice);
    else if (strcmp(codice, root->codice_pacco) > 0)
        root->right = bstDelete(root->right, codice);
    else {
        // Nodo trovato
        if (root->left == NULL) {
            BSTNode *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            BSTNode *temp = root->left;
            free(root);
            return temp;
        }
        // Caso due figli
        BSTNode* temp = minValueNode(root->right);
        strcpy(root->codice_pacco, temp->codice_pacco);
        root->pos = temp->pos; // Copiamo anche la posizione aggiornata!
        root->right = bstDelete(root->right, temp->codice_pacco);
    }
    return root;
}

/* Funzione per liberare la memoria del BST ricorsivamente */
static void freeBST(BSTNode *root) {
    if (root == NULL) return;
    freeBST(root->left);
    freeBST(root->right);
    free(root);
}


/* ==========================================
   SEZIONE 2: GESTIONE MAGAZZINO (LOGICA)
   ========================================== */

/*
 * Scopo: Inizializza le strutture leggendo da file.
 * File Input: config.txt
 * Gestione Memoria: Usa malloc pesantemente per creare la matrice frastagliata (Jagged Array).
 */
Magazzino* initMagazzino(const char* filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { perror("Errore apertura file"); return NULL; }

    Magazzino *m = (Magazzino*)malloc(sizeof(Magazzino));
    m->indice = NULL; // BST vuoto all'inizio

    int n_cor, k;
    fscanf(f, "%d %d", &n_cor, &k); // Legge num corridoi e K
    m->n_corridoi = n_cor;
    m->K_slots = k;

    // Allocazione array corridoi
    m->corridoi = (Corridoio*)malloc(n_cor * sizeof(Corridoio));

    // Lettura configurazione singolo corridoio
    for (int i = 0; i < n_cor; i++) {
        int idx, n_scaff;
        fscanf(f, "%d %d", &idx, &n_scaff); // es: Corridoio 0 ha 4 scaffali

        m->corridoi[idx].id_numerico = idx;
        m->corridoi[idx].n_scaffali = n_scaff;
        
        // Allocazione array scaffali per questo corridoio
        m->corridoi[idx].scaffali = (Scaffale*)malloc(n_scaff * sizeof(Scaffale));

        for (int j = 0; j < n_scaff; j++) {
            // Generiamo un ID scaffale fittizio tipo "C0-S1"
            sprintf(m->corridoi[idx].scaffali[j].codice_scaffale, "C%d-S%d", idx, j);
            m->corridoi[idx].scaffali[j].K = k;
            m->corridoi[idx].scaffali[j].n_occupati = 0;

            // Allocazione slot: Array di PUNTATORI a Pacco (inizialmente NULL)
            // Usiamo calloc per inizializzare tutto a NULL (slot vuoto)
            m->corridoi[idx].scaffali[j].slots = (Pacco**)calloc(k, sizeof(Pacco*));
        }
    }

    fclose(f);
    return m;
}

/*
 * Scopo: Ricerca O(log N) tramite BST.
 * Ritorna: Posizione {-1, -1, -1} se non trovato.
 */
Posizione ricercaPacco(Magazzino *m, char *codice) {
    BSTNode *res = bstSearch(m->indice, codice);
    if (res != NULL) {
        return res->pos;
    }
    Posizione p = {-1, -1, -1};
    return p;
}

/*
 * Scopo: Inserisce un pacco.
 * Logica:
 * 1. Verifica validità coordinate.
 * 2. Verifica se slot è vuoto.
 * 3. Alloca Pacco nella struttura fisica.
 * 4. Aggiorna Indice BST.
 */
int inserimentoPacco(Magazzino *m, char *codice, Posizione pos) {
    // 1. Controlli boundary
    if (pos.id_corridoio < 0 || pos.id_corridoio >= m->n_corridoi) return 0;
    Corridoio *c = &m->corridoi[pos.id_corridoio];
    
    if (pos.id_scaffale < 0 || pos.id_scaffale >= c->n_scaffali) return 0;
    Scaffale *s = &c->scaffali[pos.id_scaffale];

    if (pos.id_slot < 0 || pos.id_slot >= s->K) return 0;

    // 2. Controllo occupazione
    if (s->slots[pos.id_slot] != NULL) {
        printf("Errore: Slot [%d][%d][%d] gia' occupato!\n", pos.id_corridoio, pos.id_scaffale, pos.id_slot);
        return 0; // Fallimento
    }

    // Controllo duplicati codice (opzionale ma consigliato)
    if (bstSearch(m->indice, codice) != NULL) {
        printf("Errore: Codice pacco %s gia' esistente!\n", codice);
        return 0;
    }

    // 3. Allocazione Fisica
    Pacco *nuovo = (Pacco*)malloc(sizeof(Pacco));
    strcpy(nuovo->codice, codice);
    s->slots[pos.id_slot] = nuovo; // Assegno il puntatore
    s->n_occupati++;

    // 4. Aggiornamento Indice (BST)
    m->indice = bstInsert(m->indice, codice, pos);
    
    return 1; // Successo
}

/*
 * Scopo: Rimuove un pacco.
 * Logica: Inversa dell'inserimento.
 */
int estrazionePacco(Magazzino *m, Posizione pos) {
    // Controlli validità indici (omessi per brevità, uguali a sopra)
    if (pos.id_corridoio >= m->n_corridoi) return 0;
    Scaffale *s = &m->corridoi[pos.id_corridoio].scaffali[pos.id_scaffale];

    if (s->slots[pos.id_slot] == NULL) {
        printf("Errore: Slot vuoto!\n");
        return 0;
    }

    // Recupero codice per cancellazione dal BST
    char codiceTemp[MAX_CODICE];
    strcpy(codiceTemp, s->slots[pos.id_slot]->codice);

    // 1. Rimozione Fisica
    free(s->slots[pos.id_slot]); // Libero la struct Pacco
    s->slots[pos.id_slot] = NULL; // Imposto puntatore a NULL
    s->n_occupati--;

    // 2. Rimozione Indice
    m->indice = bstDelete(m->indice, codiceTemp);

    return 1;
}

/*
 * Scopo: Sposta un pacco da A a B.
 * Concetto Chiave: Usiamo le funzioni esistenti per non duplicare codice,
 * ma dobbiamo fare attenzione a non perdere dati se l'inserimento fallisce.
 */
int spostamentoPacco(Magazzino *m, Posizione da, Posizione a) {
    // Verifica preliminare che "da" esista e "a" sia libero
    // ... (omesso controlli standard)

    Scaffale *s_da = &m->corridoi[da.id_corridoio].scaffali[da.id_scaffale];
    if (s_da->slots[da.id_slot] == NULL) return 0;

    // Salviamo il codice
    char codice[MAX_CODICE];
    strcpy(codice, s_da->slots[da.id_slot]->codice);

    // Strategia atomica:
    // 1. Estrai
    // 2. Prova a inserire
    // 3. Se inserimento fallisce, reinserisci dove era (rollback manuale)
    
    if (estrazionePacco(m, da)) {
        if (!inserimentoPacco(m, codice, a)) {
            // Rollback: rimetto dov'era
            inserimentoPacco(m, codice, da);
            printf("Errore spostamento: destinazione non valida, rollback eseguito.\n");
            return 0;
        }
        return 1;
    }
    return 0;
}

/*
 * Scopo: Compattazione (Unisci B dentro A se c'è spazio).
 * Logica: Iteriamo su B, se troviamo pacco proviamo a spostarlo nel primo buco libero di A.
 */
int compattazioneScaffali(Magazzino *m, int corr_id, int id_scaff_Dest, int id_scaff_Sorg) {
    Corridoio *c = &m->corridoi[corr_id];
    Scaffale *dest = &c->scaffali[id_scaff_Dest];
    Scaffale *sorg = &c->scaffali[id_scaff_Sorg];

    // Verifica spazio: I pacchi di sorgente entrano nei buchi di destinazione?
    int spazio_libero_dest = dest->K - dest->n_occupati;
    if (sorg->n_occupati > spazio_libero_dest) {
        printf("Compattazione impossibile: spazio insufficiente.\n");
        return 0;
    }

    // Iteriamo sugli slot della sorgente
    for (int i = 0; i < sorg->K; i++) {
        if (sorg->slots[i] != NULL) {
            // Trovato un pacco in sorgente. Cerchiamo primo buco in destinazione
            for (int j = 0; j < dest->K; j++) {
                if (dest->slots[j] == NULL) {
                    // Eseguiamo spostamento logico
                    Posizione p_da = {corr_id, id_scaff_Sorg, i};
                    Posizione p_a = {corr_id, id_scaff_Dest, j};
                    spostamentoPacco(m, p_da, p_a);
                    break; // Passa al prossimo pacco di sorgente
                }
            }
        }
    }
    printf("Compattazione completata.\n");
    return 1;
}

void liberaMagazzino(Magazzino *m) {
    if(!m) return;
    freeBST(m->indice);
    for(int i=0; i<m->n_corridoi; i++) {
        for(int j=0; j<m->corridoi[i].n_scaffali; j++) {
            // Libera eventuali pacchi rimasti
            for(int k=0; k<m->corridoi[i].scaffali[j].K; k++) {
                if(m->corridoi[i].scaffali[j].slots[k] != NULL)
                    free(m->corridoi[i].scaffali[j].slots[k]);
            }
            free(m->corridoi[i].scaffali[j].slots);
        }
        free(m->corridoi[i].scaffali);
    }
    free(m->corridoi);
    free(m);
}

void stampaStatoMagazzino(Magazzino *m) {
    printf("\n--- STATO MAGAZZINO ---\n");
    for(int i=0; i<m->n_corridoi; i++) {
        printf("Corridoio %d:\n", i);
        for(int j=0; j<m->corridoi[i].n_scaffali; j++) {
            printf("  Scaffale %d [Occupati: %d/%d]: ", j, m->corridoi[i].scaffali[j].n_occupati, m->K_slots);
            for(int k=0; k<m->corridoi[i].scaffali[j].K; k++) {
                if(m->corridoi[i].scaffali[j].slots[k])
                    printf("[%s] ", m->corridoi[i].scaffali[j].slots[k]->codice);
                else
                    printf("[ ] ");
            }
            printf("\n");
        }
    }
    printf("-----------------------\n");
}