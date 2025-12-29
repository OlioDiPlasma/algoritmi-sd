#include "grafo.h"

/* --- FUNZIONI DI UTILITA' INTERNE (static) --- */

// Trova l'indice di un vertice dato il nome.
// Concetto Chiave: Linear Search. Non efficiente per milioni di dati, ma ok per esami.
static int trovaIndice(Grafo *g, char *nome) {
    for (int i = 0; i < g->nV; i++) {
        if (strcmp(g->vettVertici[i].id, nome) == 0) return i;
    }
    return -1;
}

// Aggiunge un arco alla lista di adiacenza
static void aggiungiArcoLista(Grafo *g, int u, int v) {
    /* --- GESTIONE MEMORIA --- */
    // Malloc: allochiamo un piccolo nodo per la lista concatenata.
    // sizeof(NodoAdj) ci dà la dimensione esatta della struct.
    NodoAdj *nuovo = (NodoAdj*)malloc(sizeof(NodoAdj));
    if (nuovo == NULL) {
        printf("Errore malloc lista\n"); exit(1);
    }
    nuovo->v_index = v;
    // Inserimento in testa: O(1)
    nuovo->next = g->vettVertici[u].head;
    g->vettVertici[u].head = nuovo;
}

/* =========================================================================
   SEZIONE 1: LETTURA E GESTIONE MEMORIA
   ========================================================================= */

/*
 * Scopo: Leggere il file, allocare il grafo e costruire le liste di adiacenza.
 * Parametri: filename (nome del file)
 * Concetti Chiave: 
 * - Doppia passata (o allocazione dinamica): Qui usiamo realloc per gli archi 
 * perché il testo dice "numero indefinito di coppie".
 */
Grafo* leggiGrafo(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore apertura file %s\n", filename);
        return NULL;
    }

    Grafo *g = (Grafo*)malloc(sizeof(Grafo));
    
    // 1. Leggi numero vertici
    fscanf(fp, "%d", &g->nV);
    
    // Allocazione array vertici
    g->vettVertici = (Vertice*)malloc(g->nV * sizeof(Vertice));
    
    // 2. Leggi nomi vertici
    for (int i = 0; i < g->nV; i++) {
        fscanf(fp, "%s", g->vettVertici[i].id);
        g->vettVertici[i].grado = 0;
        g->vettVertici[i].grado_originale = 0;
        g->vettVertici[i].attivo = 1; // Inizialmente tutti attivi
        g->vettVertici[i].head = NULL;
    }

    // 3. Leggi gli archi (numero indefinito)
    // Allocazione iniziale per gli archi (iniziamo con 10, poi espandiamo)
    int cap = 10; 
    g->nE = 0;
    g->vettArchi = (Arco*)malloc(cap * sizeof(Arco));

    char nome1[11], nome2[11];
    while (fscanf(fp, "%s %s", nome1, nome2) == 2) {
        int u = trovaIndice(g, nome1);
        int v = trovaIndice(g, nome2);

        if (u != -1 && v != -1) {
            // Aggiungi alla lista di adiacenza (grafo non orientato -> ambo i lati)
            aggiungiArcoLista(g, u, v);
            aggiungiArcoLista(g, v, u);
            
            // Incrementa gradi
            g->vettVertici[u].grado++;
            g->vettVertici[v].grado++;
            
            // Salva arco nell'array lineare (serve per il punto 3)
            if (g->nE == cap) {
                cap *= 2; // Raddoppia capacità
                g->vettArchi = (Arco*)realloc(g->vettArchi, cap * sizeof(Arco));
            }
            g->vettArchi[g->nE].u = u;
            g->vettArchi[g->nE].v = v;
            g->vettArchi[g->nE].attivo = 1;
            g->nE++;
        }
    }

    // Aggiorno i gradi originali
    for(int i=0; i<g->nV; i++) 
        g->vettVertici[i].grado_originale = g->vettVertici[i].grado;

    fclose(fp);
    return g;
}

void liberaGrafo(Grafo *g) {
    if (g == NULL) return;
    for (int i = 0; i < g->nV; i++) {
        NodoAdj *curr = g->vettVertici[i].head;
        while (curr != NULL) {
            NodoAdj *temp = curr;
            curr = curr->next;
            free(temp); // Importante: libera ogni nodo della lista
        }
    }
    free(g->vettVertici);
    free(g->vettArchi);
    free(g);
}

void stampaGrafo(Grafo *g) {
    printf("\n--- STATO GRAFO (Vertici Attivi) ---\n");
    for(int i=0; i<g->nV; i++) {
        if(g->vettVertici[i].attivo) {
            printf("%s (Grado: %d): ", g->vettVertici[i].id, g->vettVertici[i].grado);
            NodoAdj *p = g->vettVertici[i].head;
            while(p) {
                // Controllo se anche il vicino è attivo
                if(g->vettVertici[p->v_index].attivo)
                    printf("%s ", g->vettVertici[p->v_index].id);
                p = p->next;
            }
            printf("\n");
        }
    }
    printf("------------------------------------\n");
}

/* =========================================================================
   SEZIONE 2: PUNTO 2 - K-CORE
   ========================================================================= */

/*
 * Scopo: Calcolare il k-core eliminando iterativamente i vertici con grado < k.
 * Algoritmo:
 * 1. Scansiona tutti i vertici attivi.
 * 2. Se trovi un vertice con grado < k:
 * - Disattivalo (attivo = 0).
 * - Visita i suoi vicini e diminuisci il loro grado.
 * - Segna che c'è stato un cambiamento (flag mod).
 * 3. Ripeti finché in un ciclo completo non elimini nessuno.
 */
void calcolaKCore(Grafo *g, int k) {
    printf("\n>>> Calcolo %d-CORE...\n", k);
    int modificato = 1;

    while (modificato) {
        modificato = 0;
        for (int i = 0; i < g->nV; i++) {
            // Analizzo solo se è ancora vivo e ha grado insufficiente
            if (g->vettVertici[i].attivo && g->vettVertici[i].grado < k) {
                
                // Rimuovo "logicamente" il vertice
                g->vettVertici[i].attivo = 0;
                modificato = 1; // Ho fatto una modifica, dovrò ricontrollare tutto
                // printf("  -> Rimosso %s (grado %d)\n", g->vettVertici[i].id, g->vettVertici[i].grado);

                // Aggiorno i gradi dei vicini
                NodoAdj *curr = g->vettVertici[i].head;
                while (curr != NULL) {
                    int vicinoIdx = curr->v_index;
                    // Se il vicino era attivo, riduco il suo grado
                    if (g->vettVertici[vicinoIdx].attivo) {
                        g->vettVertici[vicinoIdx].grado--;
                    }
                    curr = curr->next;
                }
            }
        }
    }

    // Output richiesto
    printf("Vertici appartenenti al %d-core: ", k);
    int count = 0;
    for(int i=0; i<g->nV; i++) {
        if(g->vettVertici[i].attivo) {
            printf("%s ", g->vettVertici[i].id);
            count++;
        }
    }
    if (count == 0) printf("(Nessuno)");
    printf("\n");
}

/* =========================================================================
   SEZIONE 3: PUNTO 3 - J-EDGE-CONNECTED
   ========================================================================= */

// Helper: DFS per contare nodi raggiungibili
void DFS(Grafo *g, int u, int *visited, int *count) {
    visited[u] = 1;
    (*count)++;
    
    NodoAdj *curr = g->vettVertici[u].head;
    while(curr != NULL) {
        int v = curr->v_index;
        // Posso attraversare l'arco solo se NON è "spento" dalla nostra procedura di test
        // Per fare questo controllo in modo efficiente, dobbiamo sapere se l'arco (u,v) è attivo.
        // Poiché la lista adiacenza non ha puntatore diretto all'array archi, facciamo una verifica inversa:
        // Controlliamo l'array vettArchi globale. 
        // Nota: Questo è O(E) per ogni arco nella DFS, lento ma sicuro per l'esame.
        // Ottimizzazione possibile: flag nell'arco della lista adiacenza, ma richiede strutture più complesse.
        
        int arco_attivo = 1;
        for(int k=0; k<g->nE; k++) {
            if ( ((g->vettArchi[k].u == u && g->vettArchi[k].v == v) || 
                  (g->vettArchi[k].u == v && g->vettArchi[k].v == u)) && 
                  g->vettArchi[k].attivo == 0 ) {
                arco_attivo = 0;
                break;
            }
        }

        if (arco_attivo && !visited[v]) {
            DFS(g, v, visited, count);
        }
        curr = curr->next;
    }
}

// Helper: Verifica se il grafo è connesso (ignorando archi disattivati)
int isConnected(Grafo *g) {
    if (g->nV == 0) return 1;
    
    int *visited = (int*)calloc(g->nV, sizeof(int));
    int count = 0;
    
    // Parto dal vertice 0 (assumendo esista e faccia parte del componente)
    // Attenzione: se il vertice 0 è stato rimosso dal k-core precedente, questo codice fallirebbe.
    // MA: Il punto 3 è indipendente. Assumiamo di lavorare sul grafo originale o che la funzione chiamante
    // abbia ripristinato lo stato.
    DFS(g, 0, visited, &count);
    
    free(visited);
    return (count == g->nV);
}

// Funzione ricorsiva per generare combinazioni di archi da rimuovere
// depth: quanti archi ho rimosso finora
// target: quanti archi voglio rimuovere (j)
// startIdx: indice nell'array vettArchi da cui partire (per evitare duplicati)
int combinationUtils(Grafo *g, int depth, int target, int startIdx, int *indices) {
    // Caso base: ho rimosso 'target' archi. Verifico connessione.
    if (depth == target) {
        // Disattivo gli archi selezionati
        for (int i = 0; i < target; i++) g->vettArchi[indices[i]].attivo = 0;
        
        int connesso = isConnected(g);
        
        // Riattivo gli archi (backtracking)
        for (int i = 0; i < target; i++) g->vettArchi[indices[i]].attivo = 1;
        
        // Se NON è connesso, ho trovato un set di taglio!
        if (!connesso) {
            printf("  -> Trovato set di taglio di dim %d: ", target);
            for(int i=0; i<target; i++) {
                int idx = indices[i];
                printf("(%s-%s) ", g->vettVertici[g->vettArchi[idx].u].id, g->vettVertici[g->vettArchi[idx].v].id);
            }
            printf("\n");
            return 1; // Successo: disconnesso
        }
        return 0; // Ancora connesso
    }

    // Generazione combinazioni
    for (int i = startIdx; i < g->nE; i++) {
        indices[depth] = i;
        if (combinationUtils(g, depth + 1, target, i + 1, indices)) {
            return 1; // Propago il successo verso l'alto
        }
    }
    return 0;
}

/*
 * Scopo: Verificare la proprietà j-edge-connected.
 * Logica:
 * 1. Loop per x da 1 a j.
 * 2. Genera tutte le combinazioni di x archi.
 * 3. Rimuovi temporaneamente -> Check Connessione -> Ripristina.
 * 4. Se si sconnette con x < j: FALLIMENTO (non è j-connesso, è meno robusto).
 * 5. Se si sconnette con x == j: SUCCESSO.
 * 6. Se non si sconnette mai: FALLIMENTO (è più robusto di j).
 */
void verificaJEdgeConnected(Grafo *g, int j) {
    printf("\n>>> Verifica %d-Edge-Connected...\n", j);
    
    // Ripristiniamo il grafo allo stato originale (se modificato dal k-core)
    for(int i=0; i<g->nV; i++) {
        g->vettVertici[i].attivo = 1;
        g->vettVertici[i].grado = g->vettVertici[i].grado_originale;
    }

    int *indices = (int*)malloc(j * sizeof(int));
    int disconnected = 0;

    for (int x = 1; x <= j; x++) {
        // Cerco se esiste un subset di dimensione x che sconnette
        if (combinationUtils(g, 0, x, 0, indices)) {
            disconnected = 1;
            if (x < j) {
                printf("RISULTATO: NO. Il grafo si sconnette rimuovendo solo %d archi (richiesti %d).\n", x, j);
            } else {
                printf("RISULTATO: SI. Il grafo e' %d-edge-connected (minimo set di taglio trovato a %d).\n", j, j);
            }
            break; 
        }
    }

    if (!disconnected) {
        printf("RISULTATO: NO. Non sono riuscito a sconnettere il grafo rimuovendo %d archi (è più connesso).\n", j);
    }

    free(indices);
}