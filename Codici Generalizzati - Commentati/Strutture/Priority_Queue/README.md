**Priority Queue** - L'interfaccia
Struttura dati astratta dove ogni elemento ha una priorità, e gli elementi con priorità più alta vengono estratti prima.

Le due operazioni fondamentali sono:
insert(x) — inserisce un nuovo elemento con priorità;
extractMax() o extractMin() — estrae l’elemento con priorità massima (o minima).


**Heap** (Struttura grande - Molti inserimenti)
Scelta ocmune ed efficiente

E' un albero binario quasi completo
in un max-heap, ogni nodo ≥ dei figli (la radice è il massimo);
in un min-heap, ogni nodo ≤ dei figli (la radice è il minimo).

*Base* - Semplice e veloce
Classico, elementi memorizzati direttamente nell'array secondo la priorità dell'heap

*Di_Indici* - Aggiornare priorità (Dijkstra,Prim)
L'heap memorizza i puntatori o indici a un array esterno di elementi



**Lista Ordinata** (Struttura piccola - Molte estrazioni)
Mantiene l'ordine degli elementi


**Lista Non Ordinata**  (Poche estrazioni)
Esiste ma non viene usata di solito