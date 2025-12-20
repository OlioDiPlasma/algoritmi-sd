#include "act.h"

/* --- 1. STRUTTURA ADT COMPLETA --- */
/*
 * Concetto Chiave: Wrapper
 * Questa struttura contiene tutti i dati necessari. 
 * 'bestPlan' e 'bestSum' servono per memorizzare il risultato della ricorsione.
 */
struct act {
    attivita_t *elenco_att;  // Vettore dinamico delle attività
    dep_t *elenco_dip;       // Vettore dinamico delle dipendenze
    int na;                  // Numero attività
    int nd;                  // Numero dipendenze
    
    // Variabili per l'ottimizzazione [cite: 134]
    int nMin;                // Numero minimo di persone calcolato
    float bestSum;           // Miglior somma (varianza minima) trovata
    int *bestPlan;           // Vettore soluzione ottima (indice attività -> indice persona)
};

/* --- 2. FUNZIONI DI UTILITÀ (NON RICHIESTE MA NECESSARIE) --- */

/*
 * Scopo: Allocazione della struttura principale.
 * Gestione Memoria: 
 * - malloc: alloca byte grezzi nell'heap.
 * - sizeof(*a): calcola la dimensione della struttura puntata da 'a'.
 * - Controllo NULL: fondamentale per evitare crash se la memoria è piena.
 */
ACT ACTinit(int na, int nd) {
    ACT a = malloc(sizeof(*a)); 
    a->na = na;
    a->nd = nd;
    // Allocazione vettori interni
    a->elenco_att = malloc(na * sizeof(attivita_t));
    a->elenco_dip = malloc(nd * sizeof(dep_t));
    // Inizializzazione campi ottimizzazione
    a->bestPlan = calloc(na, sizeof(int)); // calloc inizializza a 0
    a->bestSum = -1.0; // Valore sentinella
    return a;
}

void ACTfree(ACT a) {
    if (a == NULL) return;
    free(a->elenco_att);
    free(a->elenco_dip);
    free(a->bestPlan);
    free(a); // Libera il contenitore solo dopo aver liberato il contenuto
}

void ACTaddAttivita(ACT a, int index, char *nome, int start, int end, int cost) {
    strcpy(a->elenco_att[index].nome, nome);
    a->elenco_att[index].inizio = start;
    a->elenco_att[index].fine = end;
    a->elenco_att[index].costo = cost;
}

void ACTaddDipendenza(ACT a, int index, int act_idx, int dep_idx) {
    a->elenco_dip[index].act = act_idx;
    a->elenco_dip[index].dep = dep_idx;
}

/* Helper per ordinare interi (necessario per ACTminPers) */
void sortInt(int *v, int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (v[j] > v[j+1]) {
                temp = v[j]; v[j] = v[j+1]; v[j+1] = temp;
            }
        }
    }
}

/* --- 3. PARTE 1: VERIFICA (Problema di verifica) --- */

/*
 * Scopo: Verifica compatibilità temporale delle dipendenze.
 * Riferimento: [cite: 142]
 */
int ACTcheckDep(ACT a) {
    for (int i = 0; i < a->nd; i++) {
        int att = a->elenco_dip[i].act; // Chi dipende
        int dip = a->elenco_dip[i].dep; // Da chi si dipende
        
        // Se la dipendenza finisce dopo che l'attività inizia -> ERRORE
        // NOTA: accesso ai campi tramite -> per struct puntata, . per struct locale o array
        if (a->elenco_att[dip].fine > a->elenco_att[att].inizio)
            return 0; // Falso/Invalido
    }
    return 1; // Vero/Valido
}

/*
 * Scopo: Identificare se è Albero o DAG e stampare Source/Sink.
 * Concetti Chiave: 
 * - In-degree: quanti archi entrano (dipendenze).
 * - Out-degree: quanti archi escono (sono prerequisito per altri).
 * - calloc: usato qui per inizializzare i contatori a 0 automaticamente.
 * Riferimento: [cite: 142, 143]
 */
void ACTprintSrcSnk(ACT a) {
    int *in_degree = calloc(a->na, sizeof(int));
    int *out_degree = calloc(a->na, sizeof(int));
    int isTree = 1;

    // Calcolo gradi
    for (int i = 0; i < a->nd; i++) {
        // dep -> act (l'arco va dalla dipendenza all'attività che deve aspettare)
        if ((in_degree[a->elenco_dip[i].act]++) > 0)
            isTree = 0; // Se un nodo ha più di 1 genitore, non è un albero (nella def. stretta data)
        out_degree[a->elenco_dip[i].dep]++;
    }

    printf("Le dipendenze sono un %s\n", isTree ? "albero" : "DAG");

    printf("Root(s)/Source(s): ");
    for (int i = 0; i < a->na; i++)
        if (in_degree[i] == 0) printf("%s ", a->elenco_att[i].nome);
    printf("\n");

    printf("Leave(s)/Sink(s): ");
    for (int i = 0; i < a->na; i++)
        if (out_degree[i] == 0) printf("%s ", a->elenco_att[i].nome);
    printf("\n");

    // Pulizia memoria locale
    free(in_degree);
    free(out_degree);
}

/* --- 4. PARTE 2: OTTIMIZZAZIONE (Problema di ricerca) --- */

/*
 * Scopo: Calcola il numero minimo di persone usando l'algoritmo di overlap (sweep-line semplificato).
 * Logica:
 * 1. Mette tutti gli inizi e tutte le fini in due array.
 * 2. Li ordina.
 * 3. Scorre temporalmente: quando inizia una attività, serve una persona (+1). 
 * Quando finisce, si libera una persona (-1).
 * Riferimento: [cite: 156]
 */
int ACTminPers(ACT a) {
    int i, f, minPers = 0, cntPers = 0;
    // Allocazione dinamica array temporanei
    int *t_inizio = malloc(a->na * sizeof(int));
    int *t_fine = malloc(a->na * sizeof(int));

    // Copia dati
    for (i = 0; i < a->na; i++) {
        t_inizio[i] = a->elenco_att[i].inizio;
        t_fine[i] = a->elenco_att[i].fine;
    }

    // Ordinamento (necessario per la logica a scansione)
    sortInt(t_inizio, a->na);
    sortInt(t_fine, a->na);

    // Scansione (Two-pointer technique)
    // i punta agli inizi, f punta alle fini
    for (i = 0, f = 0; i < a->na; ) {
        // Se un'attività finisce PRIMA o NELLO STESSO MOMENTO in cui ne inizia un'altra
        if (t_fine[f] <= t_inizio[i]) {
            cntPers--; // Libero risorsa
            f++;
        } else {
            cntPers++; // Occupo risorsa
            i++;
            if (cntPers > minPers) minPers = cntPers;
        }
    }

    a->nMin = minPers; // Salva nel wrapper per usarlo dopo
    free(t_inizio);
    free(t_fine);
    return minPers;
}

/* * Funzioni Helper per la Ricorsione (Disposizioni con Ripetizione + Pruning)
 * Riferimento: [cite: 156]
 */

// Verifica se l'attività 'pos' assegnata alla persona 'p' collide con altre attività
// già assegnate alla stessa persona 'p'.
int pruningCheck(ACT a, int pos, int *sol, int p) {
    for (int i = 0; i < pos; i++) {
        // Se l'attività 'i' è assegnata alla persona 'p'
        if (sol[i] == p) {
            // Controllo sovrapposizione intervalli [Start, End]
            // Overlap condition: Start1 < End2 && Start2 < End1
            if (a->elenco_att[pos].inizio < a->elenco_att[i].fine &&
                a->elenco_att[i].inizio < a->elenco_att[pos].fine)
                return 0; // PRUNING: Incompatibile
        }
    }
    return 1; // Compatibile
}

// Aggiorna la soluzione ottima se la corrente è migliore
void checkBest(ACT a, int *sol, int *costi) {
    float s = 0.0, avg = 0.0;
    
    // Calcolo media costi per persona
    for (int i = 0; i < a->nMin; i++)
        avg += costi[i];
    avg /= a->nMin;

    // Calcolo somma scarti (obiettivo: minimizzare sbilanciamento)
    for (int i = 0; i < a->nMin; i++)
        s += fabs(costi[i] - avg); // fabs è valore assoluto float

    // Aggiornamento best solution
    if (a->bestSum < 0.0 || s < a->bestSum) {
        a->bestSum = s;
        for (int i = 0; i < a->na; i++)
            a->bestPlan[i] = sol[i];
    }
}

// Funzione ricorsiva (Disposizioni con ripetizione)
void disp_ripet(ACT a, int pos, int *sol, int *costi) {
    // Caso base: Tutte le attività assegnate
    if (pos >= a->na) {
        checkBest(a, sol, costi);
        return;
    }

    // Iterazione sulle persone disponibili (da 0 a nMin-1)
    for (int p = 0; p < a->nMin; p++) {
        // Pruning: posso dare l'attività 'pos' alla persona 'p'?
        if (pruningCheck(a, pos, sol, p)) {
            
            // Assegna
            sol[pos] = p;
            costi[p] += a->elenco_att[pos].costo;

            // Ricorsione
            disp_ripet(a, pos + 1, sol, costi);

            // Backtracking (annulla mossa)
            costi[p] -= a->elenco_att[pos].costo;
            // Non serve resettare sol[pos] perché verrà sovrascritta
        }
    }
}

/*
 * Scopo: Wrapper per lanciare la ricorsione dell'ottimizzazione.
 * Gestione Memoria: Alloca vettori temporanei 'sol' e 'costi'.
 * Riferimento: [cite: 156]
 */
void ACTbestPlan(ACT a) {
    // Soluzione corrente (indice persona per ogni attività)
    int *sol = calloc(a->na, sizeof(int));
    // Costo cumulativo per ogni persona
    int *costi = calloc(a->nMin, sizeof(int));

    a->bestSum = -1.0; // Reset best
    
    // Avvia ricorsione
    disp_ripet(a, 0, sol, costi);

    // Stampa Risultati
    printf("\nLa pianificazione ottima (Deviation: %.2f) e':\n", a->bestSum);
    for (int i = 0; i < a->na; i++) {
        printf("%-10s -> Persona %d (Costo: %d)\n", 
               a->elenco_att[i].nome, 
               a->bestPlan[i] + 1, // +1 per estetica (Persona 1 invece di 0)
               a->elenco_att[i].costo);
    }

    free(sol);
    free(costi);
}