#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Definiamo una lunghezza massima per i nomi, come suggerito dal testo "MAXL"
#define MAXL 30 

/* * 1. DEFINIZIONE DEL TIPO DATE
 * Si tratta di un "Quasi ADT" perché la struct è esposta.
 * Serve a rappresentare la data di nascita (birth date).
 */
typedef struct {
    int dd; // Giorno
    int mm; // Mese
    int yy; // Anno
} Date;

/* * 2. DEFINIZIONE DEL TIPO KEY
 * Questa è la chiave che identifica univocamente i nodi nel grafo/tabella di simboli.
 * È composta da un nome (stringa) e dalla data di nascita.
 */
typedef struct {
    char name[MAXL]; // Nome della persona (chiave primaria di ordinamento)
    Date bd;         // Data di nascita (chiave secondaria per risolvere omonimie)
} Key;

/*
 * 3. FUNZIONE DI CONFRONTO DATE (DateCmp)
 * Confronta due date d1 e d2 in ordine cronologico.
 * Ritorna:
 * < 0 se d1 è precedente a d2
 * 0 se le date sono identiche
 * > 0 se d1 è successiva a d2
 */
int DateCmp(Date d1, Date d2) {
    // Primo livello: confronto gli anni
    if (d1.yy != d2.yy) 
        return d1.yy - d2.yy;
    
    // Secondo livello: se gli anni sono uguali, confronto i mesi
    if (d1.mm != d2.mm) 
        return d1.mm - d2.mm;
    
    // Terzo livello: se anche i mesi sono uguali, confronto i giorni
    return d1.dd - d2.dd;
}

/*
 * 4. FUNZIONE DI CONFRONTO CHIAVI (KEYcmp)
 * Implementa la logica richiesta: "si confrontano prima i nomi, poi le date".
 * Ritorna:
 * < 0 se k1 < k2
 * 0 se k1 == k2
 * > 0 se k1 > k2
 */
int KEYcmp(Key k1, Key k2) {
    // Confronto lessicografico tra i nomi
    int r = strcmp(k1.name, k2.name);
    
    // Se i nomi sono diversi (r != 0), il risultato dipende solo dal nome.
    // Ritorniamo direttamente r.
    if (r != 0) 
        return r;
    
    // Se i nomi sono uguali (omonimia), si usa la data di nascita per distinguere.
    return DateCmp(k1.bd, k2.bd);
}

/* * DEFINIZIONE STRUTTURA PARENTS
 * Poiché il grafo è rappresentato probabilmente con liste di adiacenza (v -> figli),
 * non abbiamo accesso diretto ai genitori. Questa struct serve a creare 
 * un "grafo inverso" o trasposto per risalire l'albero.
 * Si assume che ogni nodo abbia al massimo 2 genitori (es. albero genealogico).
 */
typedef struct parents {
    int p1; // Indice del primo genitore
    int p2; // Indice del secondo genitore
} par_t;

/*
 * FUNZIONE bfsK
 * Esegue una visita in ampiezza (BFS) per trovare se esiste un cammino
 * tra v1 e v2 entro una distanza k, rispettando il vincolo:
 * salita (facoltativa) -> discesa (facoltativa).
 * Non è ammesso: discesa -> salita.
 */
static int bfsK(Graph G, par_t *pArray, int v1, int v2, int k) {
    // Array delle distanze inizializzato a -1 (non visitato)
    int *dist = malloc(G->V * sizeof(int));
    // Array di stato: enParent[v] == 1 se posso ancora "salire" dai genitori di v
    // enParent[v] == 0 se sono in fase di "discesa" e posso solo andare ai figli
    int *enParent = calloc(G->V, sizeof(int)); 
    int res = 0; // Risultato (0 = falso, 1 = vero)
    int v;

    // Inizializzazione distanze e flag
    for (v = 0; v < G->V; v++) {
        dist[v] = -1;
    }

    // Inizializzazione Coda per BFS
    QUEUE q = QUEUEinit(G->V);
    
    // Setup nodo di partenza
    QUEUEput(q, v1);
    dist[v1] = 0;
    enParent[v1] = 1; // All'inizio posso salire

    while (!QUEUEempty(q)) {
        v = QUEUEget(q);

        // 1. Controllo Distanza: se ho superato k passi, interrompo questo ramo
        if (dist[v] > k) {
            break; 
        }

        // 2. Controllo Destinazione: se ho trovato v2, fine con successo
        if (v == v2) {
            res = 1;
            break; 
        }

        // 3. GESTIONE SALITA (verso i genitori)
        // Posso salire solo se il flag enParent[v] è attivo.
        if (enParent[v]) {
            // Recupero i genitori dal vettore pArray (costruito esternamente)
            int p1 = pArray[v].p1;
            int p2 = pArray[v].p2;

            // Controllo Genitore 1
            if (p1 >= 0 && dist[p1] < 0) { // Se esiste e non è visitato
                dist[p1] = dist[v] + 1;
                QUEUEput(q, p1);
                enParent[p1] = 1; // Continuando a salire, mantengo la possibilità di salire ancora
            }
            // Controllo Genitore 2
            if (p2 >= 0 && dist[p2] < 0) {
                dist[p2] = dist[v] + 1;
                QUEUEput(q, p2);
                enParent[p2] = 1; // Idem
            }
        }

        // 4. GESTIONE DISCESA (verso i figli)
        // Posso scendere sempre (sia che stia salendo, sia che stia già scendendo).
        // Ma una volta sceso, disabilito la possibilità di risalire per i nodi successivi.
        for (link t = G->ladj[v]; t != G->z; t = t->next) {
            if (dist[t->v] < 0) { // Se il figlio non è visitato
                dist[t->v] = dist[v] + 1;
                QUEUEput(q, t->v);
                
                // PUNTO CRUCIALE: Se vado verso un figlio, il prossimo nodo (t->v) 
                // NON potrà più risalire. Imposto enParent a 0.
                enParent[t->v] = 0; 
            }
        }
    }

    // Pulizia memoria
    QUEUEfree(q);
    free(dist);
    free(enParent);
    
    return res;
}

/*
 * FUNZIONE PRINCIPALE relationshipK
 * Prepara le strutture dati e invoca la BFS.
 */
int relationshipK(Graph G, char *name1, Date d1, char *name2, Date d2, int k) {
    // Allocazione dell'array per memorizzare i genitori per ogni nodo
    par_t *pArray = malloc(G->V * sizeof(*pArray));
    int v1, v2;
    Key k1, k2; // Strutture Key definite nell'esercizio precedente

    // Inizializzazione pArray a -1 (nessun genitore)
    for (int v = 0; v < G->V; v++) {
        pArray[v].p1 = pArray[v].p2 = -1;
    }

    /*
     * COSTRUZIONE DEL GRAFO INVERSO (Trasposto)
     * Il grafo G contiene liste di adiacenza del tipo: Padre -> Figli.
     * Noi dobbiamo riempire pArray che rappresenta: Figlio -> Padri.
     * Scorriamo tutto il grafo per popolare pArray.
     */
    for (int v = 0; v < G->V; v++) {
        // Per ogni nodo v, scorro i suoi figli t->v
        for (link t = G->ladj[v]; t != G->z; t = t->next) {
            struct parents *p = &pArray[t->v]; // Punto alla struct genitori del figlio
            
            // Assegno v come genitore del nodo t->v
            if (p->p1 == -1) 
                p->p1 = v;
            else 
                p->p2 = v;
        }
    }

    // Preparazione chiavi di ricerca
    strcpy(k1.name, name1); k1.bd = d1;
    strcpy(k2.name, name2); k2.bd = d2;

    // Ricerca degli indici numerici associati ai nomi nella Tabella di Simboli
    v1 = STsearch(G->tab, k1);
    v2 = STsearch(G->tab, k2);

    // Chiamata alla funzione core
    int res = bfsK(G, pArray, v1, v2, k);

    // Pulizia
    free(pArray);
    
    return res;
}

/* * FUNZIONE AUSILIARIA: bfsDownCount
 * Conta quanti nodi sono raggiungibili da v0 entro una distanza k.
 * Utilizza una BFS (Breadth-First Search) per esplorare il grafo "in discesa" (verso i figli).
 * * Parametri:
 * - G: Il grafo (albero genealogico o simile)
 * - v0: Il nodo di partenza (l'antenato di cui contare i discendenti)
 * - k: La distanza massima (grado di parentela)
 */
static int bfsDownCount(Graph G, int v0, int k) {
    // Array per tracciare le distanze e i nodi visitati.
    // Inizializzato a -1 (non visitato).
    // NOTA: Nell'immagine usa un VLA (int dist[G->V]), qui usiamo malloc per sicurezza.
    int *dist = malloc(G->V * sizeof(int));
    int cnt = 0; // Contatore dei discendenti trovati
    int v;

    // Inizializzazione array distanze
    for (v = 0; v < G->V; v++) {
        dist[v] = -1;
    }

    // Inizializzazione della Coda per la BFS
    QUEUE q = QUEUEinit(G->V);

    // Inserimento del nodo di partenza
    QUEUEput(q, v0);
    dist[v0] = 0; // Distanza 0 da se stesso

    while (!QUEUEempty(q)) {
        v = QUEUEget(q);

        // Se la distanza del nodo corrente supera k, interrompiamo l'esplorazione di questo ramo.
        // Poiché è una BFS, se troviamo un nodo a dist > k, anche i successivi nella coda
        // avranno dist >= k (proprietà della BFS), ma il codice controlla puntualmente.
        if (dist[v] > k) {
            break; 
        }

        // Incrementiamo il contatore.
        // NOTA: Questo conta anche il nodo radice v0. Poiché lo fanno tutti i nodi,
        // il confronto per il massimo rimane valido (tutti hanno +1).
        cnt++;

        // Esplorazione dei figli (adiacenti)
        for (link t = G->ladj[v]; t != G->z; t = t->next) {
            // Se il figlio non è stato ancora visitato
            if (dist[t->v] < 0) {
                dist[t->v] = dist[v] + 1; // Aggiorna distanza
                QUEUEput(q, t->v);       // Accoda per visita successiva
            }
        }
    }

    QUEUEfree(q);
    free(dist);
    
    return cnt;
}

/*
 * FUNZIONE PRINCIPALE: largestLineageK
 * Trova e stampa la persona con il maggior numero di discendenti di grado <= k.
 */
void largestLineageK(Graph G, int k) {
    int max = -1;      // Massimo numero di discendenti trovato finora
    int vmax = -1;     // Indice del nodo che ha il massimo
    int cnt_v;         // Variabile temporanea per il conteggio corrente

    // Iteriamo su TUTTI i nodi del grafo.
    // Per ogni persona, calcoliamo quanti discendenti ha lanciando una BFS dedicata.
    for (int v = 0; v < G->V; v++) {
        
        // Calcola discendenti per il nodo v
        cnt_v = bfsDownCount(G, v, k);
        
        // Aggiornamento del massimo
        if (cnt_v > max) {
            max = cnt_v;
            vmax = v;
        }
    }

    // Stampa del risultato
    if (vmax != -1) {
        // Recuperiamo i dati della persona dalla Tabella di Simboli usando l'indice vmax
        // Si assume che l'ADT fornisca una funzione STsearchByIndex o simile che ritorni la Chiave/Item
        Key k_max = STsearchByIndex(G->tab, vmax);
        
        printf("La persona con più discendenza di grado %d è %s\n", k, k_max.name);
        printf("Nata il %d/%d/%d\n", k_max.bd.dd, k_max.bd.mm, k_max.bd.yy);
    }
}
