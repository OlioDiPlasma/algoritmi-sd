
**Commento** 
Questa parte si concentra su alcuni algoritmi importanti per l'orale


1. Codifica di Huffman (Huffman.c)
È un esempio fondamentale di algoritmo greedy che utilizza una coda con priorità (min-heap) per costruire un albero di codici.

2. Algoritmo di Karatsuba (Karatsuba.c)
Hai un file per la moltiplicazione ricorsiva di interi (05recursive_integer_product.c), ma quello è l'algoritmo "ingenuo" O(N²). Le domande d'esame chiedono specificamente di Karatsuba, l'ottimizzazione Divide et Impera che riduce i prodotti da 4 a 3 e raggiunge una complessità O(N^log2(3))

3. Punti di Articolazione e Ponti (ArticulationPoints.c)
Il tuo Graph.h ha le visite base (DFS/BFS) e le componenti (connesse/fortemente connesse), ma le domande chiedono anche di punti di articolazione (vertici la cui rimozione aumenta il numero di componenti connesse) e ponti (archi con la stessa proprietà). Questi richiedono un'applicazione specifica (e non banale) della DFS.

4. Cammino/Ciclo di Eulero (Euler.c)
Mentre hai il Cammino di Hamilton, manca il Cammino di Eulero (un cammino che visita ogni arco una sola volta). È un altro algoritmo classico su grafi che si basa sul controllo dei gradi dei vertici.

5. Controllo Grafo Bipartito (Bipartite.c)
Una domanda classica sono i grafi bipartiti. Il test per verificare se un grafo è bipartito (colorabile con 2 colori) è un'applicazione standard di BFS o DFS.

6. Selezione di Attività (ActivitySelection.c)
Questo è l'esempio per eccellenza di un algoritmo greedy con funzione di appetibilità statica. È un problema molto semplice ma concettualmente importante che manca nella tua libreria di esempi.

7. Alberi N-ari (Left-Child, Right-Sibling) (LCRS.c)
Le domande menzionano alberi N-ari, alberi ternari, o alberi "Left-Child Right-Sibling". La tua libreria di BST è ottima, ma un'implementazione esplicita di un albero N-ario (spesso fatto con la tecnica LCRS) sarebbe un'aggiunta utile.

