#include "programma.h"

/* --- FUNZIONI DI SERVIZIO (PRIVATE) --- */

/* Controlla se una posizione è valida per inserire un articolo */
int is_valid(Programma p, Articoli art_db, int art_idx, int r, int s) {
    Articolo a = get_articolo_by_index(art_db, art_idx);
    
    // 1. Controllo limiti matrice (l'articolo non deve sforare le colonne S)
    if (s + a.slot > p.S) return 0;

    // 2. Controllo sovrapposizioni e Slot contigui [cite: 50]
    for (int k = 0; k < a.slot; k++) {
        if (p.griglia[r][s + k] != -1) return 0; // Cella già occupata
    }

    // 3. Controllo Relatore (non può essere in due sale contemporaneamente) [cite: 62]
    // Dobbiamo scansionare tutte le ALTRE righe negli stessi slot temporali
    for (int k = 0; k < a.slot; k++) { // Per ogni slot occupato dall'articolo corrente
        int current_time = s + k;
        for (int row = 0; row < p.R; row++) {
            if (row == r) continue; // Salta la riga corrente
            int other_id = p.griglia[row][current_time];
            if (other_id != -1) {
                Articolo b = get_articolo_by_index(art_db, other_id);
                if (strcmp(a.relatore, b.relatore) == 0) return 0; // Conflitto relatore
            }
        }
    }

    return 1;
}

/* --- INIZIALIZZAZIONE --- */
Programma init_programma(int R, int S) {
    Programma p;
    p.R = R;
    p.S = S;
    
    // Allocazione Matrice Dinamica (Array di puntatori)
    p.griglia = (int **)malloc(R * sizeof(int *));
    for (int i = 0; i < R; i++) {
        p.griglia[i] = (int *)malloc(S * sizeof(int));
        // Inizializza a -1 (vuoto)
        for (int j = 0; j < S; j++) {
            p.griglia[i][j] = -1;
        }
    }
    return p;
}

void free_programma(Programma p) {
    for (int i = 0; i < p.R; i++) {
        free(p.griglia[i]);
    }
    free(p.griglia);
}

void stampa_programma(Programma p, Articoli art_db) {
    printf("\n--- PROGRAMMA CONFERENCE ---\n");
    for (int i = 0; i < p.R; i++) {
        printf("Sala %d: ", i);
        for (int j = 0; j < p.S; j++) {
            if (p.griglia[i][j] == -1) printf("[   ] ");
            else printf("[%3d] ", p.griglia[i][j]);
        }
        printf("\n");
    }
}

/* --- SEZIONE 4.2.3: VERIFICA --- 
 * Legge file soluzione e controlla validità.
 */
int verifica_programma_da_file(char *filename, Programma p, Articoli art_db) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    int r, s, id_art;
    int *preso = calloc(get_num_articoli(art_db), sizeof(int)); // Array contatore presenze

    while (fscanf(fp, "%d %d %d", &r, &s, &id_art) == 3) {
        // Controllo validità posizionamento usando la funzione is_valid definita sopra
        // Nota: is_valid controlla limiti, sovrapposizioni e relatori.
        if (!is_valid(p, art_db, id_art, r, s)) {
            printf("ERRORE: Posizionamento non valido per articolo %d in (%d, %d)\n", id_art, r, s);
            free(preso); fclose(fp); return 0;
        }

        // Assegna alla griglia
        Articolo a = get_articolo_by_index(art_db, id_art);
        for (int k = 0; k < a.slot; k++) {
            p.griglia[r][s + k] = id_art;
        }
        preso[id_art] = 1;
    }

    // Controllo finale: tutti gli articoli devono essere presi [cite: 65]
    for (int i = 0; i < get_num_articoli(art_db); i++) {
        if (preso[i] == 0) {
            printf("ERRORE: Articolo %d non pianificato.\n", i);
            free(preso); fclose(fp); return 0;
        }
    }

    printf("VERIFICA SUCCESSO: Il programma nel file è valido.\n");
    free(preso);
    fclose(fp);
    return 1;
}

/* --- SEZIONE 4.2.4: OTTIMIZZAZIONE --- */

/* Calcolo della Funzione Obiettivo (Score)
 * Minore è meglio. Score = NV (buchi) + NA (temi).
 */
int calcola_score(Programma p, Articoli art_db) {
    int NV = 0; // Vuoti all'inizio o mezzo
    int NA = 0; // Somma argomenti diversi

    // 1. Calcolo NV [cite: 84]
    for (int r = 0; r < p.R; r++) {
        int last_occupied = -1;
        // Trova l'ultimo slot occupato nella riga
        for (int s = 0; s < p.S; s++) {
            if (p.griglia[r][s] != -1) last_occupied = s;
        }
        // Conta i buchi PRIMA dell'ultimo occupato
        if (last_occupied != -1) {
            for (int s = 0; s < last_occupied; s++) {
                if (p.griglia[r][s] == -1) NV++;
            }
        }
    }

    // 2. Calcolo NA [cite: 87]
    for (int r = 0; r < p.R; r++) {
        // Usiamo un piccolo trucco brute-force per contare stringhe uniche
        // Dato che N è piccolo, va bene. Altrimenti servirebbe una Hashset.
        int temi_count = 0;
        // Array statico di stringhe (max S puntatori) per tenere traccia dei temi visti nella riga
        char *temi_visti[100]; 
        int n_temi_visti = 0;

        for (int s = 0; s < p.S; s++) {
            int id = p.griglia[r][s];
            if (id != -1) {
                Articolo a = get_articolo_by_index(art_db, id);
                int already_seen = 0;
                
                // Controlla se abbiamo già contato questo tema per questa riga
                for(int k=0; k<n_temi_visti; k++) {
                    if (strcmp(temi_visti[k], a.argomento) == 0) {
                        already_seen = 1;
                        break;
                    }
                }
                if (!already_seen) {
                    temi_visti[n_temi_visti++] = a.argomento;
                    temi_count++;
                }
            }
        }
        NA += temi_count;
    }

    return NV + NA; // [cite: 88]
}

/* Backtracking Ricorsivo */
void solve_rec(int livello, Programma p, Articoli art_db, int *preso, Programma *best_p, int *best_score) {
    int n_art = get_num_articoli(art_db);

    // CASO BASE: Tutti gli articoli sono stati assegnati
    if (livello == n_art) {
        int current_score = calcola_score(p, art_db);
        if (current_score < *best_score) {
            *best_score = current_score;
            // Copia profonda della griglia nella soluzione ottima
            for(int i=0; i<p.R; i++)
                for(int j=0; j<p.S; j++)
                    best_p->griglia[i][j] = p.griglia[i][j];
        }
        return;
    }

    // PASSO RICORSIVO
    // Proviamo a piazzare l'articolo "livello" (o cerchiamo il primo non preso)
    // Strategia: iteriamo sugli articoli per trovarne uno non preso
    // NOTA: Per ottimizzare le permutazioni, fissiamo l'articolo corrente all'indice 'livello' se volessimo un ordine fisso,
    // ma qui dobbiamo provare TUTTE le combinazioni. 
    // PER SEMPLIFICARE: Assumiamo che 'livello' sia l'indice dell'articolo che stiamo cercando di piazzare ora?
    // No, meglio iterare su tutti gli articoli non presi per generare tutte le permutazioni, 
    // MA dato che l'ordine di inserimento non cambia il risultato finale della griglia (è commutativo spazialmente),
    // possiamo imporre un ordine per evitare permutazioni ridondanti: piazziamo l'articolo con indice 'livello'.
    
    Articolo a = get_articolo_by_index(art_db, livello);

    // Proviamo tutte le posizioni possibili (Sala r, Slot s)
    for (int r = 0; r < p.R; r++) {
        for (int s = 0; s < p.S; s++) {
            
            if (is_valid(p, art_db, livello, r, s)) {
                
                // DO: Assegna
                for (int k = 0; k < a.slot; k++) p.griglia[r][s + k] = livello;
                
                // RECURSE
                solve_rec(livello + 1, p, art_db, preso, best_p, best_score);
                
                // BACKTRACK: Rimuovi
                for (int k = 0; k < a.slot; k++) p.griglia[r][s + k] = -1;
            }
        }
    }
}

void genera_programma_ottimo(Programma p, Articoli art_db) {
    int n_art = get_num_articoli(art_db);
    int *preso = calloc(n_art, sizeof(int)); // Non strettamente necessario se usiamo l'indice 'livello', ma buona pratica
    
    Programma best_p = init_programma(p.R, p.S);
    int best_score = 999999; // Infinito

    printf("Avvio ricerca ottima (puo' richiedere tempo per S/R grandi)...\n");
    
    // Iniziamo piazzando l'articolo 0
    solve_rec(0, p, art_db, preso, &best_p, &best_score);

    if (best_score == 999999) {
        printf("Nessuna soluzione trovata.\n");
    } else {
        printf("Soluzione Ottima trovata con Score: %d\n", best_score);
        stampa_programma(best_p, art_db);
    }

    free(preso);
    free_programma(best_p);
}