#include "List.h"

// =============================
// Gestione delle chiavi e degli Item
// =============================

// Restituisce la chiave associata all'Item (in questo caso l'Item coincide con la chiave)
Key KEYget(Item d) {
    return d;
}

// Verifica se due chiavi sono uguali
int KEYeq(Key k1, Key k2) {
    return k1 == k2;
}

// Ritorna vero (1) se k1 < k2
int KEYless(Key k1, Key k2) {
    return k1 < k2;
}

// Ritorna vero (1) se k1 > k2
int KEYgreater(Key k1, Key k2) {
    return k1 > k2;
}

// Mostra a schermo il valore di un Item
void ITEMdisplay(Item d) {
    printf("VAL = %d\n", d);
}

// Restituisce un Item "vuoto" (usato come valore nullo)
Item ITEMsetvoid() {
    return 0;
}

// =============================
// Funzioni base per i nodi
// =============================

// Crea un nuovo nodo con valore val e puntatore next
link newNode(Item val, link next) {  link NEWnode(Item val, link next);
    link x = malloc(sizeof *x);   // alloca memoria per un nodo
    if (x == NULL)                // controlla se malloc è fallita
        return NULL;
    x->val = val;                 // imposta il valore
    x->next = next;               // collega al nodo successivo
    return x;
}

// =============================
// Inserimento in testa
// =============================

// Inserisce un nodo all'inizio e restituisce la nuova testa
link listInsHead(link h, Item val) { 
    h = newNode(val, h);          // nuovo nodo che punta al vecchio head
    return h;                     // restituisce la nuova testa
}

// Inserisce un nodo in testa usando un puntatore alla testa
void listInsHeadP(link *hp, Item val) { 
    *hp = newNode(val, *hp);      // modifica direttamente la testa passata per riferimento
}

// Inserisce un nodo in testa alla lista
void LISTinshead(LIST *l, Item val) {
    link new = NEWnode(val, *l);  // crea nuovo nodo puntando alla vecchia testa
    if (new == NULL) return;       // controlla malloc
    *l = new;                      // aggiorna la testa
}

// =============================
// Inserimento in coda
// =============================

// Inserisce un nodo in coda e restituisce la testa (versione classica)
link listInsTail(link h, Item val) {
    link x;

    if (h == NULL)                // se la lista è vuota
        return newNode(val, NULL); // il nuovo nodo diventa la testa

    // scorri fino all'ultimo nodo
    for (x = h; x->next != NULL; x = x->next);
    x->next = newNode(val, NULL); // collega il nuovo nodo in fondo
    return h;                     // restituisce la testa originale
}

// Inserisce in coda usando puntatore alla testa (versione con *hp)
void listInsTailP(link *hp, Item val) {
    link x = *hp;

    if (x == NULL)                // lista vuota
        *hp = newNode(val, NULL);
    else {
        for (; x->next != NULL; x = x->next);  // trova ultimo nodo
        x->next = newNode(val, NULL);          // collega in fondo
    }
}

// Variante elegante con doppio puntatore (più efficiente)
void listInsTailP(link *hp, Item val) {
    link *xp = hp;

    // scorre fino a che *xp non punta all'ultimo elemento
    while (*xp != NULL)
        xp = &((*xp)->next);

    // collega direttamente un nuovo nodo dove serve
    *xp = newNode(val, NULL);
}

// Inserimento efficiente in coda con puntatore anche alla "tail"
void listInsTFast(link *hp, link *tp, Item val) {
    if (*hp == NULL) {
        // lista vuota: testa e coda coincidono
        *hp = *tp = newNode(val, NULL);
    } else {
        // collega nuovo nodo in coda e aggiorna il tail
        (*tp)->next = newNode(val, NULL);
        *tp = (*tp)->next;
    }
}

// =============================
// Inserimento ordinato
// =============================

// Inserisce un elemento mantenendo la lista ordinata
link SortListIns(link h, Item val) { link LISTsortins(link h, ITEM item); 
    link x, p;
    Key k = KEYget(val);

    // caso: lista vuota o elemento da inserire prima della testa
    if (h == NULL || KEYgreater(KEYget(h->val), k))
        return newNode(val, h);

    // scorri fino a trovare la posizione corretta
    for (x = h->next, p = h; x != NULL && KEYgreater(k, KEYget(x->val)); p = x, x = x->next);
    p->next = newNode(val, x);

    return h;
}

// =============================
// Visite (scorrimenti)
// =============================

// Visita ricorsiva: stampa i nodi in ordine normale
void listTravR(link h) {
    if (h == NULL)
        return;
    ITEMdisplay(h->val);
    listTravR(h->next);
}

// Visita ricorsiva inversa: stampa dal fondo verso la testa
void listRevTravR(link h) {
    if (h == NULL)
        return;
    listRevTravR(h->next);
    ITEMdisplay(h->val);
}

// =============================
// Ricerca
// =============================

// Cerca un Item con chiave k nella lista (non ordinata)
Item listSearch(link h, Key k) {
    link x;
    for (x = h; x != NULL; x = x->next)
        if (KEYeq(KEYget(x->val), k))
            return x->val;
    return ITEMsetvoid(); // non trovato
}

// Cerca in una lista ordinata (usa il fatto che è ordinata)
Item SortListSearch(link h, Key k) { ITEM LISTsortsearch(link h, Key k);
    link x;
    for (x = h; x != NULL && KEYgeq(k, KEYget(x->val)); x = x->next)
        if (KEYeq(KEYget(x->val), k))
            return x->val;
    return ITEMsetvoid();
}

// =============================
// Cancellazione
// =============================

// Cancella il primo nodo (testa) e restituisce la nuova testa
link listDelHead(link h) {
    link t = h;

    if (h == NULL)
        return NULL;
    h = h->next;
    free(t);                      // libera il nodo eliminato
    return h;
}

// Cancella il nodo con chiave k (versione iterativa)
link listDelKey(link h, Key k) {
    link x, p;

    if (h == NULL)
        return NULL;

    for (x = h, p = NULL; x != NULL; p = x, x = x->next) {
        if (KEYeq(KEYget(x->val), k)) {
            if (x == h)
                h = x->next;      // elimina testa
            else
                p->next = x->next;// collega il precedente al successivo
            free(x);
            break;
        }
    }

    return h;
}

// Cancellazione ricorsiva per chiave
link listDelKeyR(link x, Key k) {
    link t;

    if (x == NULL)
        return NULL;
    if (KEYeq(KEYget(x->val), k)) {
        t = x->next;
        free(x);
        return t;                 // ritorna la nuova testa
    }
    x->next = listDelKeyR(x->next, k);
    return x;
}

// Cancellazione in lista ordinata
link SortListDel(link h, Key k) { link LISTsortdel(link h, Key k);
    link x, p;

    if (h == NULL) 
        return NULL;

    for (x = h, p = NULL; x != NULL && KEYgeq(k, KEYget(x->val)); p = x, x = x->next) {
        if (KEYeq(KEYget(x->val), k)) {
            if (x == h) 
                h = x->next;
            else
                p->next = x->next;
            free(x);
            break;
        }
    }

    return h;
}

// =============================
// Estrazione (rimuove e restituisce un valore)
// =============================

// Estrae (rimuove) la testa della lista e restituisce il valore
Item listExtrHeadP(link *hp) {
    link t = *hp;
    Item tmp;

    if (t == NULL)
        return ITEMsetvoid();
    tmp = t->val;
    *hp = t->next;
    free(t);
    return tmp;
}

// Estrae un nodo con chiave k (usa puntatore al puntatore)
Item listExtrKeyP(link *hp, Key k) {
    link *xp, t;
    Item i = ITEMsetvoid();

    // xp scorre la lista come puntatore a puntatore
    for (xp = hp; (*xp) != NULL; xp = &((*xp)->next)) {
        if (KEYeq(KEYget((*xp)->val), k)) {
            t = *xp;
            *xp = (*xp)->next;    // scollega il nodo
            i = t->val;           // salva il valore
            free(t);
            break;
        }
    }
    return i;
}

// =============================
// Inversione della lista
// =============================

// Versione con estrazione e reinserimento (meno efficiente)
link listReverseF(link x) {
    link y = x, r = NULL;
    Item tmp;
    
    while (y != NULL) {
        tmp = listExtrHeadP(&y);  // estrae testa
        r = listInsHead(r, tmp);  // la reinserisce in testa alla nuova lista
    }
    return r;
}

// Versione efficiente: inverte i collegamenti dei nodi
link listReverseF(link x) {
    link t, y = x, r = NULL;

    while (y != NULL) {
        t = y->next;  // salva il successivo
        y->next = r;  // inverte il collegamento
        r = y;        // sposta il puntatore della lista invertita
        y = t;        // passa al prossimo nodo
    }

    return r;
}

// =============================
// Ordinamento della lista
// =============================

// Crea una nuova lista ordinata estraendo e reinserendo in ordine
link listSortF(link h) {
    link y = h, r = NULL;
    Item tmp;

    while (y != NULL) {
        tmp = listExtrHeadP(&y);  // estrae elemento dalla lista originale
        r = SortListIns(r, tmp);  // inserisce in ordine nella nuova lista
    }

    return r;
}

// =============================
// Stampa e Free
// =============================

// Stampa tutti i valori della lista a partire dalla testa
void LISTshow(link h) {
    for (link x = h; x != NULL; x = x->next)  // scorre tutti i nodi
        ITEMdisplay(x->val);                  // stampa il valore del nodo corrente
}

// Libera tutta la memoria occupata dai nodi della lista
void LISTfree(link h) {
    link t;
    while (h != NULL) {       // finché ci sono nodi
        t = h;                // salva nodo corrente
        h = h->next;          // passa al nodo successivo
        free(t);              // libera la memoria del nodo corrente
    }
}