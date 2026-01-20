#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Graph.h"

/* --- 1. FUNZIONI DI UTILITÀ --- */

/*
 * Scopo: Verificare se un carattere è una vocale.
 * Dettaglio Sintattico: Usiamo tolower() per gestire sia maiuscole che minuscole.
 */
int isVowel(char c) {
    c = tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

/*
 * Scopo: Verificare il vincolo di adiacenza tra due parole.
 * Regola: 
 * - Se parola1 finisce con vocale, parola2 NON può iniziare con vocale.
 * - Se parola1 finisce con consonante, parola2 NON può iniziare con consonante.
 * Logica: Quindi l'ultima lettera di parola1 e la prima di parola2 devono avere
 * natura diversa (una vocale, l'altra no).
 */
int checkConstraint(char *s1, char *s2) {
    int len1 = strlen(s1);
    char end1 = s1[len1 - 1]; // Ultimo char s1
    char start2 = s2[0];      // Primo char s2

    // XOR logico: Devono essere diverse (1 vocale e 0 vocale, o viceversa)
    if (isVowel(end1) != isVowel(start2))
        return 1; // Valido
    return 0; // Non valido
}

/*
 * Scopo: Contare quante volte un 'nome' specifico è già presente nel cammino attuale.
 * Parametri:
 * - path: array degli indici dei nodi nel cammino corrente.
 * - depth: lunghezza attuale del cammino.
 * - nameToCheck: il nome che vogliamo contare.
 * - G: il grafo per recuperare i nomi dagli id nel path.
 */
int countNameOccurrences(int *path, int depth, char *nameToCheck, Graph G) {
    int count = 0;
    for (int i = 0; i < depth; i++) {
        // strcmp ritorna 0 se le stringhe sono uguali
        if (strcmp(GRAPHgetName(G, path[i]), nameToCheck) == 0) {
            count++;
        }
    }
    return count;
}

/* --- 2. VARIABILI GLOBALI PER LA RICORSIONE --- */
// Nota: In un esame, passare tutto come parametri è più pulito, ma le globali
// riducono la complessità della firma della funzione dfs.
int *bestPathArr = NULL;
int bestPathLen = 0;
int maxVal = -1;

/* --- 3. CORE LOGIC (DFS) --- */

/*
 * Scopo: Esplorazione ricorsiva (DFS) per trovare il cammino ottimo.
 * Parametri:
 * - v: nodo corrente che stiamo visitando.
 * - G: grafo.
 * - M: limite ripetizione nomi.
 * - currPath: array dei nodi visitati finora.
 * - depth: profondità corrente (numero nodi nel path).
 * - currVal: somma dei valori nel path corrente.
 */
void dfs(int v, Graph G, int M, int *currPath, int depth, int currVal) {
    int i;
    
    // Aggiungo nodo corrente al path
    currPath[depth] = v;
    int newVal = currVal + GRAPHgetVal(G, v);
    
    // Aggiorno l'ottimo se abbiamo trovato un cammino con valore superiore
    // Nota: Il problema non richiede un nodo di destinazione specifico, 
    // ogni passo è un potenziale "best path".
    if (newVal > maxVal) {
        maxVal = newVal;
        bestPathLen = depth + 1;
        // Copia il path corrente nel bestPathArr
        for (i = 0; i <= depth; i++) {
            bestPathArr[i] = currPath[i];
        }
    }

    // Itero sui vicini
    Link t;
    for (t = GRAPHgetHead(G, v); t != NULL; t = GRAPHgetNext(t)) {
        int w = GRAPHgetDest(t);
        
        char *nameV = GRAPHgetName(G, v);
        char *nameW = GRAPHgetName(G, w);

        // PRUNING 1: Vincolo Vocale/Consonante
        if (!checkConstraint(nameV, nameW)) {
            continue; // Salta questo vicino
        }

        // PRUNING 2: Vincolo ripetizione nomi (max M volte)
        // Dobbiamo vedere se aggiungendo 'w', il suo nome supererebbe M.
        // Contiamo quante volte nameW è GIA' nel path.
        int occurrences = countNameOccurrences(currPath, depth + 1, nameW, G);
        if (occurrences < M) {
            // Se occurrences < M, aggiungendo w arriveremo al massimo a M. OK.
            // Passo ricorsivo
            dfs(w, G, M, currPath, depth + 1, newVal);
        }
    }
}

/* --- 4. WRAPPER FUNCTION --- */

/*
 * Scopo: Funzione chiamata dal main. Alloca memoria e lancia la ricorsione da ogni nodo.
 */
void bestPath(Graph G, int M) {
    int V = GRAPHgetNV(G);
    
    // Calcolo dimensione massima teorica:
    // Nel caso peggiore visitiamo ogni nodo esattamente M volte prima di non avere più mosse.
    // Usiamo V * M come limite superiore sicuro per evitare buffer overflow.
    int maxDepth = V * M + 1; 

    int *currPath = malloc(maxDepth * sizeof(int));
    if (currPath == NULL) {
        printf("Errore malloc currPath\n");
        exit(1);
    }
    
    if (bestPathArr != NULL) free(bestPathArr);
    bestPathArr = malloc(maxDepth * sizeof(int));
    if (bestPathArr == NULL) {
        printf("Errore malloc bestPathArr\n");
        free(currPath);
        exit(1);
    }
    
    maxVal = -1; // Reset globale
    bestPathLen = 0;

    printf("\n--- Avvio Ricerca BestPath (M=%d) ---\n", M);

    // Il cammino può iniziare da QUALSIASI nodo del grafo
    for (int i = 0; i < V; i++) {
        // Avvio DFS dal nodo i come radice (profondità 0, valore base 0)
        dfs(i, G, M, currPath, 0, 0);
    }

    // Stampa Risultati
    printf("Valore Massimo Trovato: %d\n", maxVal);
    printf("Cammino: ");
    for (int i = 0; i < bestPathLen; i++) {
        int id = bestPathArr[i];
        printf("%s (%d)", GRAPHgetName(G, id), GRAPHgetVal(G, id));
        if (i < bestPathLen - 1) printf(" -> ");
    }
    printf("\n");

    // Pulizia
    free(currPath);
    free(bestPathArr);
    bestPathArr = NULL; // Evita dangling pointer
}

/* --- 5. MAIN DI TEST --- */

int main() {
    FILE *fin = fopen("grafo.txt", "r");
    if (fin == NULL) {
        printf("Errore: file grafo.txt non trovato.\n");
        return 1;
    }

    Graph G = GRAPHload(fin);
    fclose(fin);

    printf("Grafo caricato: %d nodi\n", GRAPHgetNV(G));

    // Test Case come da esempio PDF
    // M=2 significa che ogni nome può comparire max 2 volte.
    bestPath(G, 2);

    GRAPHfree(G);
    return 0;
}