#include <stdio.h>
#include <stdlib.h>

/* ========================================================================= */
/* 1. FUNZIONI AUSILIARIE E LOGICA CORE */
/* ========================================================================= */

/*
 * Funzione: check_amici
 * Scopo: Verificare se una persona può entrare in un gruppo esistente.
 * Affinché possa entrare, deve essere amica di TUTTI i membri attuali del gruppo.
 *
 * Parametri:
 * - int persona: L'indice della persona che stiamo provando a inserire (es. 2).
 * - int gruppo_id: L'indice del gruppo in cui vogliamo inserirla.
 * - int *sol_corr: Array che contiene l'assegnazione corrente (sol_corr[i] = gruppo di i).
 * - int **M: La matrice di adiacenza delle amicizie.
 * - int N: Numero totale di persone.
 *
 * Ritorna: 1 se la persona è amica di tutti nel gruppo, 0 altrimenti.
 */
int check_amici(int persona, int gruppo_id, int *sol_corr, int **M, int N) {
    /* --- 1. SCANSIONE MEMBRI ESISTENTI --- */
    // Controlliamo tutte le persone venute prima dell'attuale
    for (int i = 0; i < persona; i++) {
        
        // Se la persona 'i' fa parte del 'gruppo_id' che stiamo testando...
        if (sol_corr[i] == gruppo_id) {
            
            // ...verifichiamo l'amicizia nella matrice M.
            // Se M[persona][i] è 0, non sono amici.
            // Basta un solo "non amico" per invalidare la clique.
            if (M[persona][i] == 0) {
                return 0; // Non compatibile
            }
        }
    }
    return 1; // Compatibile con tutti
}

/*
 * Funzione: solve_clique_partition
 * Scopo: Funzione ricorsiva di backtracking per trovare la partizione minima.
 *
 * Parametri:
 * - int persona: Il livello della ricorsione (indice della persona da sistemare).
 * - int **M: Matrice adiacenza.
 * - int N: Numero persone.
 * - int *sol_corr: Vettore soluzione parziale (chi va in quale gruppo).
 * - int *best_sol: Vettore soluzione migliore trovata finora.
 * - int n_gruppi_corr: Quanti gruppi stiamo usando attualmente.
 * - int *min_gruppi: (Puntatore a intero) Il record del minor numero di gruppi trovati finora.
 *
 * Modello Combinatorio: Partizioni di un Insieme.
 * Criterio di Pruning: Se n_gruppi_corr >= *min_gruppi, ci fermiamo (non miglioreremo il minimo).
 */
void solve_clique_partition(int persona, int **M, int N, int *sol_corr, int *best_sol, int n_gruppi_corr, int *min_gruppi) {

    /* --- 1. PRUNING (POTATURA) --- */
    // Se stiamo già usando un numero di gruppi uguale o superiore alla soluzione migliore trovata
    // in precedenza, è inutile continuare su questo ramo. Cerchiamo il MINIMO.
    if (n_gruppi_corr >= *min_gruppi) {
        return;
    }

    /* --- 2. BASE CASE (TERMINAZIONE) --- */
    // Se abbiamo sistemato tutte le persone (dalla 0 alla N-1)
    if (persona == N) {
        // Abbiamo trovato una soluzione valida completa.
        // Grazie al pruning, se siamo qui, n_gruppi_corr è sicuramente < *min_gruppi.
        
        *min_gruppi = n_gruppi_corr; // Aggiorniamo il record
        
        // Copiamo la soluzione corrente nella memoria della soluzione migliore
        for (int i = 0; i < N; i++) {
            best_sol[i] = sol_corr[i];
        }
        return;
    }

    /* --- 3. PASSO RICORSIVO --- */
    
    // Tentativo A: Inserire la 'persona' in uno dei gruppi GIÀ ESISTENTI.
    for (int g = 0; g < n_gruppi_corr; g++) {
        if (check_amici(persona, g, sol_corr, M, N)) {
            
            sol_corr[persona] = g; // Assegna
            
            // Ricorsione: passa alla prossima persona
            // Il numero di gruppi non cambia
            solve_clique_partition(persona + 1, M, N, sol_corr, best_sol, n_gruppi_corr, min_gruppi);
            
            // Backtracking implicito: al prossimo giro del for sovrascriviamo sol_corr[persona]
        }
    }

    // Tentativo B: Creare un NUOVO GRUPPO per questa persona.
    // Un nuovo gruppo con una sola persona è sempre una clique valida (banalmente).
    sol_corr[persona] = n_gruppi_corr; // Assegna nuovo ID gruppo (es: se avevo 0,1, ora assegno 2)
    
    // Ricorsione: passa alla prossima persona, incrementando il contatore gruppi
    solve_clique_partition(persona + 1, M, N, sol_corr, best_sol, n_gruppi_corr + 1, min_gruppi);
}

/* ========================================================================= */
/* 2. FUNZIONE WRAPPER (INTERFACCIA) */
/* ========================================================================= */

/*
 * Scopo: Preparare la memoria e lanciare la ricorsione.
 */
void trova_gruppi_amici(int **M, int N) {
    /* --- 1. ALLOCAZIONE VETTORI SOLUZIONE --- */
    // Usiamo calloc per pulizia, ma malloc va bene.
    // sol_corr: terrà traccia delle scelte mentre scendiamo nell'albero.
    int *sol_corr = (int *)malloc(N * sizeof(int));
    
    // best_sol: salverà la configurazione vincente.
    int *best_sol = (int *)malloc(N * sizeof(int));

    if (sol_corr == NULL || best_sol == NULL) {
        printf("Errore malloc\n");
        return;
    }

    // Inizializziamo il minimo al caso peggiore: N gruppi (ogni persona da sola).
    int min_gruppi = N + 1; 

    /* --- 2. LANCIO RICORSIONE --- */
    // Partiamo dalla persona 0, con 0 gruppi attuali.
    solve_clique_partition(0, M, N, sol_corr, best_sol, 0, &min_gruppi);

    /* --- 3. STAMPA RISULTATI FORMATTATI --- */
    printf("Numero minimo di gruppi: %d\n", min_gruppi);
    
    // Stampiamo i gruppi. Iteriamo per ogni ID di gruppo valido.
    for (int g = 0; g < min_gruppi; g++) {
        printf("Gruppo %d: { ", g + 1);
        int primo = 1;
        for (int i = 0; i < N; i++) {
            if (best_sol[i] == g) {
                if (!primo) printf(", ");
                printf("p%d", i);
                primo = 0;
            }
        }
        printf(" }\n");
    }

    /* --- 4. GESTIONE MEMORIA --- */
    free(sol_corr);
    free(best_sol);
}

/* ========================================================================= */
/* 3. MAIN DI TEST */
/* ========================================================================= */

int main() {
    printf("--- Inizio Test Clique Partitioning ---\n");

    /* --- 1. CREAZIONE DATI (L'ESEMPIO DELL'IMMAGINE) --- */
    int N = 4;
    
    // Allocazione matrice dinamica (Array di Puntatori)
    int **M = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        M[i] = (int *)malloc(N * sizeof(int));
    }

    // Riempimento Matrice come da esempio:
    // 1 1 0 1
    // 1 1 0 1
    // 0 0 1 0
    // 1 1 0 1
    
    // Riga 0 (p0)
    M[0][0]=1; M[0][1]=1; M[0][2]=0; M[0][3]=1;
    // Riga 1 (p1)
    M[1][0]=1; M[1][1]=1; M[1][2]=0; M[1][3]=1;
    // Riga 2 (p2) - L'isolato
    M[2][0]=0; M[2][1]=0; M[2][2]=1; M[2][3]=0;
    // Riga 3 (p3)
    M[3][0]=1; M[3][1]=1; M[3][2]=0; M[3][3]=1;

    /* --- 2. CHIAMATA AL WRAPPER --- */
    trova_gruppi_amici(M, N);

    /* --- 3. PULIZIA MEMORIA MATRICE --- */
    for (int i = 0; i < N; i++) {
        free(M[i]); // Libero le singole righe
    }
    free(M); // Libero il vettore di puntatori
    
    printf("--- Fine Test ---\n");
    return 0;
}