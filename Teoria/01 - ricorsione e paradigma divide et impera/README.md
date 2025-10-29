

# 01 — Ricorsione e paradigma divide et impera

## 1.1 Parte Teorica
### 1.1.1 Ricorsione
Una funzione è definita ricorsiva quando, all'interno della sua definizione, chiama sé stessa (ricorsione diretta) o chiama un'altra funzione che, direttamente o indirettamente, finisce per richiamare la funzione originale (ricorsione indiretta). 
Matematicamente, la soluzione $S$ a un problema su dati $D$ è espressa come una funzione $f$ della soluzione dello stesso problema applicato a dati $D'$ più semplici: $S(D) = f(S(D'))$.

Un esempio di funzione che chiama se stessa:
```
unsigned long fact (int n) {
  if ((n == 0) || (n==1))
    return 1;
  return n*fact(n-1); 
}
``` 

### 1.1.2 La condizione di Terminazione
Ogni algoritmo ricorsivo deve terminare. Per garantire ciò, è indispensabile definire una condizione di terminazione (o caso base).
 Si tratta di un caso del problema sufficientemente "banale" (es. operare su un insieme con un solo elemento) che può essere risolto direttamente, senza ulteriori chiamate ricorsive, interrompendo così la catena di chiamate

Come visto dall'esempio precedente 
```
  if ((n == 0) || (n==1))
    return 1;
``` 

### 1.1.3 Il Paradigma "Divide et Impera" (Divide and Conquer)
La ricorsione è alla base del paradigma di problem-solving Divide et Impera , che scompone la soluzione di un problema in tre fasi:
1. Divide (Dividi): Il problema principale, di dimensione $n$, viene suddiviso in $a$ sottoproblemi indipendenti, della stessa natura ma di dimensione $n'$ inferiore ($n' < n$) .
2. Impera (Risolvi): I sottoproblemi vengono risolti. Questo avviene tipicamente tramite chiamate ricorsive. Se un sottoproblema è abbastanza piccolo da corrispondere al caso base, viene risolto direttamente (condizione di terminazione).
3. Combina (Combina): Le soluzioni parziali ottenute dai sottoproblemi vengono ricombinate per costruire la soluzione del problema originario.

```
/*
 * 1.1.3: Esempio di Divide et Impera (Massimo in un Vettore)
 */
int trovaMax(int v[], int l, int r) {
    // CASO BASE (necessario per l'Impera)
    if (l == r) {
        return v[l];
    }

    // 1. DIVIDE: Trova il punto medio
    int m = (l + r) / 2;

    // 2. IMPERA: Risolvi ricorsivamente i due sottoproblemi
    int max_sx = trovaMax(v, l, m);
    int max_dx = trovaMax(v, m + 1, r);

    // 3. COMBINA: Metti insieme i risultati
    if (max_sx > max_dx) {
        return max_sx;
    } else {
        return max_dx;
    }
}
``` 

### 1.1.4 Classificazione degli Algoritmi Ricorsivi
Gli algoritmi ricorsivi possono essere classificati in base a come riducono la dimensione del problema:
- Decrease and Conquer: Ad ogni passo la dimensione del problema è ridotta di un valore costante (es. $n-1$, $n-k$). Ne sono esempi il calcolo del Fattoriale (un sottoproblema $a=1$, riduzione $k=1$) e i numeri di Fibonacci (due sottoproblemi $a=2$, riduzioni $k=1$ e $k=2$).
- Divide and Conquer: Ad ogni passo la dimensione del problema è ridotta di un fattore costante (es. $n/2$, $n/b$). Ne sono esempi la Ricerca Binaria (un sottoproblema $a=1$, fattore $b=2$) e l'algoritmo di Karatsuba per il prodotto di interi ($a=3$, $b=2$).

``` 
/*
 * 1.1.4: Esempio di "Decrease and Conquer" (Fattoriale)
 * La dimensione del problema passa da 'n' a 'n-1' (riduzione costante).
 */
unsigned long fattoriale(int n) {
    if (n <= 1)
        return 1;
    // La chiamata ricorsiva è su (n-1)
    return n * fattoriale(n - 1);
}

/*
 * 1.1.4: Esempio di "Divide and Conquer" (Ricerca Binaria)
 * La dimensione del problema passa da 'n' a 'n/2' (riduzione di un fattore).
 */
int ricercaBinaria(int v[], int l, int r, int chiave) {
    if (l > r)
        return -1; // Non trovato (caso base)

    int m = (l + r) / 2;

    if (v[m] == chiave)
        return m; // Trovato (caso base)

    // La chiamata ricorsiva è su circa (n/2) elementi
    if (chiave < v[m])
        return ricercaBinaria(v, l, m - 1, chiave);
    else
        return ricercaBinaria(v, m + 1, r, chiave);
}
``` 


### 1.1.5 Analisi di Complessità e Equazioni alle Ricorrenze
Per analizzare la complessità $T(n)$ di un algoritmo ricorsivo, si utilizzano le Equazioni alle Ricorrenze. Queste equazioni esprimono il tempo di esecuzione totale $T(n)$ in funzione del tempo speso nelle tre fasi:
- $D(n)$: Costo della fase di Divide
- $C(n)$: Costo della fase di Combina
- Costo della fase Impera:
    - Per Divide and Conquer: $a \cdot T(n/b)$, dove $a$ è il numero di sottoproblemi e $n/b$ la loro dimensione. L'equazione completa è: $T(n) = D(n) + aT(n/b) + C(n)$.
- Per Decrease and Conquer: $\sum T(n-k_i)$, dove $k_i$ sono le riduzioni. L'equazione completa è: $T(n) = D(n) + \sum T(n-k_i) + C(n)$.

### 1.1.6 Complessità Comuni
Risolvendo le equazioni alle ricorrenze (spesso per "srotolamento" o unfolding ), si ottengono complessità note:
- $O(\log n)$: Ricerca Binaria ($T(n) = T(n/2) + 1$)
- $O(n)$: Fattoriale ($T(n) = T(n-1) + 1$) o Massimo di un Vettore ($T(n) = 2T(n/2) + 1$).
- $O(n^2)$: Prodotto standard di interi ($T(n) = 4T(n/2) + n$).
- $O(n^{\log_2 3})$: Prodotto di interi con Karatsuba ($T(n) = 3T(n/2) + n$).
- $O(2^n)$: Numeri di Fibonacci ($T(n) = T(n-1) + T(n-2) + 1$) o Torri di Hanoi ($T(n) = 2T(n-1) + 1$).

### 1.1.7 Meccanismo Computazionale: lo Stack
Le chiamate a funzione ricorsive sono gestite dal sistema utilizzando una struttura dati fondamentale: lo stack (o pila). Lo stack opera con una strategia LIFO (Last In First Out).
- Ogni volta che una funzione viene chiamata (ricorsivamente o meno), un nuovo stack frame (o record di attivazione) viene creato e messo in cima (operazione push) allo stack.
- Questo frame contiene i parametri della funzione, le sue variabili locali e l'indirizzo a cui tornare al termine dell'esecuzione.
- Quando la funzione termina, il suo frame viene rimosso dalla cima (operazione pop) e il controllo torna all'istruzione successiva del chiamante.
- Se la ricorsione è troppo profonda e riempie tutto lo spazio di memoria allocato per lo stack, si verifica un errore di stack overflow.

```
/*
 * 1.1.7: Esempio di utilizzo dello Stack (Fattoriale non-tail)
 *
 * Quando chiami fattoriale(3):
 * 1. PUSH: fattoriale(3) entra, deve calcolare 3 * fattoriale(2).
 * Mette in pausa e salva '3' nello stack.
 * 2. PUSH: fattoriale(2) entra, deve calcolare 2 * fattoriale(1).
 * Mette in pausa e salva '2' nello stack.
 * 3. PUSH: fattoriale(1) entra, caso base, ritorna 1.
 * 4. POP:  fattoriale(2) riprende, prende '2' dallo stack,
 * calcola 2 * 1 = 2, ritorna 2.
 * 5. POP:  fattoriale(3) riprende, prende '3' dallo stack,
 * calcola 3 * 2 = 6, ritorna 6.
 */
unsigned long fattoriale_usa_stack(int n) {
    if (n <= 1)
        return 1;
        
    // L'operazione "n * ..." resta in attesa sullo stack
    // finché "fattoriale(n-1)" non ha finito.
    return n * fattoriale_usa_stack(n - 1);
}
```


### 1.1.8 Ricorsione in Coda (Tail Recursion)
Un aspetto cruciale per l'efficienza è la tail recursion.
- Non Tail-Recursive: Una funzione in cui la chiamata ricorsiva non è l'ultima operazione da eseguire. Ad esempio, return n * fact(n-1);. La moltiplicazione per n viene eseguita dopo il ritorno della chiamata fact(n-1). I calcoli avvengono "in risalita" e richiedono che tutti gli stack frame siano mantenuti in memoria.
- Tail-Recursive: Una funzione in cui la chiamata ricorsiva è l'ultima operazione in assoluto (eccezion fatta per return). Tutti i calcoli necessari (es. n*f) vengono eseguiti prima della chiamata successiva e passati come parametri. I calcoli avvengono "in discesa".
- Ottimizzazione: Le funzioni tail-recursive possono essere ottimizzate: lo stack frame della funzione chiamata può rimpiazzare quello del chiamante, invece di aggiungerne uno nuovo. Questo riduce l'occupazione di memoria da $O(n)$ (proporzionale alla profondità della ricorsione) a $O(1)$ (costante), eliminando il rischio di stack overflow. (Nota: molti compilatori C non implementano questa ottimizzazione ).

```
/*
 * 1.1.8: Esempio NON Tail-Recursive
 * L'ultima operazione è la MOLTIPLICAZIONE ('*').
 * I calcoli avvengono "in risalita".
 */
unsigned long fattoriale_non_tail(int n) {
    if (n <= 1)
        return 1;
    return n * fattoriale_non_tail(n - 1);
}


/*
 * 1.1.8: Esempio TAIL-Recursive
 * Usa una funzione "helper" con un accumulatore 'acc'.
 */
unsigned long fattoriale_tail_helper(int n, unsigned long acc) {
    if (n <= 1)
        return acc; // Caso base: il risultato è nell'accumulatore
    
    // L'ultima operazione è la CHIAMATA RICORSIVA stessa.
    // Il calcolo (n * acc) avviene PRIMA della chiamata.
    return fattoriale_tail_helper(n - 1, n * acc);
}

// Funzione "wrapper" (involucro) per l'utente
unsigned long fattoriale_tail(int n) {
    // La prima chiamata inizializza l'accumulatore a 1
    return fattoriale_tail_helper(n, 1);
}
```

### 1.1.9 Dualità Ricorsione - Iterazione
Esiste una dualità tra ricorsione e iterazione: qualsiasi algoritmo ricorsivo può essere trasformato in uno iterativo (basato su cicli)
- Le funzioni tail-recursive sono trasformabili direttamente in un ciclo iterativo (es. while) senza bisogno di uno stack.
- Le funzioni non tail-recursive possono essere trasformate in iterative solo utilizzando uno stack gestito esplicitamente dall'utente (per simulare lo stack di sistema).
- In generale, le soluzioni iterative "native" (pensate fin dall'inizio come iterative) sono considerate più efficienti di quelle ricorsive.

```
/*
 * 1.1.9: Esempio Ricorsivo (Fattoriale)
 */
unsigned long fattoriale_ric(int n) {
    if (n <= 1)
        return 1;
    return n * fattoriale_ric(n - 1);
}

/*
 * 1.1.9: Stessa funzione in forma Iterativa (con un ciclo 'for')
 * Questa versione "nativa" è O(n) in tempo e O(1) in spazio (stack).
 */
unsigned long fattoriale_iterativo(int n) {
    unsigned long risultato = 1;
    
    // Il ciclo 'for' sostituisce la catena di chiamate ricorsive
    for (int i = 2; i <= n; i++) {
        risultato = risultato * i;
    }
    
    return risultato;
}
```

