Analisi 
Modello Combinatorio: DISPOSIZIONI Semplici senza ripetizioni, ordine conta

Stiamo facendo una ricerca nello spazio degli stati (State Space Search).

Ogni "stato" è la stringa corrente.

Ogni ramo dell'albero è l'applicazione di una sostituzione disponibile.

Usiamo un vettore used perché assumiamo che le sostituzioni in S siano "risorse" consumabili (come carte in mano). Se il testo implicasse risorse infinite (es. puoi usare "a" quante volte vuoi), toglieremmo l'array used, ma attenzione ai loop infiniti!

Gestione delle Stringhe (strdup vs Modifica):

Nel codice ho usato strdup (duplicazione stringa) ad ogni passo ricorsivo.

Perché? Se modificassimo direttamente la stringa originale, dovremmo implementare una logica di "undo" (tornare indietro) molto complessa, ripristinando i caratteri sovrascritti.

Con strdup, creiamo una copia "pulita" per il prossimo livello. Quando la funzione ritorna (backtracking), facciamo free della copia e la versione precedente del chiamante rimane intatta. È meno efficiente come RAM, ma molto più sicuro per evitare bug logici durante un esame.

Il Criterio di Pruning:

if (curr_cost > *min_cost) return;

Questo è fondamentale. Se ho già trovato una soluzione che costa 7, e nel mio percorso attuale sono già a costo 8 senza aver finito, non ha senso continuare. Taglio il ramo (potatura).

Aritmetica dei Puntatori (next_str + S[i].pos):

Quando facciamo memcpy(next_str + S[i].pos, ...) stiamo dicendo: "Prendi l'indirizzo iniziale della stringa, aggiungi N byte (pos) e inizia a copiare da lì". È equivalente a &next_str[S[i].pos].