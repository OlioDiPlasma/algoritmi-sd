/*
 * File: main.c
 * Autore: Professore di C
 * Corso: Algoritmi e Programmazione
 * Scopo: Risoluzione problema Koala (Verifica e Ottimizzazione)
 */

#include "koala_structs.h"

/* --- VARIABILI GLOBALI PER BACKTRACKING (OTTIMIZZAZIONE) --- */
/* * Usiamo variabili globali (o statiche) per evitare di passare 
 * troppi parametri nella ricorsione, rendendo il codice più leggibile 
 * per fini didattici.
 */
int min_trees_used = 999999; // Inizializzato a valore alto
int *best_assignment = NULL; // Conterrà l'assegnazione ottima (vettore di T)

/* --- 1. FUNZIONI DI SUPPORTO (Ricerca Indici) --- */

/*
 * Funzione: get_koala_index
 * Scopo: Dato il nome (es "K7"), trova il suo indice nell'array koalas.
 */
int get_koala_index(Koala *koalas, int n, char *id) {
    for (int i = 0; i < n; i++) {
        if (strcmp(koalas[i].id, id) == 0) return i;
    }
    return -1; // Non trovato
}

/*
 * Funzione: get_family_index
 * Scopo: Dato il nome (es "F2"), trova il suo indice nell'array families.
 */
int get_family_index(Family *families, int s, char *id) {
    for (int i = 0; i < s; i++) {
        if (strcmp(families[i].id, id) == 0) return i;
    }
    return -1;
}

/* --- 2. FUNZIONE DI VERIFICA (Richiesta 1) --- */

/*
 * Funzione: verify_solution
 * Parametri:
 * - filename: nome del file solution.txt
 * - koalas, N: array koala e dimensione
 * - S: numero famiglie
 * - T: numero alberi
 * - m: capacità massima albero
 * - enemies_mat: matrice S x S delle inimicizie
 * * Concetti Chiave:
 * - Vettore 'tree_occupancy': conta quanti koala ci sono su ogni albero.
 * - Matrice 'tree_families': tiene traccia di quali famiglie sono presenti su un albero.
 * Usiamo una matrice (T x S) dove cella [t][f] = 1 se famiglia f è su albero t.
 */
int verify_solution(char *filename, Koala *koalas, int N, int S, int T, int m, int **enemies_mat) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Errore apertura file soluzione.\n");
        return 0;
    }

    /* Strutture temporanee per la verifica dello stato degli alberi */
    /* Calloc inizializza a 0, fondamentale per contatori */
    int *tree_occupancy = (int *)calloc(T, sizeof(int)); 
    
    /* Matrice [Albero][Famiglia]: 1 se la famiglia è presente sull'albero */
    int **tree_families = (int **)malloc(T * sizeof(int *));
    for (int i = 0; i < T; i++) {
        tree_families[i] = (int *)calloc(S, sizeof(int));
    }

    char k_id[MAX_STR];
    int t_id;
    int valid = 1; // Flag di validità

    printf("\n--- INIZIO VERIFICA SOLUZIONE ---\n");

    /* Lettura riga per riga: KoalaID AlberoID */
    while (fscanf(f, "%s %d", k_id, &t_id) == 2 && valid) {
        
        // 1. Esiste il koala?
        int k_idx = get_koala_index(koalas, N, k_id);
        if (k_idx == -1) {
            printf("Errore: Koala %s sconosciuto.\n", k_id);
            valid = 0; break;
        }

        // 2. L'albero esiste?
        if (t_id < 0 || t_id >= T) {
            printf("Errore: Albero %d inesistente.\n", t_id);
            valid = 0; break;
        }

        // 3. Il koala può vivere su questo albero? (Habitat)
        int can_live = 0;
        for (int i = 0; i < koalas[k_idx].n_habitats; i++) {
            if (koalas[k_idx].habitats[i] == t_id) {
                can_live = 1;
                break;
            }
        }
        if (!can_live) {
            printf("Errore: Koala %s non puo' vivere su albero %d.\n", k_id, t_id);
            valid = 0; break;
        }

        // 4. Capacità albero rispettata?
        if (tree_occupancy[t_id] >= m) {
            printf("Errore: Albero %d pieno (Capacita' %d).\n", t_id, m);
            valid = 0; break;
        }

        // 5. Verifica nemici
        int my_fam = koalas[k_idx].family_idx;
        // Controllo se sull'albero t_id ci sono famiglie nemiche della mia
        for (int other_fam = 0; other_fam < S; other_fam++) {
            // Se c'è una famiglia 'other_fam' sull'albero E quella famiglia è nemica
            if (tree_families[t_id][other_fam] == 1 && enemies_mat[my_fam][other_fam] == 1) {
                printf("Errore: Conflitto su albero %d tra famiglia di %s e famiglia indice %d.\n", t_id, k_id, other_fam);
                valid = 0; break;
            }
        }
        if (!valid) break;

        // Se tutto ok, aggiorno lo stato
        tree_occupancy[t_id]++;
        tree_families[t_id][my_fam] = 1; // Segno che questa famiglia è ora presente
    }

    /* Pulizia memoria verifica */
    free(tree_occupancy);
    for (int i = 0; i < T; i++) free(tree_families[i]);
    free(tree_families);
    fclose(f);

    if (valid) printf("ESITO: Soluzione VALIDA.\n");
    else printf("ESITO: Soluzione NON VALIDA.\n");

    return valid;
}

/* --- 3. FUNZIONI DI OTTIMIZZAZIONE (Richiesta 2) --- */

/*
 * Funzione: count_used_trees
 * Scopo: Conta quanti alberi non vuoti ci sono nella soluzione attuale
 */
int count_used_trees(Koala *koalas, int N, int T) {
    int *used = (int *)calloc(T, sizeof(int));
    int count = 0;
    for (int i = 0; i < N; i++) {
        if (koalas[i].assigned_tree != -1) {
            used[koalas[i].assigned_tree] = 1;
        }
    }
    for (int i = 0; i < T; i++) {
        if (used[i]) count++;
    }
    free(used);
    return count;
}

/*
 * Funzione: solve_recursive
 * Scopo: Algoritmo di Backtracking.
 * Parametri:
 * - k_idx: indice del koala che stiamo provando a piazzare (profondità ricorsione)
 * - current_occupancy: array (size T) con numero koala per albero
 * - tree_families: matrice (T x S) con presenza famiglie
 */
void solve_recursive(int k_idx, Koala *koalas, int N, int S, int T, int m, 
                     int **enemies_mat, int *current_occupancy, int **tree_families) {

    /* PRUNING (Potatura): Se abbiamo già usato più alberi del minimo trovato finora, ci fermiamo */
    /* Nota: Questo è un controllo costoso O(N), si potrebbe ottimizzare tenendo un contatore incrementale */
    int current_cost = count_used_trees(koalas, k_idx, T); // Conta fino a k_idx
    if (current_cost >= min_trees_used) return;

    /* BASE CASE: Tutti i koala sono stati piazzati */
    if (k_idx == N) {
        int final_cost = count_used_trees(koalas, N, T);
        if (final_cost < min_trees_used) {
            min_trees_used = final_cost;
            // Salvo la soluzione migliore
            for (int i = 0; i < N; i++) {
                best_assignment[i] = koalas[i].assigned_tree;
            }
        }
        return;
    }

    /* PASSO RICORSIVO: Provo a mettere il koala 'k_idx' su uno dei suoi alberi habitat */
    Koala *curr_k = &koalas[k_idx];
    int my_fam = curr_k->family_idx;

    for (int h = 0; h < curr_k->n_habitats; h++) {
        int t_dest = curr_k->habitats[h];

        // CHECK 1: Capacità
        if (current_occupancy[t_dest] >= m) continue;

        // CHECK 2: Nemici
        int conflict = 0;
        for (int f = 0; f < S; f++) {
            if (tree_families[t_dest][f] == 1 && enemies_mat[my_fam][f] == 1) {
                conflict = 1; break;
            }
        }
        if (conflict) continue;

        /* SE VALIDO: Applica mossa (DO) */
        int prev_fam_status = tree_families[t_dest][my_fam]; // Salviamo stato precedente
        
        curr_k->assigned_tree = t_dest;
        current_occupancy[t_dest]++;
        tree_families[t_dest][my_fam] = 1;

        /* Ricorsione */
        solve_recursive(k_idx + 1, koalas, N, S, T, m, enemies_mat, current_occupancy, tree_families);

        /* Backtracking (UNDO) */
        curr_k->assigned_tree = -1;
        current_occupancy[t_dest]--;
        // Attenzione: rimuovo la presenza della famiglia SOLO SE non ci sono altri koala della stessa famiglia
        // Ma per semplificare, l'approccio corretto richiederebbe un contatore di membri famiglia per albero.
        // Dato che tree_families è 0/1, dobbiamo essere cauti.
        // FIX: Usiamo un contatore invece di un flag booleano per tree_families? 
        // Per semplicità dell'esame, ricalcoliamo se la famiglia è ancora presente? No, lento.
        // SOLUZIONE MIGLIORE: `tree_families[t][f]` dovrebbe essere un contatore (int), non booleano.
        
        // Verifica rapida: se questo era l'unico koala di questa famiglia sull'albero, resetto a 0
        // Ma poiche' ho modificato la struttura dati logica qui sotto, correggiamo l'approccio:
        // Se `prev_fam_status` era 1, significa che c'era GIA' qualcuno della mia famiglia, quindi resta 1.
        // Se `prev_fam_status` era 0, ora deve tornare 0.
        tree_families[t_dest][my_fam] = prev_fam_status; 
    }
}

/* Wrapper per l'ottimizzazione */
void solve_optimization(Koala *koalas, int N, int S, int T, int m, int **enemies_mat) {
    printf("\n--- INIZIO OTTIMIZZAZIONE ---\n");

    /* Allocazione strutture ausiliarie per la ricorsione */
    best_assignment = (int *)malloc(N * sizeof(int));
    int *current_occupancy = (int *)calloc(T, sizeof(int));
    
    // Matrice T x S (usiamo int per contare le presenze o flaggare)
    // Qui usiamo la logica: se > 0 famiglia presente.
    int **tree_families = (int **)malloc(T * sizeof(int *));
    for (int i = 0; i < T; i++) tree_families[i] = (int *)calloc(S, sizeof(int));

    /* Inizializza assegnamenti koala a -1 */
    for(int i=0; i<N; i++) koalas[i].assigned_tree = -1;

    /* Lancio Backtracking */
    solve_recursive(0, koalas, N, S, T, m, enemies_mat, current_occupancy, tree_families);

    if (min_trees_used == 999999) {
        printf("Nessuna soluzione trovata che soddisfi i vincoli.\n");
    } else {
        printf("Soluzione Ottima Trovata! Alberi usati: %d\n", min_trees_used);
        printf("--- Dettaglio Soluzione ---\n");
        for (int i = 0; i < N; i++) {
            printf("%s %d\n", koalas[i].id, best_assignment[i]);
        }
    }

    /* Free memoria locale */
    free(best_assignment);
    free(current_occupancy);
    for (int i = 0; i < T; i++) free(tree_families[i]);
    free(tree_families);
}


/* --- 4. MAIN PROGRAM --- */

int main(int argc, char *argv[]) {
    /* * Controllo Argomenti Riga di Comando:
     * Uso: ./a.out <N> <T> <S> <m>
     */
    if (argc != 5) {
        printf("Uso: %s <N> <T> <S> <m>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int T = atoi(argv[2]);
    int S = atoi(argv[3]);
    int m = atoi(argv[4]);

    printf("Configurazione: N=%d, T=%d, S=%d, m=%d\n", N, T, S, m);

    /* --- ALLOCAZIONE STRUTTURE GLOBALI --- */
    Koala *koalas = (Koala *)malloc(N * sizeof(Koala));
    Family *families = (Family *)malloc(S * sizeof(Family));
    
    /* Allocazione Matrice Nemici (S x S) */
    int **enemies_mat = (int **)malloc(S * sizeof(int *));
    for (int i = 0; i < S; i++) {
        enemies_mat[i] = (int *)calloc(S, sizeof(int)); // Inizializza a 0
    }

    FILE *f;
    char buffer[100];

    /* --- LETTURA FILE 1: habitat.txt --- */
    f = fopen("habitat.txt", "r");
    if (!f) { perror("Errore habitat.txt"); return 1; }
    
    for (int i = 0; i < N; i++) {
        /* Legge riga tipo: "K7 3" */
        fscanf(f, "%s %d", koalas[i].id, &koalas[i].n_habitats);
        
        /* Alloca array dinamico per gli alberi di questo koala */
        koalas[i].habitats = (int *)malloc(koalas[i].n_habitats * sizeof(int));
        
        /* Legge gli alberi nelle righe successive */
        for (int j = 0; j < koalas[i].n_habitats; j++) {
            fscanf(f, "%d", &koalas[i].habitats[j]);
        }
    }
    fclose(f);

    /* --- LETTURA FILE 2: families.txt --- */
    f = fopen("families.txt", "r");
    if (!f) { perror("Errore families.txt"); return 1; }

    for (int i = 0; i < S; i++) {
        int n_members;
        /* Legge riga tipo: "F2 2" */
        fscanf(f, "%s %d", families[i].id, &n_members);
        
        /* Legge i membri koala e assegna loro l'indice della famiglia */
        char k_name[MAX_STR];
        for (int j = 0; j < n_members; j++) {
            fscanf(f, "%s", k_name);
            int k_idx = get_koala_index(koalas, N, k_name);
            if (k_idx != -1) {
                koalas[k_idx].family_idx = i; // Collega Koala alla Famiglia 'i'
            }
        }
    }
    fclose(f);

    /* --- LETTURA FILE 3: enemies.txt --- */
    f = fopen("enemies.txt", "r");
    if (!f) { perror("Errore enemies.txt"); return 1; }

    char f1_name[MAX_STR], f2_name[MAX_STR];
    while (fscanf(f, "%s %s", f1_name, f2_name) == 2) {
        int id1 = get_family_index(families, S, f1_name);
        int id2 = get_family_index(families, S, f2_name);
        
        if (id1 != -1 && id2 != -1) {
            /* Matrice simmetrica: A nemico di B => B nemico di A */
            enemies_mat[id1][id2] = 1;
            enemies_mat[id2][id1] = 1;
        }
    }
    fclose(f);

    /* --- ESECUZIONE VERIFICA --- */
    verify_solution("solution.txt", koalas, N, S, T, m, enemies_mat);

    /* --- ESECUZIONE OTTIMIZZAZIONE --- */
    solve_optimization(koalas, N, S, T, m, enemies_mat);

    /* --- PULIZIA MEMORIA (Fondamentale!) --- */
    for (int i = 0; i < N; i++) free(koalas[i].habitats);
    free(koalas);
    free(families);
    for (int i = 0; i < S; i++) free(enemies_mat[i]);
    free(enemies_mat);

    return 0;
}