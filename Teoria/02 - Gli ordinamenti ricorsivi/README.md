# 02 — Gli ordinamenti ricorsivi

## 2.1 Parte Teorica
### 2.1.1 Generalizzazione dei Dati
Allocare una variabile vuol dire associarvi una porzione di memoria 
Negli algoritmi di ordinamento, i dati da ordinare non sono necessariamente solo numeri interi. Per generalizzare, si definisce un tipo Item (spesso una struct), che contiene:
- Un campo che funge da chiave di ordinamento.
- Altri campi che rappresentano dati aggiuntivi.

Per operare su questo tipo Item, si definiscono funzioni di interfaccia (come un "Quasi ADT") per l'accesso alla chiave, la lettura/scrittura e gli operatori relazionali (es. ITEMlt, ITEMeq).

```
/*
 * 2.1.1: Esempio di "Quasi ADT" (Tipologia 1)
 * La chiave coincide con l'item stesso.
 */
 
// Definizione del tipo Item 
typedef int Item; 

// Prototipi delle funzioni di interfaccia 
Item ITEMScan();
int ITEMeq (Item A, Item B);
int ITEMlt (Item A, Item B);
void ITEMShow (Item A);
// ... altre funzioni ...

// Implementazione di una funzione
int ITEMlt(Item A, Item B) {
  return (A<B);
}
```

### 2.1.2 Merge Sort (von Neumann, 1945)
Il Merge Sort è un algoritmo di ordinamento ricorsivo basato sul paradigma Divide et Impera.
1. Divide (Dividi): Il vettore viene diviso in due sottovettori, Sinistro (SX) e Destro (DX), rispetto al punto centrale6. Questo ha costo $\Theta(1)$.
2. Impera (Risolvi): L'algoritmo viene richiamato ricorsivamente sui due sottovettori (SX e DX).
    - Condizione di Terminazione: La ricorsione si ferma quando un sottovettore ha 0 o 1 elemento, poiché è già considerato ordinato.
3. Combina (Combina): I due sottovettori, ora ordinati, vengono fusi (con la funzione Merge) per creare un unico vettore ordinato. Questa fase ha costo $\Theta(n)$.

L'implementazione C tipicamente usa una funzione "wrapper" (involucro) non ricorsiva, MergeSort, che inizializza gli indici e chiama la funzione ricorsiva vera e propria, MergeSortR.

```
/*
 * 2.1.2: Struttura del Merge Sort
 */

// Funzione Wrapper
void MergeSort (Item A[], Item B[], int N) {
  int l=0, r=N-1;
  // Chiama la funzione ricorsiva 
  MergeSortR(A, B, l, r);
}

// Funzione Ricorsiva 
void MergeSortR (Item A[], Item B[], int l, int r){
  int q;
  
  // 2. Condizione di Terminazione
  if (l>=r)
    return;
    
  // 1. Divide (calcolo del centro) 
  q = (l+r)/2;
  
  // 2. Impera (chiamate ricorsive)
  MergeSortR(A, B, l, q);
  MergeSortR(A, B, q+1, r);
  
  // 3. Combina (fusione) 
  Merge(A, B, l, q, r);
}
```
La funzione Merge è il cuore dell'algoritmo.
    - Utilizza un vettore ausiliario B della stessa dimensione dell'originale.
    - Scorre i due sottovettori ordinati (A[l..q] e A[q+1..r]) usando due indici, i e j.
    - Confronta A[i] e A[j] e copia l'elemento più piccolo nel vettore ausiliario B (all'indice k)
    - Quando uno dei due sottovettori è esaurito, copia tutti gli elementi rimanenti dell'altro vettore in B
    - Infine, ricopia tutto il contenuto del vettore ausiliario B nel vettore originale A

### 2.1.3 Caratteristiche e Complessità del Merge Sort
- Non in loco (Not in-place): Richiede un vettore ausiliario B la cui dimensione dipende da N, quindi non ordina modificando solo il vettore originale.
- Stabile: Preserva l'ordine relativo degli elementi con chiavi uguali. Questo perché, in caso di parità, la funzione Merge sceglie l'elemento dal sottovettore sinistro (SX).

**Analisi di Complessità**
L'equazione alle ricorrenze per il Merge Sort è:
$T(n) = 2T(n/2) + \Theta(n)$ (per $n>1$) 
Risolvendo l'equazione (ad esempio per "srotolamento" o unfolding), si ottiene una complessità totale:
$T(n) = \Theta(n \log n)$ 
Questo perché l'albero della ricorsione ha $\log_2 n$ livelli e ad ogni livello, il lavoro totale (per la fusione) è $\Theta(n)$.

### 2.1.4 Quicksort (Hoare, 1961)
Anche il Quicksort è un algoritmo Divide et Impera.
1. Divide (Dividi): Il vettore A[l..r] viene partizionato in due sottovettori (SX e DX).
    - Si sceglie un elemento pivot $x$.
    - Il vettore viene riorganizzato in modo che tutti gli elementi in SX siano $\le x$ e tutti gli elementi in DX siano $\ge x$.
    - Il pivot $x$ si trova ora nella sua posizione finale corretta $q$.
    - Questa fase è eseguita dalla funzione partition, che ha costo $\Theta(n)$.
2. Impera (Risolvi): L'algoritmo viene richiamato ricorsivamente sui due sottovettori A[l..q-1] e A[q+1..r].
    - Condizione di Terminazione: La ricorsione si ferma quando un sottovettore ha 0 o 1 elemento ($l \ge r$).
3. Combina (Combina): Nessuna operazione. Il vettore è già ordinato al termine delle chiamate ricorsive.

A differenza del Merge Sort, la divisione qui non è necessariamente a metà.
```
/*
 * 2.1.4: Struttura del Quicksort
 */

// Funzione Wrapper 
void QuickSort (Item A[], int N) {
  int l=0, r=N-1;
  // Chiama la funzione ricorsiva 
  quicksortR(A, l, r);
}

// Funzione Ricorsiva 
void quicksortR (Item A[], int l, int r){
  int q;
  
  // 2. Condizione di Terminazione 
  if (l>=r)
    return;
    
  // 1. Divide (partizionamento) 
  q = partition(A, l, r);
  
  // 2. Impera (chiamate ricorsive) 
  quicksortR(A, l, q-1);
  quicksortR(A, q+1, r);
}
```
La funzione partition riorganizza il vettore.
- Si sceglie un pivot (es. l'ultimo elemento, $x = A[r]$)
- Si usano due indici: i che scandisce da sinistra verso destra e j da destra verso sinistra.
- Il ciclo while su i cerca un elemento $A[i]$ più grande del pivot.
- Il ciclo while su j cerca un elemento $A[j]$ più piccolo del pivot.
- Se i < j, gli elementi "fuori posto" $A[i]$ e $A[j]$ vengono scambiati (Swap)
- Quando gli indici si incrociano ($i \ge j$), si scambia $A[i]$ con il pivot $A[r]$ per posizionare il pivot al centro.
- La funzione ritorna la posizione finale del pivot $q=i$

### 2.1.5 Caratteristiche e Complessità del Quicksort
Anche il Quicksort è un algoritmo Divide et Impera.
- In loco (In-place): L'ordinamento avviene modificando direttamente il vettore originale, senza bisogno di un vettore ausiliario
- Non stabile: Durante il partizionamento, elementi con chiavi uguali possono essere scambiati e "scavalcarsi", perdendo il loro ordine relativo originale.

**Analisi di Complessità**
L'efficienza del Quicksort dipende crucialmente dal bilanciamento delle partizioni, che a sua volta dipende dalla scelta del pivot
- Caso Peggiore: Si verifica quando il pivot è sempre il minimo o il massimo (es. vettore già ordinato). La partizione è sbilanciata (un vettore da $n-1$ elementi e l'altro da 1).
    - Equazione: $T(n) = T(n-1) + \Theta(n)$
    - Complessità: $T(n) = \Theta(n^2)$ 
- Caso Migliore: Si verifica quando il pivot è la mediana, dividendo il vettore in due metà quasi uguali ($n/2$).
    - Equazione: $T(n) = 2T(n/2) + \Theta(n)$ 
    - Complessità: $T(n) = O(n \log n)$ 
- Caso Medio: Anche se il partizionamento è sbilanciato (es. $n/10$ e $9n/10$), la complessità media rimane $O(n \log n)$.
Per evitare il caso peggiore, si usano strategie di scelta del pivot, come scegliere un elemento casuale, l'elemento centrale, o la "mediana di tre".

### 2.1.6 Quadro Riassuntivo
La tabella seguente confronta le proprietà dei diversi algoritmi di ordinamento:


| ALGORITMO | IN LOCO | STABILE | COMPLESSITÀ (Caso Peggiore) |
|------------|-------------------------|---------------------|---------------------|
| Bubble Sort | Sì | Sì | O(n2) |
|Selection Sort|Sì|No|O(n2)|
|Insertion Sort|Sì|Sì|O(n2)|
|Shell Sort|Sì|No|(dipende)|
|Merge Sort|No|Sì|O(nlogn)|
|Quick Sort|Sì|No|O(n2)|
|Counting Sort|No|Sì|O(n)|
|Radix Sort|No|Sì|O(n)|


*(Nota: la complessità del Quick Sort è $O(n \log n)$ nel caso medio e migliore)*