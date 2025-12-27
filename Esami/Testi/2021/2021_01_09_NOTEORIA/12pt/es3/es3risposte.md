1. Analisi del Modello Combinatorio
Modello Adottato: Si tratta di un modello basato sulle Disposizioni con ripetizione (o più precisamente, un'esplorazione esaustiva su Albero di Decisione).

Giustificazione:

Il problema richiede di costruire una sequenza di parti.

Lo stato della ricorsione è rappresentato dall'indice corrente nella stringa target.

Ad ogni passo, le scelte possibili sono limitate solo alle parti che iniziano esattamente a quell'indice (P[i].pos == current_index).

Ogni scelta ci porta in un nuovo stato (nuovo indice = current_index + lunghezza_parte), creando una struttura ad albero dove ogni ramo è una potenziale soluzione.

2. Criteri di Pruning (Potatura)
Criterio Adottato: Pruning sul Costo Minimo.

Condizione: Interrompiamo la ricorsione se costo_attuale > costo_minimo_globale.

Giustificazione: Dato che i costi sono interi positivi, il costo è monotono crescente. Se il costo parziale ha già superato la migliore soluzione completa trovata finora, continuando ad aggiungere parti il costo potrà solo aumentare, rendendo impossibile trovare un nuovo ottimo in quel ramo.