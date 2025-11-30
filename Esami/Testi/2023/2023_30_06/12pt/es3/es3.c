#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* * ======================================================================================
 * NOME FUNZIONE: solve_partition (Funzione Ausiliaria Ricorsiva)
 * ======================================================================================
 * SCOPO:
 * Tenta di inserire l'elemento corrente del vettore (v[idx]) in uno dei 'x' sottoinsiemi.
 * Se l'inserimento è valido, procede ricorsivamente all'elemento successivo.
 * Se arriva in fondo (idx == d) e tutto è valido, restituisce true.
 *
 * PARAMETRI:
 * - int *v:          Il vettore originale di interi positivi.
 * - int d:           Dimensione del vettore originale.
 * - int idx:         L'indice dell'elemento che stiamo cercando di piazzare ora (0..d-1).
 * - int *bucket_sums:Array di dimensione 'x' che tiene traccia della somma attuale di ogni sottoinsieme.
 * - int x:           Numero di sottoinsiemi richiesti.
 * - int target:      La somma obiettivo che ogni sottoinsieme deve raggiungere.
 *
 * CONCETTI CHIAVE:
 * 1. Ricorsione e Backtracking:
 * Non sappiamo a priori dove mettere un numero. Lo mettiamo nel secchio 0, poi ricorriamo.
 * Se la ricorsione fallisce (torna false), togliamo il numero (backtrack) e proviamo il secchio 1.
 * ======================================================================================
 */
bool solve_partition(int *v, int d, int idx, int *bucket_sums, int x, int target) {
    
    /* --- 1. CASO BASE (Terminazione) --- */
    /* Se idx == d, significa che abbiamo piazzato con successo tutti gli elementi */
    if (idx == d) {
        /* Controllo finale: verifichiamo che tutti i secchi siano pieni al target.
           Nota: teoricamente, se la somma totale era corretta e non abbiamo mai superato il target,
           i secchi devono essere per forza pieni. Ma un controllo extra è didattico. */
        for (int i = 0; i < x; i++) {
            if (bucket_sums[i] != target) return false;
        }
        return true; // Soluzione trovata!
    }

    /* --- 2. LOGICA RICORSIVA (Iterazione sui Sottoinsiemi) --- */
    /* Proviamo a mettere l'elemento v[idx] in ciascuno degli 'x' sottoinsiemi */
    for (int i = 0; i < x; i++) {
        
        /* PRUNING (Potatura):
         * Se aggiungendo l'elemento superiamo il target, è inutile provare:
         * quel ramo dell'albero di ricerca è morto.
         */
        if (bucket_sums[i] + v[idx] <= target) {
            
            /* DO (Fai): Aggiungi l'elemento al sottoinsieme corrente */
            bucket_sums[i] += v[idx];

            /* RECURSE (Ricorri): Prova a piazzare il prossimo elemento (idx + 1) */
            /* Se la chiamata ricorsiva torna true, abbiamo finito: risaliamo restituendo true */
            if (solve_partition(v, d, idx + 1, bucket_sums, x, target)) {
                return true;
            }

            /* UNDO (Disfa - Backtracking):
             * Se siamo qui, la strada intrapresa non ha portato a soluzione.
             * Dobbiamo rimuovere l'elemento dal secchio corrente per poterlo provare nel prossimo loop.
             */
            bucket_sums[i] -= v[idx];
        }
        
        /* PRUNING AVANZATO (Opzionale ma potente):
         * Se il secchio attuale è vuoto (0) e non siamo riusciti a riempirlo con questo elemento,
         * è inutile provare con i successivi secchi vuoti, perché sono identici.
         * Questo evita permutazioni inutili di secchi vuoti.
         */
        if (bucket_sums[i] == 0) break;
    }

    /* Se abbiamo provato tutti i secchi e nessuno va bene, questo ramo è un vicolo cieco */
    return false;
}

/* * ======================================================================================
 * NOME FUNZIONE: can_partition
 * ======================================================================================
 * SCOPO:
 * Funzione "wrapper" (interfaccia) che prepara i dati, fa i controlli preliminari
 * e lancia la ricorsione.
 *
 * PARAMETRI:
 * - int *v, int d, int x: Dati del problema (Vettore, dimensione, numero sottoinsiemi).
 * ======================================================================================
 */
bool can_partition(int *v, int d, int x) {
    /* --- 1. CONTROLLI PRELIMINARI (Pruning Globale) --- */
    int total_sum = 0;
    for (int i = 0; i < d; i++) total_sum += v[i];

    /* Se la somma totale non è divisibile per x, è matematicamente impossibile */
    if (x <= 0 || total_sum % x != 0) {
        return false;
    }

    int target_sum = total_sum / x;

    /* --- 2. ALLOCAZIONE --- */
    /*
     * Usiamo calloc invece di malloc.
     * calloc(numero_elementi, dimensione) inizializza AUTOMATICAMENTE a zero.
     * È fondamentale perché 'bucket_sums' parte da somme zero.
     */
    int *bucket_sums = (int *)calloc(x, sizeof(int));
    if (bucket_sums == NULL) {
        printf("Errore di memoria\n");
        exit(1);
    }

    /* --- 3. AVVIO RICORSIONE --- */
    /* Partiamo dall'indice 0 del vettore */
    bool result = solve_partition(v, d, 0, bucket_sums, x, target_sum);

    /* --- 4. GESTIONE MEMORIA --- */
    free(bucket_sums); // Mai dimenticare la free!

    return result;
}

/* * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    // Caso di test: Vettore [4, 3, 2, 3, 5, 2, 1], Somma totale = 20.
    // Vogliamo dividerlo in x=4 sottoinsiemi. Target per sottoinsieme = 5.
    // Una soluzione possibile: {5}, {4,1}, {3,2}, {3,2}
    int v[] = {4, 3, 2, 3, 5, 2, 1};
    int d = 7; // Dimensione del vettore
    int x = 4; // Numero di sottoinsiemi

    printf("--- TEST K-PARTITION ---\n");
    printf("Vettore: [ ");
    for(int i=0; i<d; i++) printf("%d ", v[i]);
    printf("]\n");
    printf("Cerco di dividere in %d sottoinsiemi di somma uguale.\n\n", x);

    if (can_partition(v, d, x)) {
        printf("RISULTATO: Successo! È possibile suddividere il vettore.\n");
    } else {
        printf("RISULTATO: Fallimento. Non è possibile.\n");
    }

    return 0;
}