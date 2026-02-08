L'esercizio chiede di trovare il numero minimo di gruppi di amici. In termini matematici, questo equivale al problema della Partizione in Clique (Clique Partitioning). Una "clique" è un sottografo in cui tutti i nodi sono collegati tra loro (tutti sono amici di tutti).

Il problema è complesso (NP-Hard), quindi useremo la ricorsione con backtracking per esplorare le soluzioni, applicando il Pruning (potatura) per tagliare i rami inutili e velocizzare l'esecuzione.

Il modello combinatorio utilizzato è quello delle Partizioni di un Insieme (Set Partitions): ogni elemento (persona) deve appartenere esattamente a un sottoinsieme (gruppo).


Disposizioni con ripetizione circa
