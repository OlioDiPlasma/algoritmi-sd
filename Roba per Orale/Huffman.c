#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- STRUTTURE DATI ---

// Nodo dell'albero di Huffman
struct MinHeapNode {
    char data;              // Il carattere (utile solo per le foglie)
    unsigned freq;          // La frequenza del carattere
    struct MinHeapNode *left, *right; // Figli
};

// Struttura per il Min Heap (Coda con priorità)
struct MinHeap {
    unsigned size;          // Numero attuale di elementi
    unsigned capacity;      // Capacità massima
    struct MinHeapNode** array; // Array di puntatori ai nodi
};

// --- FUNZIONI DI SUPPORTO PER I NODI ---

// Crea un nuovo nodo
struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// --- FUNZIONI PER IL MIN HEAP ---

// Crea un Min Heap di data capacità
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// Scambia due nodi del Min Heap
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// Standard minHeapify (per mantenere la proprietà di heap: padre <= figli)
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Controlla se la dimensione è 1 (abbiamo finito)
int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}

// Estrae il nodo con frequenza minima
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// Inserisce un nuovo nodo nel Min Heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    // Risale l'albero finché non trova la posizione corretta
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// Costruisce il Min Heap iniziale
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// --- LOGICA PRINCIPALE DI HUFFMAN ---

// Verifica se è una foglia
int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

// Crea e costruisce il Min Heap iniziale basato su caratteri e frequenze
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

// Funzione principale che costruisce l'albero di Huffman
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;

    // Passo 1: Crea Min Heap con capacità uguale al numero di caratteri
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    // Itera finché la dimensione dello heap non diventa 1
    while (!isSizeOne(minHeap)) {
        // Passo 2: Estrai i due nodi con frequenza minima
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Passo 3: Crea un nuovo nodo interno con frequenza pari alla somma
        // Il carattere '$' è un placeholder per i nodi interni
        top = newNode('$', left->freq + right->freq);
        
        top->left = left;
        top->right = right;

        // Passo 4: Inserisci il nuovo nodo nello heap
        insertMinHeap(minHeap, top);
    }

    // Passo 5: Il nodo rimanente è la radice
    return extractMin(minHeap);
}

// Funzione ricorsiva per stampare i codici dall'albero
// arr[] memorizza il percorso di bit, top è l'indice corrente
void printCodes(struct MinHeapNode* root, int arr[], int top) {
    // Assegna 0 al lato sinistro
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    // Assegna 1 al lato destro
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    // Se è una foglia, stampa il carattere e il suo codice
    if (isLeaf(root)) {
        printf("Carattere: %c | Frequenza: %d | Codice: ", root->data, root->freq);
        for (int i = 0; i < top; ++i)
            printf("%d", arr[i]);
        printf("\n");
    }
}

// Wrapper finale per l'utente
void HuffmanCodes(char data[], int freq[], int size) {
    // Costruisci l'albero
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);

    // Stampa i codici
    int arr[100], top = 0;
    
    printf("--- Codici di Huffman Generati ---\n");
    printCodes(root, arr, top);
}

// --- MAIN DI ESEMPIO ---
int main() {
    char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
    int freq[] = { 5, 9, 12, 13, 16, 45 };
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("Input: 6 caratteri con frequenze diverse.\n");
    HuffmanCodes(arr, freq, size);

    return 0;
}