
**Basic** (Ricerca base)

```
node
Key k;
Item val;
link left, right;
```

**Extended** (Selezioni - bilanciamento)
In pratica è un estensione del primo con operazioni più complesse

```
node
Key k;
Item val;
link left, right;
int N; // numero di nodi nel sottoalbero
```

**IBST** (Ricerca di intervalli sovrapposti)
Anche se visto separatamente è comunque un implementazione non un ADT

```
typedef struct {
    float lo, hi;   // estremi dell'intervallo
} Interval;

struct node {
    Interval val;
    link left, right;
    float max; // massimo valore di hi nel sottoalbero
};
```

Al suo interno abbiamo anche item questo perchè l'Item è specifico suo dato da un valore low e high