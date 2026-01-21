/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////CALCOLO COMBINATORIO


/*
1. Modello Binario (Sottoinsiemi / Power Set / Combinazioni)
Logica: "Prendo o Lascio?"  "Seleziono"
Uso: Trovare un gruppo di elementi (es. "Zaino", "Somma uguale a K"). 
Caratteristica: Non c'è il ciclo for. 
L'albero si biforca sempre in 2.


Hai un elenco di 10 progetti aziendali. 
Ogni progetto ha un Costo e un Guadagno. 
Hai un Budget massimo di 1000€. Obiettivo: 
Seleziona il sottoinsieme di progetti che massimizza il guadagno totale senza sforare il budget.

 */

/*
 * index: indice dell'oggetto che stiamo valutando (da 0 a N-1)
 * curr_sol: vettore che contiene gli elementi presi finora
 * count: quanti elementi ho preso finora
 * val: vettore input con i valori (es. pesi, numeri)
 */
void solve_binaria(int index, int *curr_sol, int count, int N, int *val) {

    // --- PRUNING (Opzionale) ---
    // Esempio: Se ho già preso troppi elementi o superato un peso massimo
    // if (count > k || peso_attuale > max_peso) return;

    // --- BASE CASE ---
    if (index == N) {
        // Ho deciso per tutti gli N oggetti. Controllo se la soluzione è valida.
        if (check_validita(curr_sol, count)) {
            salva_soluzione(curr_sol, count);
        }
        return;
    }

    // --- RAMO 1: PRENDO l'elemento 'index' ---
    curr_sol[count] = val[index]; // Lo aggiungo al sacco
    solve_binaria(index + 1, curr_sol, count + 1, N, val);

    // --- RAMO 2: LASCIO l'elemento 'index' ---
    // Non faccio nulla su curr_sol, al prossimo "PRENDO" sovrascriverò questa posizione.
    // Passo al prossimo oggetto (index + 1) ma il count non aumenta.
    solve_binaria(index + 1, curr_sol, count, N, val);
}

///////////////////////////////////////////////////////
/*
2. Modello a Slot (Permutazioni / Disposizioni)
Logica: "Chi metto in questa posizione?" "Ordinamento/Sequenza"
Uso: Anagrammi, N-Regine, Classifiche, Commesso Viaggiatore (TSP). 
Caratteristica: C'è il ciclo for su tutti gli elementi. 
Serve mark[] per non ripetere.

Hai 5 città (A, B, C, D, E) e una matrice con le distanze tra loro. 
Obiettivo: Trova l'ordine di visita (es. A -> C -> E -> B -> D -> A) 
che minimizza i km totali percorsi, visitando ogni città una sola volta.

*/

/*
 * pos: la posizione (slot) che stiamo riempiendo adesso (0, 1, ... k-1)
 * curr_sol: vettore soluzione che stiamo costruendo
 * mark: vettore booleano (0/1) per ricordare chi è già stato usato
 * k: numero totale di slot da riempire (spesso k = N nelle permutazioni)
 */
void solve_slot(int pos, int *curr_sol, int *mark, int N, int k, int *val) {

    // --- BASE CASE ---
    if (pos == k) {
        // Ho riempito tutti i k slot. La soluzione è pronta.
        salva_soluzione(curr_sol, k);
        return;
    }

    // --- CICLO DELLE SCELTE ---
    // Per lo slot 'pos', provo a metterci OGNI elemento 'i' disponibile
    for (int i = 0; i < N; i++) {
        
        if (mark[i] == 0) { // Se l'elemento 'i' NON è ancora stato usato
            
            // 1. DO (Faccio la mossa)
            mark[i] = 1;          // Lo segno come usato
            curr_sol[pos] = val[i]; // Lo metto nello slot
            
            // 2. RECURSE (Vado avanti)
            // Nota: passo 'pos + 1' perché ora devo riempire il prossimo slot
            solve_slot(pos + 1, curr_sol, mark, N, k, val);
            
            // 3. BACKTRACK (Disfaccio la mossa)
            // Fondamentale: libero l'elemento 'i' per poterlo usare in altre combinazioni
            mark[i] = 0; 
        }
    }
}

/*Nota: Se l'esercizio è "Disposizioni con ripetizione" (posso riusare i numeri), basta togliere tutto ciò che riguarda mark.*/


//////////////////////////////////

/*
3. Modello Partizioni (Palle nelle Scatole)
Logica: "Metto in una scatola vecchia o apro una nuova?" "Raggruppamento"
Uso: Suddividere un insieme in gruppi (es. Squadre, Bin Packing). 
Caratteristica: Ciclo for dinamico (fino a n_scatole) + 1 chiamata fuori ciclo.


Hai N file da salvare e un numero imprecisato di Hard Disk. Ogni disco ha capienza 1TB.
Obiettivo: Raggruppa i file in modo da usare il minor numero di dischi possibile.
*/

/*
 * index: l'oggetto che devo sistemare adesso (da 0 a N-1)
 * sol: qui sol[index] = numero della scatola dove ho messo l'oggetto index
 * n_scatole: quante scatole ho aperto/utilizzato finora
 */
void solve_partizioni(int index, int *sol, int n_scatole, int N) {

    // --- BASE CASE ---
    if (index == N) {
        // Ho sistemato tutti gli oggetti.
        stampa_partizione(sol, N, n_scatole);
        return;
    }

    // --- OPZIONE A: Metto in una scatola GIÀ ESISTENTE ---
    // Provo a metterlo nella scatola 0, poi nella 1... fino all'ultima aperta
    for (int i = 0; i < n_scatole; i++) {
        sol[index] = i; // Assegno l'oggetto 'index' alla scatola 'i'
        
        // Ricorsione: passo al prossimo oggetto (index+1), 
        // il numero di scatole aperte (n_scatole) NON cambia
        solve_partizioni(index + 1, sol, n_scatole, N);
    }

    // --- OPZIONE B: Apro una NUOVA SCATOLA ---
    // Metto l'oggetto nella prima scatola libera (che ha indice 'n_scatole')
    sol[index] = n_scatole;
    
    // Ricorsione: passo al prossimo oggetto (index+1),
    // il numero di scatole aperte AUMENTA di 1
    solve_partizioni(index + 1, sol, n_scatole + 1, N);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// MISTO DI PROGRAMMAZIONE








