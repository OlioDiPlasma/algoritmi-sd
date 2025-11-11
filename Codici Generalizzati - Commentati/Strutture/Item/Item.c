#include "Item.h"

/* --- QUASI ADT --- L'utente (es. main.c) conosce la definizione di Item (ad esempio, 
sa che è un int o una struct). */ 

/* TIPO 1 */ - item è int
// Ritorna la chiave dell'Item. In questo caso, l'Item E' la chiave.
Key KEYget(Item val) {
    return val;
}
// Confronta due chiavi. Ritorna <0 se k1<k2, 0 se k1==k2, >0 se k1>k2.
int KEYcompare (Key k1, Key k2) {
    return (k1-k2);
}
// Legge un Item (un intero) da tastiera
Item ITEMscan() {
    Item val;
    scanf("%d", &val);
    return val;
}
// Stampa un Item (un intero)
void ITEMshow(Item val) {
    printf("%d", val);
}
// Ritorna vero (1) se val1 < val2
int ITEMless(Item val1, Item val2) {
    return (KEYget(val1) < KEYget(val2));
}
// Ritorna vero (1) se val1 > val2
int ITEMgreater(Item val1, Item val2) {
    return (KEYget(val1) > KEYget(val2));
}

/* TIPO 2 */ - item è char *
// Buffer di appoggio (statico, globale a questo file)
static char buf[MAXC];

// L'Item (stringa) E' la chiave
Key KEYget(Item val) {
    return val;
}
// Confronta due chiavi (stringhe)
int KEYcompare (Key k1, Key k2) {
    return (strcmp(k1, k2));
}
// Legge una stringa da tastiera
Item ITEMscan() {
    scanf("%s", buf); // Legge nel buffer temporaneo
    // Duplica la stringa (alloca nuova memoria) e ritorna il puntatore
    return strdup(buf);
}
// Stampa l'Item (stringa)
void ITEMshow(Item val) {
    printf("%s", val);
}
// Confronta (less) due stringhe
int ITEMless(Item val1, Item val2) {
    return (strcmp(KEYget(val1), KEYget(val2)) < 0);
}
// Confronta (greater) due stringhe
int ITEMgreater(Item val1, Item val2) {
    return (strcmp(KEYget(val1), KEYget(val2)) > 0);
}

/* TIPO 3 */ - stringa statica
// Ritorna la chiave (il campo 'name') dell'Item
// NOTA: Prende un PUNTATORE all'Item per efficienza
Key KEYget(Item *pval) {
    return (pval->name);
}
// Confronta due chiavi (stringhe)
int KEYcompare (Key k1, Key k2) {
    return (strcmp(k1,k2));
}
// Legge i campi della struct da tastiera
Item ITEMscan() {
    Item val; // Alloca la struct (sullo stack)
    // Legge direttamente dentro i campi della struct
    scanf("%s %d", val.name, &(val.num));
    return val; // Ritorna la struct (copiata)
}
// Stampa i campi della struct
void ITEMshow(Item val) {
    printf("%s %d", val.name, val.num);
}
// Confronta (less) due Item (basandosi sulla chiave 'name')
int ITEMless(Item val1, Item val2) {
    // Passa l'INDIRIZZO (&) a KEYget, come richiesto
    return (strcmp(KEYget(&val1), KEYget(&val2)) < 0);
}
// Confronta (greater) due Item
int ITEMgreater(Item val1, Item val2) {
    return (strcmp(KEYget(&val1), KEYget(&val2)) > 0);
}

/* TIPO 4 */ - stringa dinamica
// Buffer di appoggio (statico, globale a questo file)
static char buf[MAXC];

// Ritorna la chiave (il puntatore 'name')
Key KEYget(Item *pval) {
    return (pval->name);
}
// Confronta due chiavi (stringhe)
int KEYcompare (Key k1, Key k2) {
    return (strcmp(k1, k2));
}
// Legge i campi della struct
Item ITEMscan() {
    Item val; // Alloca la struct (sullo stack)
    // Legge la stringa nel buffer e il numero nel campo 'num'
    scanf("%s %d", buf, &(val.num));
    // Alloca memoria per la stringa e assegna il puntatore a 'name'
    val.name = strdup(buf);
    return val; // Ritorna la struct
}
// Stampa i campi della struct
void ITEMshow(Item val) {
    printf("%s %d", val.name, val.num);
}
// Confronta (less) due Item
int ITEMless(Item val1, Item val2) {
    return (strcmp(KEYget(&val1), KEYget(&val2)) < 0);
}
// Confronta (greater) due Item
int ITEMgreater(Item val1, Item val2) {
    return (strcmp(KEYget(&val1), KEYget(&val2)) > 0);
}

/* --- ADT I° CLASSE ---
L'utente include Item.h che definisce typedef struct 
item *Item;. L'utente NON CONOSCE i campi name e num.*/

/* TIPO 3 */ - Stringa statica
// Definizione della struttura *nascosta*
struct item {
    char name[MAXC]; // Array statico
    int num;
};

// Alloca un nuovo Item (un puntatore a struct item)
Item ITEMnew(void) {
    // Alloca memoria sull'heap per la struct
    Item val = (Item) malloc(sizeof(struct item));
    
    // Se malloc fallisce, ritorna un Item "vuoto"
    if (val == NULL)
        return ITEMsetvoid(); // ITEMsetvoid non mostrata
        
    // Inizializza i campi
    val->name[0] = '\0'; // Stringa vuota
    val->num = 0;

    return val; // Ritorna il PUNTATORE
}

// Libera la memoria allocata per l'Item
void ITEMfree(Item val) {
    free(val); // Semplice free (la stringa è dentro la struct)
}

// Alloca e legge un nuovo Item da tastiera
Item ITEMscan() {
    Item val = ITEMnew(); // Alloca un nuovo Item vuoto
    
    // Se l'allocazione ha funzionato...
    if (val != NULL) {
        // ...leggi i dati direttamente nei campi
        scanf("%s %d", val->name, &val->num);
    }
    
    return val; // Ritorna il puntatore all'Item riempito
}

/* TIPO 4 */ - Stringa dinamica
// Buffer di appoggio (statico, globale a questo file)
static char buf[MAXC];

// Definizione della struttura *nascosta*
struct item {
    char *name; // Puntatore a stringa (dinamico)
    int num;
};

// Alloca un nuovo Item (puntatore a struct item)
Item ITEMnew(void) {
    Item val = (Item) malloc(sizeof(struct item));
    
    if (val == NULL)
        return ITEMsetvoid();
        
    // Inizializza i campi (il puntatore a 'name' è nullo)
    val->name = NULL;
    val->num = 0;
    
    return val; // Ritorna il puntatore
}

// Libera la memoria allocata per l'Item (doppia free)
void ITEMfree(Item val) {
    // 1. Controlla se il puntatore 'name' è stato allocato
    if (val->name != NULL) 
        free(val->name); // 2. Libera la stringa 'name'
    free(val); // 3. Libera la struct 'item'
}

// Alloca e legge un nuovo Item da tastiera
Item ITEMscan() {
    Item val = ITEMnew(); // Alloca un nuovo Item vuoto

    if (val != NULL) {
        // Legge nel buffer temporaneo e nel campo 'num'
        scanf("%s %d", buf, &val->num);
        // Alloca e copia la stringa dal buffer a 'val->name'
        val->name = strdup(buf);
    }
    
    return val; // Ritorna il puntatore
}

// Versione alternativa di ITEMscan (probabilmente un errore nel file)
// Questa versione NON alloca un nuovo Item, ma riempie
// uno già esistente passato come argomento.
void ITEMscan(Item val) {
    scanf("%s %d", buf, &val->num);
    val->name = strdup(buf); // Sovrascrive il vecchio 'name' (rischio memory leak)
}


/* FUNZIONI COMUNI */
// Ritorna la chiave (il puntatore 'name')
// 'val' è il puntatore (Item)
Key KEYget(Item val) {
    return (val->name);
}
// Confronta due chiavi (stringhe)
int KEYcompare (Key k1, Key k2) {
    return (strcmp(k1, k2));
}
// Stampa i campi dell'Item (puntato da 'val')
void ITEMshow(Item val) {
    printf("%s %d", val->name, val->num);
}
// Confronta (less) due Item (puntati)
int ITEMless(Item val1, Item val2) {
    // Chiama KEYget (che ritorna i puntatori 'name') e li confronta
    return (strcmp(KEYget(val1), KEYget(val2)) < 0);
}
// Confronta (greater) due Item (puntati)
int ITEMgreater(Item val1, Item val2) {
    return (strcmp(KEYget(val1), KEYget(val2)) > 0);
}