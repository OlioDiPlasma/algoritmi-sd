**IMPORTANTE** : 
unico main per simulare esame, niente modularità


**Obiettivo e Algoritmo**:  Il progetto risolve il problema di "Assegnazione Ottima delle Risorse con Sinergie" (Esame 21/02/23) determinando l'allocazione di $P$ persone su $T$ incarichi che massimizza la resa totale. L'algoritmo utilizzato è un Backtracking su modello delle Disposizioni con Ripetizione ($T^P$), che valuta ogni configurazione completa sommando i contributi individuali e le sinergie di coppia, applicando un taglio (clamping) se si supera il valore del task o azzerando la resa se non si raggiunge la soglia minima del 75%.

**Struttura Codice**: Il codice è organizzato secondo un approccio modulare logico all'interno di un unico source file per agevolare la compilazione d'esame.
-   Strutture Dati: struct Problema (incapsula vettori $D, E$ e matrice $S$) e struct Soluzione (vettore assegnazioni e score).
- Logica Core: solve_rec esplora lo spazio delle soluzioni assegnando ricorsivamente ogni persona a un task; valuta_soluzione implementa la logica di business (calcolo esperienze + sinergie, verifica soglia 0.75, calcolo min tra forza lavoro e valore task).
-   Gestione I/O: carica_dati gestisce il parsing del file e l'allocazione dinamica; libera_problema previene memory leaks.


input.txt :
3          <-- Numero di Incarichi (T)
10 6 8     <-- Valore/Difficoltà di ogni incarico (D)
4          <-- Numero di Persone (P)
3 2 5 3    <-- Livello di Esperienza di ogni persona (E)
0 2 1 4    <-- Riga 0 della Matrice di Sinergia (S)
2 0 4 1    <-- Riga 1
1 4 0 3    <-- Riga 2
4 1 3 0    <-- Riga 3

OUTPUT
=== RISULTATO OTTIMO ===
Resa Totale Massima: 18        <-- Valore della funzione obiettivo massimizzato
Dettaglio Assegnazioni:
- Persona 0 -> Incarico 0      <-- Mapping specifico (Indice Persona -> Indice Task)
- Persona 1 -> Incarico 2
- Persona 2 -> Incarico 2
- Persona 3 -> Incarico 0

