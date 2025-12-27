* Modello Combinatorio: Disposizioni con ripetizione (limitata).
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