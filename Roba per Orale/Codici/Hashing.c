#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 11  // Dimensione Tabella (numero primo per ridurre collisioni)

// ============================================================
// PARTE 1: SEPARATE CHAINING (Liste di collisione)
// ============================================================

struct Node {
    int key;
    int value; // Payload
    struct Node* next;
};

// La tabella è un array di puntatori a Node
struct Node* chainTable[M];

void initChain() {
    for (int i = 0; i < M; i++) chainTable[i] = NULL;
}

int hash(int key) {
    return key % M; // Funzione hash semplice
}

// INSERT (Chaining): Inserimento in testa alla lista (O(1))
void chainInsert(int key, int value) {
    int idx = hash(key);
    
    // Creazione nodo
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->key = key;
    newNode->value = value;
    
    // Inserimento in testa (gestisce collisioni automaticamente)
    newNode->next = chainTable[idx];
    chainTable[idx] = newNode;
    
    printf("[Chain] Inserito (%d, %d) all'indice %d\n", key, value, idx);
}

// SEARCH (Chaining): Scansione lista
int chainSearch(int key) {
    int idx = hash(key);
    struct Node* curr = chainTable[idx];
    
    while (curr != NULL) {
        if (curr->key == key) return curr->value; // Trovato
        curr = curr->next;
    }
    return -1; // Non trovato
}

// DELETE (Chaining): Rimozione classica da lista
void chainDelete(int key) {
    int idx = hash(key);
    struct Node* curr = chainTable[idx];
    struct Node* prev = NULL;
    
    while (curr != NULL) {
        if (curr->key == key) {
            if (prev == NULL) {
                // Rimozione in testa
                chainTable[idx] = curr->next;
            } else {
                // Rimozione interna
                prev->next = curr->next;
            }
            free(curr);
            printf("[Chain] Cancellato chiave %d\n", key);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("[Chain] Chiave %d non trovata per cancellazione.\n", key);
}

// ============================================================
// PARTE 2: OPEN ADDRESSING (Linear Probing)
// ============================================================

// Stati possibili di una cella
enum State { EMPTY, FULL, DELETED };

struct Element {
    int key;
    int value;
    enum State state; // Fondamentale per la Lazy Deletion
};

struct Element probeTable[M];

void initProbe() {
    for (int i = 0; i < M; i++) probeTable[i].state = EMPTY;
}

// INSERT (Linear Probing)
void probeInsert(int key, int value) {
    int idx = hash(key);
    int startIdx = idx; // Per evitare loop infinito se tabella piena
    
    while (probeTable[idx].state == FULL) {
        // Se la chiave esiste già, aggiorno il valore (opzionale)
        if (probeTable[idx].key == key) {
            probeTable[idx].value = value;
            return;
        }
        
        // Linear Probing: passo alla prossima cella
        idx = (idx + 1) % M;
        
        // Se siamo tornati all'inizio, la tabella è piena
        if (idx == startIdx) {
            printf("[Probe] ERRORE: Tabella piena (Overflow)!\n");
            return;
        }
    }
    
    // Trovata cella EMPTY o DELETED -> Inserisco qui
    // Nota: Possiamo sovrascrivere DELETED
    probeTable[idx].key = key;
    probeTable[idx].value = value;
    probeTable[idx].state = FULL;
    printf("[Probe] Inserito (%d, %d) all'indice %d (Hash originale: %d)\n", 
           key, value, idx, hash(key));
}

// SEARCH (Linear Probing)
int probeSearch(int key) {
    int idx = hash(key);
    int startIdx = idx;
    
    // Continuo a cercare finché non trovo EMPTY o torno all'inizio
    // IMPORTANTE: NON mi fermo su DELETED!
    while (probeTable[idx].state != EMPTY) {
        
        // Controllo se è FULL e se la chiave corrisponde
        if (probeTable[idx].state == FULL && probeTable[idx].key == key) {
            return probeTable[idx].value;
        }
        
        idx = (idx + 1) % M;
        if (idx == startIdx) break; // Giro completo
    }
    
    return -1; // Non trovato
}

// DELETE (Linear Probing) - LAZY DELETION
void probeDelete(int key) {
    int idx = hash(key);
    int startIdx = idx;
    
    while (probeTable[idx].state != EMPTY) {
        
        if (probeTable[idx].state == FULL && probeTable[idx].key == key) {
            // Trovato! Segno come DELETED (Tombstone)
            // NON metto EMPTY, altrimenti rompo le ricerche future di chiavi collidenti
            probeTable[idx].state = DELETED;
            printf("[Probe] Cancellato chiave %d (Marcato DELETED)\n", key);
            return;
        }
        
        idx = (idx + 1) % M;
        if (idx == startIdx) break;
    }
    printf("[Probe] Chiave %d non trovata.\n", key);
}

// Funzione debug per vedere lo stato della tabella Probing
void printProbeTable() {
    printf("--- Stato Tabella Linear Probing ---\n");
    for(int i=0; i<M; i++) {
        if (probeTable[i].state == EMPTY) printf("[%d]: EMPTY\n", i);
        else if (probeTable[i].state == DELETED) printf("[%d]: XXX (DELETED)\n", i);
        else printf("[%d]: %d \n", i, probeTable[i].key);
    }
    printf("------------------------------------\n");
}

// ============================================================
// MAIN DI ESEMPIO
// ============================================================
int main() {
    // --- TEST CHAINING ---
    printf("\n=== TEST SEPARATE CHAINING ===\n");
    initChain();
    chainInsert(10, 100); // Hash 10%11 = 10
    chainInsert(21, 200); // Hash 21%11 = 10 (Collisione!)
    chainInsert(32, 300); // Hash 32%11 = 10 (Collisione!)
    
    // La lista all'indice 10 sarà: 32 -> 21 -> 10
    printf("Cerca 21: %d\n", chainSearch(21));
    chainDelete(21);
    printf("Cerca 21 dopo delete: %d\n", chainSearch(21));


    // --- TEST LINEAR PROBING ---
    printf("\n=== TEST LINEAR PROBING (Open Addressing) ===\n");
    initProbe();
    
    // Riempiamo per causare collisioni
    // M = 11
    probeInsert(10, 100); // Va in 10
    probeInsert(21, 200); // Hash 10 -> Occ. -> Va in 0 (10+1)%11
    probeInsert(32, 300); // Hash 10 -> Occ. -> 0 Occ. -> Va in 1
    
    printProbeTable();
    
    // Test Lazy Deletion
    printf("\n--- Test Lazy Deletion ---\n");
    printf("Cancello la chiave 21 (che si trova all'indice 0).\n");
    probeDelete(21); // Marca indice 0 come DELETED
    
    printProbeTable();
    
    printf("Cerco la chiave 32 (che si trova all'indice 1).\n");
    printf("Risultato: %d\n", probeSearch(32)); 
    // Spiegazione: L'hash di 32 è 10.
    // 1. Guarda in 10 -> Trovato 10 (NO)
    // 2. Guarda in 0 -> Trovato DELETED -> CONTINUA (Non si ferma!)
    // 3. Guarda in 1 -> Trovato 32 (SI)
    // Se avessimo messo EMPTY in 0, la ricerca si sarebbe fermata lì fallendo.

    return 0;
}