// Include l'header file che definisce il TIPO 'Item'
// (probabilmente una struct: typedef struct { int low; int high; } Item;)
#include "Item.h"

// Funzione per LEGGERE un Item (intervallo) da tastiera
Item ITEMscan() {
    Item val; // Dichiara una variabile 'Item' (struct)
    
    // Chiede all'utente l'estremo inferiore
    printf("low = "); 
    // Legge l'intero e lo salva nel campo 'low' della struct
    scanf("%d", &val.low);
    
    // Chiede all'utente l'estremo superiore
    printf("high = "); 
    // Legge l'intero e lo salva nel campo 'high' della struct
    scanf("%d", &val.high);
    
    // Ritorna l'intera struct 'val'
    return val;
}

// Funzione per STAMPARE un Item (intervallo)
void ITEMstore(Item val) {
    // Stampa l'intervallo nel formato standard [low, high]
    printf("[%d, %d] ", val.low, val.high);
}

// Funzione per creare un Item "nullo" o "vuoto"
// Serve per la sentinella (nodo 'z') e per le ricerche fallite
Item ITEMsetVoid() {
    // Definiamo un intervallo "vuoto" come {-1, -1}
    // (presuppone che [-1, -1] non sia un intervallo valido nei dati)
    Item val = {-1, -1};
    return val;
}

// Funzione per CONTROLLARE se un Item è "vuoto"
int ITEMcheckVoid(Item val) {
    // Controlla se entrambi i campi corrispondono al nostro Item "vuoto"
    if ((val.low == -1) && (val.high == -1))
        return 1; // 1 = vero (è vuoto)
    return 0; // 0 = falso (non è vuoto)
}

// Funzione 'ITEMoverlap' (Sovrapposizione) - IL CUORE DELL'IBST
// Controlla se due intervalli val1 e val2 si sovrappongono
int ITEMoverlap(Item val1, Item val2) {
    // La logica di overlap è:
    // L'intervallo 1 [A, B] e l'intervallo 2 [C, D] si sovrappongono se
    // A <= D e C <= B.
    if ((val1.low <= val2.high) && (val2.low <= val1.high))
        return 1; // 1 = vero (c'è overlap)
    return 0; // 0 = falso (non c'è overlap)
}

// Funzione 'ITEMeq' (Uguaglianza)
// Controlla se due intervalli sono identici
int ITEMeq(Item val1, Item val2) {
    // Devono essere uguali sia 'low' che 'high'
    if ((val1.low == val2.low) && (val1.high == val2.high))
        return 1; // 1 = vero (sono uguali)
    return 0; // 0 = falso (sono diversi)
}

// Funzione 'ITEMlt' (Minore Di - Less Than)
// Confronta due intervalli per l'ordinamento nel BST
int ITEMlt(Item val1, Item val2) {
    // L'ordinamento nell'IBST si basa SOLO sull'estremo INFERIORE (low)
    if ((val1.low < val2.low))
        return 1; // 1 = vero (val1 viene prima di val2)
    return 0; // 0 = falso
}

// Funzione 'ITEMlt_int' (Minore Di - Confronto con Intero)
// Confronta l'estremo 'low' di un intervallo con un intero 'val2'
int ITEMlt_int(Item val1, int val2) {
    // Questa funzione serve per la RICERCA (searchR)
    // per controllare se val1.low < h->l->max
    if ((val1.low < val2))
        return 1; // 1 = vero
    return 0; // 0 = falso
}

// Funzione 'ITEMhigh' (Getter)
// Ritorna l'estremo SUPERIORE (high) di un intervallo
int ITEMhigh(Item val) { 
    return val.high; 
}

// Funzione 'ITEMlow' (Getter)
// Ritorna l'estremo INFERIORE (low) di un intervallo
int ITEMlow(Item val) { 
    return val.low; 
}