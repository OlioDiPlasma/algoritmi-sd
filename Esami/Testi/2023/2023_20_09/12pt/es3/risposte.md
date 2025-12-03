commento sul risultato 
L'algoritmo ha verosimilmente confrontato due rami principali:
Ramo A: Inizio con "orso" (4 lettere)
-   orso (finisce V) $\to$ cerca parole che iniziano per C.
-   Opzioni: mela, rospo, tana.
-   Miglior sequenza possibile: orso $\to$ rospo $\to$ mela $\to$ tana (o variazioni dell'ordine delle ultime tre).
Totale: $4 + 5 + 4 + 4 = \mathbf{17}$.

Ramo B: Inizio con "altare" (6 lettere)
-   altare (finisce V) $\to$ cerca parole che iniziano per C.
-  Opzioni: mela, rospo, tana. (orso è escluso perché inizia per V).
-   Miglior sequenza: altare $\to$ mela $\to$ rospo $\to$ tana.
Totale: $6 + 4 + 5 + 4 = \mathbf{19}$.

**1.Giustificazione del modello combinatorio adottato**
Il problema richiede di trovare una sequenza di parole prese da un insieme dato.
-   L'ordine è fondamentale. Concatenare "mela" + "tana" produce una stringa diversa (e vincoli diversi sull'ultimo carattere) rispetto a "tana" + "mela".
-   Il testo dice "prese al più una volta". Non possiamo riusare la stessa parola.

Pertanto, il modello matematico di riferimento è quello delle Disposizioni Semplici. Nello spazio delle soluzioni stiamo esplorando un sottoinsieme delle permutazioni possibili, troncato dai vincoli


**2.Criteri di Pruning (Potatura)**
Nel codice abbiamo adottato un criterio di pruning fondamentale basato sull'Ammissibilità (Feasibility).

1. Il criterio: Prima di scendere in ricorsione aggiungendo la parola parole[i], verifichiamo immediatamente la funzione check_constraint.
2. Descrizione: Se l'ultima lettera della parola precedente e la prima della parola candidata sono entrambe vocali (o entrambe consonanti), il ramo viene "tagliato" (istruzione continue o mancata chiamata ricorsiva).
3. Perché: È inutile esplorare quel ramo, poiché qualsiasi soluzione generata a valle conterrebbe una violazione del vincolo "consecutivo", rendendo l'intera concatenazione invalida. Questo riduce drasticamente lo spazio di ricerca rispetto a generare tutte le permutazioni e controllarle solo alla fine.

