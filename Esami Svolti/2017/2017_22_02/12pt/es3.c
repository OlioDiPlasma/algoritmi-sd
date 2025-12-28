#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * FUNZIONE HELPER: DFS (Depth First Search - Visita in Profondità)
 * ============================================================================
 * Scopo:
 * Trovare tutti gli elementi "incollati" (indivisibili) a partire da un
 * certo elemento 'start'. Se m[i][j] == 0, sono collegati.
 *
 * Parametri:
 * - n: numero totale elementi.
 * - start: l'indice dell'elemento da cui parte l'esplorazione.
 * - m: la matrice delle adiacenze (0 = collegato/indivisibile).
 * - visited: array per tenere traccia di chi abbiamo già visitato per non girare in tondo.
 * - component: array dove stiamo accumulando gli indici del blocco corrente.
 * - comp_size: puntatore a intero per contare quanti elementi stiamo trovando.
 *
 * Concetti Chiave:
 * - Ricorsione: La funzione chiama se stessa per esplorare i vicini dei vicini.
 * ============================================================================
 */
void trova_indivisibili(int n, int start, int **m, int *visited, int *component, int *comp_size) {
    /* Segniamo il nodo corrente come visitato per non tornarci */
    visited[start] = 1;

    /* Aggiungiamo questo nodo alla lista del "blocco" (componente) corrente */
    component[*comp_size] = start;
    (*comp_size)++; /* Dereferenziamo per incrementare il contatore reale in memoria */

    /* Ciclo per cercare i vicini */
    for (int i = 0; i < n; i++) {
        /*
         * DUE CONDIZIONI FONDAMENTALI:
         * 1. m[start][i] == 0: Il testo dice che 0 significa NON separabili (quindi uniti).
         * 2. !visited[i]: Non dobbiamo processare chi abbiamo già preso.
         */
        if (m[start][i] == 0 && !visited[i]) {
            /* Passo ricorsivo: scendi in profondità nel nodo 'i' */
            trova_indivisibili(n, i, m, visited, component, comp_size);
        }
    }
}

/* ============================================================================
 * FUNZIONE PRINCIPALE: f
 * ============================================================================
 * Scopo:
 * Partizionare l'insieme in due sottoinsiemi rispettando i vincoli di indivisibilità.
 * Il secondo sottoinsieme deve avere dimensione >= k.
 *
 * Parametri:
 * - n: numero elementi.
 * - k: dimensione minima del secondo sottoinsieme.
 * - m: matrice nxn delle relazioni (0 = indivisibile).
 *
 * Ritorna:
 * - Un puntatore a int (array) di dimensione n.
 * valore 0 -> Elemento nel primo sottoinsieme.
 * valore 1 -> Elemento nel secondo sottoinsieme (quello target >= k).
 * Ritorna NULL se impossibile (es. n < k).
 *
 * Concetti Chiave:
 * - Greedy (Avido): Prendiamo interi blocchi indivisibili e li buttiamo
 * nel secondo insieme finché non è pieno abbastanza.
 * ============================================================================
 */
int *f(int n, int k, int **m) {
    /* --- 1. CONTROLLI PRELIMINARI --- */
    if (n < k) return NULL; /* Impossibile soddisfare la richiesta */

    /* --- 2. ALLOCAZIONE MEMORIA --- */
    /*
     * calloc è perfetta qui: alloca E inizializza a 0.
     * result[i] = 0 significherà "Primo Sottoinsieme" di default.
     * Noi cambieremo a 1 solo quelli che spostiamo nel "Secondo Sottoinsieme".
     */
    int *result = (int *)calloc(n, sizeof(int));
    if (result == NULL) return NULL; // Controllo fallimento malloc

    /* Array di supporto per la visita del grafo (0 = non visitato, 1 = visitato) */
    int *visited = (int *)calloc(n, sizeof(int));
    if (visited == NULL) {
        free(result); // Evitiamo memory leak se la seconda malloc fallisce
        return NULL;
    }

    /* Buffer temporaneo per memorizzare gli indici di un singolo blocco indivisibile */
    int *temp_component = (int *)malloc(n * sizeof(int));
    if (temp_component == NULL) {
        free(result);
        free(visited);
        return NULL;
    }

    /* --- 3. LOGICA DI PARTIZIONAMENTO --- */
    int count_subset2 = 0; // Quanti elementi abbiamo messo nel secondo set

    for (int i = 0; i < n; i++) {
        /*
         * Se abbiamo già raggiunto k elementi nel secondo insieme, possiamo fermarci!
         * Il resto degli elementi rimarrà a 0 (Primo Sottoinsieme) ed è valido.
         */
        if (count_subset2 >= k) break;

        /* Se l'elemento i non è stato ancora visitato, troviamo tutto il suo blocco */
        if (!visited[i]) {
            int comp_size = 0;

            /* Chiamata alla funzione helper per trovare tutti gli amici di 'i' */
            trova_indivisibili(n, i, m, visited, temp_component, &comp_size);

            /*
             * Ora 'temp_component' contiene 'comp_size' indici che DEVONO stare insieme.
             * Li spostiamo tutti nel secondo sottoinsieme (result = 1).
             */
            for (int j = 0; j < comp_size; j++) {
                int node_index = temp_component[j]; // Leggo l'indice salvato
                result[node_index] = 1;             // Lo assegno al secondo set
            }

            /* Aggiorniamo il contatore totale del secondo set */
            count_subset2 += comp_size;
        }
    }

    /* --- 4. PULIZIA E RITORNO --- */
    free(visited);       // Non serve più
    free(temp_component); // Non serve più

    /*
     * Nota: Non controlliamo se count_subset2 < k alla fine, perché
     * assumiamo che n >= k. Se prendiamo TUTTI i componenti, avremo n elementi,
     * che è >= k. Quindi una soluzione esiste sempre se n >= k.
     */

    return result; // Ritorniamo l'array risultante al chiamante
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo: Verificare il funzionamento con dati hardcoded.
 */
int main() {
    int n = 5;
    int k = 3;

    /*
     * Creiamo una matrice nxn.
     * Immaginiamo questi collegamenti (0 = incollato, 1 = separabile):
     * 0 è incollato a 1 (e viceversa). -> Blocco {0, 1} dim 2
     * 2 è incollato a 3 (e viceversa). -> Blocco {2, 3} dim 2
     * 4 è solo. -> Blocco {4} dim 1
     *
     * Obiettivo k=3: L'algoritmo dovrebbe prendere {0,1} (tot 2) e poi {2,3} (tot 4 >= 3).
     * Risultato atteso: 0,1,2,3 nel set 2. 4 nel set 1 (o viceversa in base all'ordine).
     */

    /* Allocazione matrice di puntatori (righe) */
    int **m = (int **)malloc(n * sizeof(int *));

    /* Allocazione delle singole righe */
    for (int i = 0; i < n; i++) {
        m[i] = (int *)malloc(n * sizeof(int));
    }

    /* Riempimento matrice (1 = separabile, 0 = INSEPARABILE) */
    /* Inizializziamo tutto a 1 (separabile) */
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++) m[i][j] = 1;

    /* Definiamo i vincoli (diagonale sempre 0, un elemento non si separa da se stesso) */
    for(int i=0; i<n; i++) m[i][i] = 0;

    /* Definiamo le coppie inseparabili (simmetriche) */
    m[0][1] = 0; m[1][0] = 0; // 0 e 1 uniti
    m[2][3] = 0; m[3][2] = 0; // 2 e 3 uniti

    printf("--- INIZIO TEST ---\n");
    printf("Cerco partizione con secondo sottoinsieme >= %d elementi.\n", k);

    int *partition = f(n, k, m);

    if (partition) {
        printf("Risultato partizione (1 = Secondo Set, 0 = Primo Set):\n");
        int count = 0;
        for (int i = 0; i < n; i++) {
            printf("Elemento %d: %d\n", i, partition[i]);
            if(partition[i] == 1) count++;
        }
        printf("Totale elementi nel secondo set: %d (Richiesto >= %d)\n", count, k);
        free(partition); // IMPORTANTISSIMO: Chi chiama la funzione deve liberare il risultato!
    } else {
        printf("Errore: impossibile trovare partizione.\n");
    }

    /* --- PULIZIA MEMORIA MAIN --- */
    for (int i = 0; i < n; i++) {
        free(m[i]); // Libera ogni riga
    }
    free(m); // Libera l'array di puntatori alle righe

    return 0;
}