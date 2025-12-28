Analisi e Giustificazione del Modello

Prima di scrivere il codice, come richiesto dalla traccia, giustifichiamo le scelte:

Modello Combinatorio: Il problema è una variante del Set Cover o della Scomposizione di Stringhe. Dobbiamo trovare una sequenza ordinata di parole $w_1, w_2, ..., w_m$ tali che la loro concatenazione sia esattamente str.

Tecnica: Usiamo la Ricorsione con Backtracking (DFS - Depth First Search). Immagina la stringa target come un percorso da completare. A ogni passo, cerchiamo nel dizionario una parola che combaci con l'inizio della stringa rimanente (prefisso).

Gestione del Vincolo "s distinte": Per soddisfare la richiesta "almeno s stringhe diverse", dobbiamo tenere traccia di quante volte usiamo ogni parola del dizionario. Non basta un booleano (usato/non usato), serve un contatore, perché potrei usare la parola "BRA" due volte, e se faccio backtracking sulla seconda, la parola deve risultare ancora "in uso" dalla prima volta.

Criteri di Pruning (Potatura):Matching: Se una parola del dizionario non coincide esattamente con i caratteri attuali di str, la scartiamo subito (strncmp).Lunghezza: Se la parola del dizionario è più lunga di ciò che resta di str, la scartiamo.