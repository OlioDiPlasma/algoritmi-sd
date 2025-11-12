
**Commento** 
Questa parte si concentra su alcuni algoritmi importanti per l'orale

## Argomenti

**1. Codifica di Huffman (Huffman.c)**
È un esempio fondamentale di algoritmo greedy che utilizza una coda con priorità (min-heap) per costruire un albero di codici.

Teoria : 
-   Definizione di codici liberi da prefisso (o "codice prefisso") e spiegazione delle loro caratteristiche.
-   Definizione specifica dei Codici di Huffman.
Programmazione: 
-   Spiegare come funzionano i codici liberi da prefisso.
-   Domanda specifica: I codici a lunghezza fissa sono considerati liberi da prefisso?
-   Quando conviene utilizzare codici a lunghezza fissa e quando quelli a lunghezza variabile (ad esempio, in base alla frequenza dei caratteri)?
-   C'è perdita di informazione o di dati utilizzando i codici di Huffman?
-   Fornire un esempio di una codifica che NON è libera da prefisso.
-   Quale paradigma (es. Greedy) utilizzano i codici di Huffman e quale funzione di appetibilità si sceglie?
-   Descrivere ad alto livello come si implementa o come funziona il procedimento per creare i codici di Huffman.

Programmazione : 
-   Scrivere l'algoritmo (o la funzione) che crea i codici di Huffman.
-   Implementare la costruzione di un albero dei codici di Huffman.
-   Implementazione generica dei codici di Huffman.


**2. Algoritmo di Karatsuba (Karatsuba.c)**
Hai un file per la moltiplicazione ricorsiva di interi (05recursive_integer_product.c), ma quello è l'algoritmo "ingenuo" O(N²). Le domande d'esame chiedono specificamente di Karatsuba, l'ottimizzazione Divide et Impera che riduce i prodotti da 4 a 3 e raggiunge una complessità O(N^log2(3))

Teoria : 
-   Spiegare qual è il vantaggio dell'algoritmo di Karatsuba rispetto all'algoritmo ricorsivo tradizionale (o generico) per il prodotto di due interi.
-   Descrivere l'algoritmo e specificare perché è migliore.
-   Viene chiesto esplicitamente cosa "migliora" Karatsuba: la risposta attesa è che riduce il numero di moltiplicazioni ricorsive da 4 a 3
-  Descrivere la manipolazione algebrica che consente di passare da 4 a 3 sottoproblemi (prodotti).
-   Definire la complessità dell'algoritmo di Karatsuba.
-   Fornire l'equazione alle ricorrenze per la complessità di Karatsuba.
-   Confrontare la complessità di Karatsuba (che è $O(N^{\log_2(3)})$) con quella dell'algoritmo "senza nome" (ingenuo), che è $O(N^2)$.
-   Spiegare come l'algoritmo si inserisce nel paradigma Divide et Impera.
-   Specificare per quale problema si applica (moltiplicazione di interi).


**3. Punti di Articolazione e Ponti (ArticulationPoints.c)**
Il tuo Graph.h ha le visite base (DFS/BFS) e le componenti (connesse/fortemente connesse), ma le domande chiedono anche di punti di articolazione (vertici la cui rimozione aumenta il numero di componenti connesse) e ponti (archi con la stessa proprietà). Questi richiedono un'applicazione specifica (e non banale) della DFS.

Teoria : 
-   Cos'è un punto di articolazione?
-   Come si trovano i punti di articolazione?
-   Quale algoritmo si utilizza per determinarli? (La risposta attesa è una visita DFS specifica).
-   In che tipo di grafi si trovano?
-   La presenza di punti di articolazione implica la presenza di bridge?
-   La presenza di bridge implica la presenza di punti di articolazione?
-   Cos'è un arco "bridge" (ponte)?
-   Come si trovano gli archi bridge?
-   Qual è la complessità dell'algoritmo per trovarli?
-   Come si determina se un dato arco è un bridge? (Suggerimento: togli l'arco e verifica la connessione con una DFS).
-   Come si trova una coppia di archi che insieme (se rimossi entrambi) sconnettono il grafo?

Programmazione : 
-   Scrivere una funzione che, dato un grafo, verifica se un vertice specifico è un punto di articolazione.
-   Se si implementa la funzione precedente, cosa succede o come ci si comporta se il grafo era già sconnesso in partenza? (Bisogna verificarlo prima).
-   Dato un grafo, determinare gli archi bridge.
-   Dato un grafo non orientato e pesato, verificare se esiste almeno un arco bridge. Trovare il peso minimo tra tutti gli archi bridge rilevati.



**4. Cammino/Ciclo di Eulero (Euler.c)**
Mentre hai il Cammino di Hamilton, manca il Cammino di Eulero (un cammino che visita ogni arco una sola volta). È un altro algoritmo classico su grafi che si basa sul controllo dei gradi dei vertici.

Teoria : 
-   Cos'è un cammino di Eulero?
-   È un cammino semplice?
-   Qual è la condizione (o i lemmi corrispondenti) affinché esista un cammino di Eulero in un grafo?
-   Cos'è un multigrafo?

**5. Controllo Grafo Bipartito (Bipartite.c)**
Una domanda classica sono i grafi bipartiti. Il test per verificare se un grafo è bipartito (colorabile con 2 colori) è un'applicazione standard di BFS o DFS.

Teoria : 
-   Cos'è un grafo bipartito?
-   Cos'è un bipartizionamento di un grafo? (Viene descritto come due insiemi di vertici che non hanno vertici in comune con quelli dell'insieme stesso) .
-   Fornire un esempio di applicazione dei grafi bipartiti.
-   Qual è la complessità di un algoritmo per verificare se un grafo è bipartito, date le due partizioni ipotetiche?

Programmazione : 
-   Scrivere una funzione per controllare se un grafo è bipartito.
-   Scrivere una funzione di verifica per controllare se una partizione generata da un modello combinatorio dà origine a un grafo bipartito.
-   Calcolare tutte le bipartizioni possibili di un grafo.
-   Trovare tutte le possibili partizioni (se esistono) di un grafo bipartito.
-   Generare un bipartizionamento (se esiste) e prendere quello con la minima differenza di elementi tra i due insiemi.
-  Trovare ricorsivamente la partizione di cardinalità massima che renda il grafo bipartito.
-    Viene chiesto esplicitamente di usare un modello combinatorio (es. disposizioni ripetute) per generare le partizioni e poi usare una funzione di verifica.

**6. Selezione di Attività (ActivitySelection.c)**
Questo è l'esempio per eccellenza di un algoritmo greedy con funzione di appetibilità statica. È un problema molto semplice ma concettualmente importante che manca nella tua libreria di esempi.

Teoria : 
-   Definizione del problema della "Selezione di Attività" (Activity Selection). Viene citato anche come "Interval Selection" (selezione di intervalli).
-   Quali algoritmi e approcci si possono usare per risolvere questo problema?
-   Domanda di ragionamento: Cosa succede se si vuole massimizzare il tempo complessivamente coperto dalle attività, invece del numero di attività? In questa variante, l'approccio Greedy garantisce ancora la soluzione ottima? (Risposta: No) .



**7. Alberi N-ari (Left-Child, Right-Sibling) (LCRS.c)**
Le domande menzionano alberi N-ari, alberi ternari, o alberi "Left-Child Right-Sibling". La tua libreria di BST è ottima, ma un'implementazione esplicita di un albero N-ario (spesso fatto con la tecnica LCRS) sarebbe un'aggiunta utile.

Teoria : 
-   Spiegare il significato di "Left-Child, Right-Sibling" (LCRS)
-   Definire il "grado" in un albero N-ario.
-   Qual è la differenza tra una rappresentazione LCRS e un BST (Albero Binario di Ricerca)?
-   Domanda concettuale (solo a voce): Come trasformeresti un albero N-ario in un albero binario? (La risposta implicita è usare la tecnica LCRS)

Programmazione : 
-   Definire la struct per rappresentare un albero N-ario (ennario).
-   Scrivere una funzione che calcola il numero totale di nodi in un albero N-ario.
-   Dato un albero N-ario in cui ogni nodo contiene un vettore con i suoi figli, calcolare quanti figli ci sono al livello k dell'albero.
-   Dato un albero LCRS, riempire per ogni nodo un campo che indica il numero di figli "diretti" che ha.
-   Dato un albero LCRS, calcolare il numero massimo di figli (grado).
-   Dato un albero ternario, scrivere una funzione che calcoli quanti cammini di lunghezza dispari sono presenti tra la radice e le foglie.

**8. Programmazione Dinamica (con LIS) (DynamicProgramming.c)**
Questo è forse il paradigma più richiesto dopo Greedy e D&I. Le domande sulla PD, sulla sottostruttura ottima e sull'esempio classico della "Longest Increasing Subsequence" (LIS) sono molto comuni.

Teoria:
-   Definizione di Programmazione Dinamica (PD).
-   Quali sono le condizioni di applicabilità? (Sottostruttura ottima, sottoproblemi sovrapposti).
-   Spiegare la differenza con Divide et Impera.
-   Differenza tra Memoization (top-down) e approccio bottom-up.
-   Definizione del problema LIS (Longest Increasing Subsequence).
-   Come si dimostra che LIS ha una sottostruttura ottima?
-   I cammini minimi sono un problema di PD? E i cammini massimi? (Domanda classica: Bellman-Ford sì, cammini massimi generici no, su DAG sì).

Programmazione:
-   Scrivere una funzione per risolvere LIS (sia con modello combinatorio/ricorsivo semplice, sia con la soluzione O(N²) di PD).

**9. MST (Kruskal e Prim) e ADT Union-Find (MST.c, UnionFind.c)**
Un altro argomento importantissimo sui grafi. Le domande sul Teorema degli Archi Sicuri, Kruskal e l'ADT Union-Find (che serve a Kruskal) sono onnipresenti.

Teoria:
-   Definizione di Albero Ricoprente Minimo (MST). Su che tipo di grafi si applica? (Connessi, non orientati, pesati).
-   Enunciare il Teorema e il Corollario degli Archi Sicuri.
-   Definizione di "taglio" (cut) di un grafo.
-   Spiegare l'algoritmo di Kruskal. A quale paradigma appartiene? (Greedy).
-   Spiegare l'algoritmo di Prim. A quale paradigma appartiene? (Greedy).
-   Definizione dell'ADT Union-Find. A cosa serve?
-   Come si implementa? (Quick Find, Quick Union, Weighted Quick Union).
-   Complessità delle operazioni find e union nelle varie implementazioni.

Programmazione:
-   Implementare le funzioni UFunion e UFfind (spesso è richiesta la versione "weighted" o con "path compression").
-   (Meno comune scrivere l'algoritmo intero, più facile la teoria o l'implementazione di Union-Find).

**10. Cammini Minimi (Dijkstra e Bellman-Ford) (ShortestPath.c)**
Insieme a MST, è l'altro grande problema di ottimizzazione su grafi. Le domande sulle differenze tra i due algoritmi e sul concetto di "relaxation" sono un must.

Teoria:
-   Definizione del problema dei cammini minimi (single-source).
-   Cos'è l'operazione di "Relaxation" (Rilassamento)?
-   Descrivere l'algoritmo di Dijkstra:
-   A quale paradigma appartiene? (Greedy).
-   Quale struttura dati usa per essere efficiente? (Coda a Priorità).
-   Quali sono i suoi limiti? (Non funziona con archi di peso negativo).
-   Descrivere l'algoritmo di Bellman-Ford:
-   A quale paradigma appartiene? (Programmazione Dinamica).
-   Perché esegue V-1 iterazioni?
-   Come rileva la presenza di cicli negativi?
-   Quando si usa al posto di Dijkstra?

Programmazione:
-   (Molto rari da implementare per intero all'orale, la teoria è predominante. Più probabile una domanda su una struttura dati usata, es. PQchange).

**11. Tabelle Hash (Gestione Collisioni) (Hashing.c)**
Questo argomento (insieme a BST e Heap) è il pilastro delle domande sulle "Strutture Dati". Le domande su collisioni, cancellazione e "fattore di carico" sono le più gettonate.

Teoria:
-   Cos'è una collisione?
-   Metodi di gestione (Open Addressing vs. Linear Chaining).
-   Definizione di "Fattore di Carico" (alpha): come si calcola e che valori assume nei due metodi?
-   Open Addressing:
    Linear Probing: come funziona? Cos'è il "clustering primario"?
-   Quadratic Probing: come funziona? Cos'è il "clustering secondario"?
-   Double Hashing: come funziona? Perché è considerato il migliore?
-   Come si gestisce la cancellazione in Open Addressing? (Domanda frequentissima: non basta liberare la cella, serve un "lazy deletion" / flag).
-   Come si gestisce la cancellazione in Linear Chaining? (È una semplice cancellazione da lista).

Programmazione:
-   Implementare STinsert, STsearch e STdelete con Linear Chaining.
-   Implementare STinsert, STsearch e STdelete con Linear Probing.

**12. Ordinamento (Quicksort & Mergesort) (Sorting.c)**
La tua lista non include algoritmi di ordinamento. Quicksort è forse la singola domanda più frequente del PDF, specialmente riguardo al suo caso peggiore. Mergesort è importante per la stabilità.

Teoria:

-   Quicksort:
-   Descrivere l'algoritmo (paradigma Divide et Impera).
-   Cos'è il caso peggiore? Quando si verifica? (Vettore già ordinato/inverso, pivot sempre minimo/massimo).
-   Qual è l'equazione alle ricorrenze e la complessità del caso peggiore? ($T(n) = T(n-1) + O(n) \rightarrow O(N^2)$).
-   Come si può mitigare? (Pivot random, median-of-three).
-   È un algoritmo stabile? (No). È "in loco"? (Sì).
-   Mergesort:
-   Descrivere l'algoritmo.
-   Complessità (caso ottimo, medio, peggiore)? ($O(N \log N)$ sempre).
-   Equazione alle ricorrenze? ($T(n) = 2T(n/2) + O(n)$).
-   È un algoritmo stabile? (Sì, se implementato correttamente). È "in loco"? (No).

Programmazione:
-   Scrivere la funzione partition dell'algoritmo Quicksort.
-   (Meno comune) Scrivere la funzione merge dell'algoritmo Mergesort.