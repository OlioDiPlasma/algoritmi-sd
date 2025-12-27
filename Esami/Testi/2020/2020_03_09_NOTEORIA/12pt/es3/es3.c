#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Per INT_MAX

/* =================================================================================
 * MODELLO COMBINATORIO E PRUNING
 * =================================================================================
 * * Modello Combinatorio: Disposizioni con ripetizione (limitata).
 * Immaginiamo un albero delle decisioni in cui:
 * - I LIVELLI dell'albero rappresentano i GRUPPI (livello 0 = gruppo 0, ecc.).
 * - I RAMI che partono da un nodo rappresentano le SALE disponibili.
 * Ad ogni passo ricorsivo, decidiamo: "In quale sala metto il gruppo corrente?"
 * * Criteri di Pruning (Potatura dell'albero):
 * 1. Vincolo di Capacità (Hard Constraint): Se la dimensione del gruppo > capacità 
 * residua della sala, quel ramo viene tagliato (non scendiamo in ricorsione).
 * 2. Ottimizzazione (Bound): Se nel ramo corrente stiamo già usando un numero di sale
 * maggiore o uguale al "miglior risultato trovato finora", ci fermiamo. 
 * È inutile continuare se abbiamo già una soluzione migliore o uguale salvata.
 */

/* =================================================================================
 * FUNZIONE: solve_ricorsiva (Il "motore" del backtracking)
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Esplora ricorsivamente le assegnazioni dei gruppi alle sale.
 * * PARAMETRI:
 * - int gr_idx: Indice del gruppo che stiamo provando a sedere (livello ricorsione).
 * - int *P: Vettore dimensioni gruppi (sola lettura).
 * - int nP: Numero totale gruppi.
 * - int *cap_rimanente: Vettore modificabile con la capacità residua attuale delle sale.
 * - int nS: Numero sale.
 * - int *sale_usate_attualmente: Vettore di flag (1 se sala usata, 0 altrimenti).
 * - int *best_sol: Puntatore a intero che conserva il numero minimo di sale trovato finora.
 * * CONCETTI CHIAVE:
 * Passiamo 'best_sol' per puntatore perché deve essere condiviso tra tutte le
 * chiamate ricorsive. Se una chiamata trova un nuovo minimo, tutti devono saperlo.
 * =================================================================================
 */
void solve_ricorsiva(int gr_idx, int *P, int nP, int *cap_rimanente, int nS, int *sale_usate_attualmente, int *best_sol) {

    /* --- 1. CASO BASE (Condizione di terminazione) --- */
    // Se gr_idx raggiunge nP, significa che abbiamo sistemato TUTTI i gruppi con successo.
    if (gr_idx == nP) {
        // Contiamo quante sale sono state effettivamente usate in questa configurazione
        int count_usate = 0;
        for (int i = 0; i < nS; i++) {
            if (sale_usate_attualmente[i] > 0) { // Se > 0 contiene almeno un gruppo
                count_usate++;
            }
        }

        // Se abbiamo trovato un nuovo minimo globale, aggiorniamo best_sol
        if (count_usate < *best_sol) {
            *best_sol = count_usate;
            // NOTA: Qui si potrebbe anche salvare la configurazione esatta (es. un vettore assegnazioni)
            // ma il testo chiede di determinare il numero minimo.
        }
        return;
    }

    /* --- 2. LOGICA RICORSIVA (Iterazione sulle scelte possibili) --- */
    
    // Per il gruppo corrente 'P[gr_idx]', proviamo ogni sala 'i'
    for (int i = 0; i < nS; i++) {

        // PRUNING 1: La sala ha spazio sufficiente?
        if (cap_rimanente[i] >= P[gr_idx]) {

            // Salviamo lo stato precedente per il backtracking
            int old_uso = sale_usate_attualmente[i];
            
            // PRUNING 2 (Ottimizzazione):
            // Calcoliamo quante sale stiamo usando ORA.
            // Se stiamo per usare una sala NUOVA e il conteggio attuale eguaglia
            // già il 'best_sol', è inutile proseguire su questo ramo.
            // (Calcolo semplificato per didattica: sommiamo uso attuale + 1 se la sala era vuota)
            // In un esame, questo controllo può essere omesso se troppo complesso, ma è richiesto dal testo.
            
            // APPLICAZIONE SCELTA (Do Move)
            cap_rimanente[i] -= P[gr_idx]; // Riduciamo capacità sala
            sale_usate_attualmente[i]++;   // Incrementiamo contatore gruppi in quella sala

            // CHIAMATA RICORSIVA
            solve_ricorsiva(gr_idx + 1, P, nP, cap_rimanente, nS, sale_usate_attualmente, best_sol);

            // BACKTRACKING (Undo Move)
            // Fondamentale: ripristiniamo la situazione com'era prima di entrare nella ricorsione
            // per permettere al ciclo 'for' di provare la sala successiva pulita.
            cap_rimanente[i] += P[gr_idx];
            sale_usate_attualmente[i] = old_uso;
        }
    }
}

/* =================================================================================
 * FUNZIONE: solve (Wrapper)
 * ---------------------------------------------------------------------------------
 * SCOPO:
 * Prepara le strutture dati ausiliarie e lancia la prima chiamata ricorsiva.
 * * PARAMETRI:
 * - int *S: Vettore capacità sale iniziali.
 * - int nS: Numero sale.
 * - int *P: Vettore gruppi.
 * - int nP: Numero gruppi.
 * * GESTIONE MEMORIA:
 * Dobbiamo duplicare il vettore S perché la ricorsione lo modificherà (sottraendo posti).
 * Non dobbiamo "sporcare" i dati originali del main.
 * =================================================================================
 */
void solve(int *S, int nS, int *P, int nP) {
    
    /* --- 1. ALLOCAZIONE STRUTTURE AUSILIARIE --- */
    
    // Allocazione array per tenere traccia della capacità residua durante i tentativi
    int *cap_rimanente = (int *)malloc(nS * sizeof(int));
    
    // Allocazione array per contare quanti gruppi ci sono in ogni sala (serve per capire se è usata)
    int *sale_usate_attualmente = (int *)malloc(nS * sizeof(int));

    // Controllo malloc
    if (cap_rimanente == NULL || sale_usate_attualmente == NULL) {
        printf("Errore malloc\n");
        exit(-1);
    }

    /* --- 2. INIZIALIZZAZIONE --- */
    
    for (int i = 0; i < nS; i++) {
        cap_rimanente[i] = S[i];    // Copiamo le capacità originali
        sale_usate_attualmente[i] = 0; // All'inizio tutte le sale sono vuote
    }

    // Variabile per il risultato migliore. La inizializziamo a un valore pessimo (es. nS + 1)
    // così la prima soluzione valida trovata sarà sicuramente minore.
    int best_sol = nS + 1;

    /* --- 3. CHIAMATA AL MOTORE RICORSIVO --- */
    // Passiamo l'indirizzo di best_sol (&best_sol) per modificarlo dentro.
    solve_ricorsiva(0, P, nP, cap_rimanente, nS, sale_usate_attualmente, &best_sol);

    /* --- 4. STAMPA RISULTATO --- */
    
    if (best_sol > nS) {
        printf("Nessuna soluzione trovata (i gruppi non entrano nelle sale).\n");
    } else {
        printf("Soluzione trovata! Numero minimo di sale utilizzate: %d\n", best_sol);
    }

    /* --- 5. FREE (Pulizia Memoria) --- */
    free(cap_rimanente);
    free(sale_usate_attualmente);
}

/* =================================================================================
 * MAIN DI TEST
 * =================================================================================
 * SCOPO:
 * Riprodurre l'esempio dell'immagine per verificare la correttezza.
 * * DETTAGLIO SINTATTICO:
 * Creiamo i vettori staticamente (es. int S[] = {...}) perché è un test rapido.
 * In un caso reale, potrebbero venire da input utente o file.
 * =================================================================================
 */
int main() {
    printf("--- ESERCIZIO PRENOTAZIONI SALE ---\n\n");

    /* DATI DELL'ESEMPIO */
    // nS = 4, S = {4, 9, 11, 5}
    int nS = 4;
    int S[] = {4, 9, 11, 5};

    // nP = 9, P = {2, 2, 3, 2, 6, 2, 4, 4, 3}
    int nP = 9;
    int P[] = {2, 2, 3, 2, 6, 2, 4, 4, 3};

    printf("Capacita' Sale: ");
    for(int i=0; i<nS; i++) printf("%d ", S[i]);
    printf("\n");

    printf("Gruppi: ");
    for(int i=0; i<nP; i++) printf("%d ", P[i]);
    printf("\n\n");

    // Invocazione Wrapper
    solve(S, nS, P, nP);

    return 0;
}