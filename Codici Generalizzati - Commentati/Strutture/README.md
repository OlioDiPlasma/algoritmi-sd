
**Commento** 
Vorrei fare un ringraziamento https://github.com/giacomodandolo/Algoritmi_e_Strutture_Dati/tree/main/Strutture_Dati da cui ho rubato le strutture dati

Ora è importante sottolineare la differenza tra Interfaccia e Implementazione

A qualcuno verrebbe da chiedersi ma come mai ci sono 2/3 sottocartelle per ogni "Struttura", questo perchè la "Struttura" è un ADT = l'interfaccia mentre quelle sottocartelle rappresentano l'implementazione

L'ADT dice soltanto il comportamento e i metodi principali che una funzione dovrebbe avere, mentre le implementazioni decidono loro come implementarli. 

Questo spiega perchè ad esempio la Lista Ordinata sta sotto la Priority_Queue, questo perchè la Lista Ordinata implementa l'interfaccia di Priority_Queue e per fare ciò sfrutta il concetto di lista ordinata come strumento per farlo.

Chiarisco meglio com'è strutturata la cartella visto che potrebbe essere confusionaria : 

# ADT
Questi sono i "concetti" principali, che definiscono un set di operazioni (un'interfaccia) indipendentemente dall'implementazione.

## 1. Stack (Pila - LIFO)
Implementa una logica Last-In, First-Out.
-   `ADT_I_Classe/`: Implementazione robusta come ADT di I classe (puntatore opaco) usando una Lista Linkata.
-   `Quasi_ADT/`: Implementazione "pericolosa" che usa una variabile globale statica. Può esistere una sola pila in tutto il programma.

## 2. Queue (Coda - FIFO)
Implementa una logica First-In, First-Out.
-   `Lista/`: Implementazioni basate su Lista Linkata.
    - `ADT_I_Classe/`: Implementazione O(1) con puntatori head e tail.
    - `Quasi_ADT/`: Implementazione O(1) con variabili globali head e tail (pericolosa).
-   `Vettore/`: Implementazioni basate su Vettore Circolare.
    - `ADT_I_Classe/`: Implementazione O(1) con head, tail e un array N+1.
    - `Quasi_ADT/`: Implementazione O(1) con variabili globali (pericolosa).

## 3. Lista
Implementazione di una Lista Linkata generica con operazioni standard (inserimento in testa, in coda, cancellazione, ecc.).

## 4. Priority Queue (Coda con priorità)
Un ADT in cui ogni elemento ha una priorità. extractMax (o Min) è l'operazione chiave.

-   `Heap/`: Implementazione efficiente (O(log N)) basata su Heap.
    - `Base/`: Uno heap standard.
    - `Di_indici/`: Uno Heap Indiretto (o Indicizzato) con un array qp, che permette un'operazione `changePriority` in O(log N) (fondamentale per Dijkstra).
-   `Lista_ordinata/`: Implementazione inefficiente.
    - `PQinsert` è O(N) (lento).
    - `PQextractMax` è O(1) (veloce).

## 5. Heap
È la struttura dati "pura" usata per implementare la `Priority_Queue` o per l'Heapsort.

## 6. Set (Insieme)
Un ADT che memorizza una collezione di elementi unici.

-   `Lista_non_ordinata/`: Implementazione semplice.
    - Ricerca e Unione/Intersezione sono lente (O(N) e O(N²)).
-   `Vettore_ordinato/`: Implementazione efficiente per le operazioni sugli insiemi.
    - Ricerca è O(log N) (ricerca binaria).
    - Unione e Intersezione sono O(N+M) (algoritmo di merge).

## 7. Symbol Table (ST) - (Tavola dei Simboli)
L'ADT più complesso, usato per memorizzare coppie (Chiave, Valore). Mostra il maggior numero di trade-off.
-   `Lista_non_ordinata/`: La più semplice. Inserimento O(1), Ricerca O(N).
-   `Lista_ordinata/`: Inserimento O(N), Ricerca O(N) (non può usare ricerca binaria).
-   `Vettore_non_ordinato/`: Inserimento O(1) (in coda), Ricerca O(N).
-   `Vettore_ordinato/`: Inserimento O(N) (deve shiftare), Ricerca O(log N) (binaria).
-   `Direct_Access/`: (Accesso Diretto) Implementazione speciale in cui la chiave è l'indice.
    - Inserimento e Ricerca sono O(1).
    - Spreca molta memoria se le chiavi non sono dense (es. da 0 a M-1).
-   `Hash_Table/`: L'implementazione più comune (O(1) in media).
    - `Linear_Chaining/`: (Liste di Trabocco) Gestisce le collisioni con liste linkate.
    - `Open_Addressing/`: (Indirizzamento Aperto) Gestisce le collisioni cercando la cella libera successiva nell'array stesso.
        - `Linear_Probing/`: Cerca $(i + 1) % M.$
        - `Quadratic_Probing/`: Cerca $(i + c1*j + c2*j^2) % M.$
        - `Double_Hashing/`: Cerca $(i + j*hash2(k)) % M.$

## 8. Binary Search Tree (BST)
Implementazioni di Alberi Binari di Ricerca.
-   `Basic/`: Un BST standard (ricerca, inserimento, ecc.).
-   `Extended/`: Un BST potenziato con un puntatore al genitore (p) e la dimensione del sottoalbero (N), che permette operazioni avanzate come select (O(log N)) e succ/pred (O(log N)).
-   `Interval_Binary_Search_Tree/`: Un albero specializzato per memorizzare intervalli e trovare sovrapposizioni (overlap) in modo efficiente.


## 9. Graph (Grafo)
Implementazione dell'ADT Grafo.
-   `Not_Oriented/`: Grafo Non Orientato (es. A-B), implementato con Matrice di Adiacenza simmetrica.
-   `Oriented/`: Grafo Orientato (es. A->B), implementato con Matrice di Adiacenza (potenzialmente asimmetrica).

## 10. Union-Find (DSU)
Implementazione di un Insieme Disgiunto (Disjoint Set Union) con l'ottimizzazione Weighted Quick-Union (unione ponderata). Usato per tracciare le componenti connesse (es. nell'algoritmo di Kruskal).


# Supporto

## 1. Moduli di Supporto (Helpers)
Questi file non sono ADT, ma sono "strumenti" usati dagli ADT o algoritmi a sé stanti.

-   `Item/`: Definisce il tipo di dato (`Item`) che viene memorizzato in tutte le altre strutture (es. int, struct, char*). È il cuore dell'astrazione dei dati.
- `Graph/Edge/`: Definisce la struttura `Edge` (Arco), usata dall'ADT Grafo per rappresentare le connessioni.
- `Graph/Algoritmi_Visita.c/h`: Fornisce le implementazioni delle visite *DFS* e *BFS* (che sono algoritmi che operano sull'ADT Grafo).
- `Symbol_Table/Hash_Table/Hash.c`: Fornisce una collezione di diverse *funzioni di hashing* (es. Modulare, Horner, Universale) usate dalle implementazioni Hash Table.

## 2. Algoritmi di Ordinamento**
Questa cartella contiene implementazioni pure di algoritmi di ordinamento che operano su un array di Item.

-   `Iterativi/`: Algoritmi O(N²) (Bubble, Selection, Insertion) e lo Shell Sort. Include anche il Counting Sort (O(N+k)).
-   `Ricorsivi/`: Algoritmi O(N log N) (QuickSort, MergeSort) e la versione iterativa "Bottom-Up" del MergeSort.

