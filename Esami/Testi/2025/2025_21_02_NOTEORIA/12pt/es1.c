#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ======================================================================================
 * 1. DEFINIZIONE DELLE STRUTTURE DATI
 * ======================================================================================
 */

// Struct 'dummy' per il libro. 
// Il testo dice che è esterna, quindi non ci interessa il contenuto interno.
struct libro {
    char title[100];
    // ... altri campi ...
};

// Struct che rappresenta un singolo Editore
typedef struct {
    char *nome;             // Stringa allocata dinamicamente
    struct libro **libri;   // Vettore dinamico di PUNTATORI a struct libro
    int n_libri;            // Dimensione del vettore libri
} editore_t;

// Struct principale dell'ADT (Wrapper)
struct editori {
    editore_t *editori;     // Vettore dinamico di editori
    int n_editori;          // Dimensione del vettore editori
};

// Puntatore opaco all'ADT
typedef struct editori *editori_adt;

/*
 * ======================================================================================
 * FUNZIONE: EditoriDup (DUPLICAZIONE ADT)
 * ======================================================================================
 * Scopo:
 * Creare una copia esatta e indipendente dell'ADT (Deep Copy strutturale).
 * * Parametri:
 * - editori_adt e: L'ADT originale da copiare.
 * * Concetti Chiave:
 * - Allocazione a Cascata: Dobbiamo allocare prima il contenitore esterno, poi i vettori
 * interni, e infine le stringhe.
 * - Copia dei Puntatori (Shallow Copy dei Libri): Poiché i libri sono "esterni",
 * NON dobbiamo duplicare la `struct libro`. Copiamo solo l'indirizzo di memoria.
 * Se copiamo l'indirizzo, entrambi gli ADT punteranno allo stesso libro fisico in RAM.
 */
editori_adt EditoriDup(editori_adt e) {
    /* --- 1. CONTROLLI DI SICUREZZA --- */
    if (e == NULL) return NULL;

    /* --- 2. ALLOCAZIONE DEL WRAPPER PRINCIPALE --- */
    // Allocazione della struct 'struct editori'
    editori_adt e2 = (editori_adt)malloc(sizeof(struct editori));
    if (e2 == NULL) return NULL; // Gestione errore malloc

    // Copia del campo scalare (numero di editori)
    e2->n_editori = e->n_editori;

    /* --- 3. ALLOCAZIONE VETTORE EDITORI --- */
    // Allocazione dell'array di struct editore_t
    e2->editori = (editore_t *)malloc(e2->n_editori * sizeof(editore_t));
    
    // Controllo allocazione paranoico (buona pratica didattica)
    if (e2->editori == NULL) {
        free(e2);
        return NULL;
    }

    /* --- 4. CICLO DI COPIA PROFONDA (DEEP COPY) --- */
    for (int i = 0; i < e2->n_editori; i++) {
        // Alias per comodità di lettura
        // src = sorgente (originale), dst = destinazione (nuovo)
        editore_t *src = &e->editori[i];
        editore_t *dst = &e2->editori[i];

        // A. Copia campi scalari
        dst->n_libri = src->n_libri;

        // B. Duplicazione Stringa (NOME)
        // strdup fa malloc + strcpy in automatico
        dst->nome = strdup(src->nome); 

        // C. Allocazione Vettore Puntatori ai Libri
        // Notare il sizeof: stiamo allocando puntatori (struct libro *), non intere struct!
        dst->libri = (struct libro **)malloc(dst->n_libri * sizeof(struct libro *));

        // D. Copia dei Riferimenti (SHALLOW COPY DEI LIBRI)
        // Qui copiamo solo gli indirizzi. Non facciamo malloc per i libri.
        for (int j = 0; j < dst->n_libri; j++) {
            dst->libri[j] = src->libri[j]; 
        }
    }

    return e2;
}

/*
 * ======================================================================================
 * FUNZIONE: EditoriFree (DEALLOCAZIONE ADT)
 * ======================================================================================
 * Scopo:
 * Liberare tutta la memoria allocata dall'ADT, evitando Memory Leaks.
 * * Concetti Chiave:
 * - Ordine Inverso: Si deve liberare sempre dall'interno verso l'esterno.
 * (Prima le foglie, poi i rami, poi il tronco).
 * - Non toccare i Libri: Poiché non li abbiamo allocati noi (sono esterni),
 * NON dobbiamo fare free sui puntatori `libri[j]`. Liberiamo solo il vettore che li contiene.
 */
void EditoriFree(editori_adt e) {
    if (e == NULL) return;

    /* --- 1. CICLO DI PULIZIA INTERNA --- */
    for (int i = 0; i < e->n_editori; i++) {
        // A. Liberiamo la stringa del nome (allocata con strdup)
        free(e->editori[i].nome);

        // B. Liberiamo il vettore di puntatori ai libri
        // (Nota: NON facciamo un ciclo for interno per liberare i singoli libri!)
        free(e->editori[i].libri);
    }

    /* --- 2. PULIZIA ESTERNA --- */
    // Liberiamo il vettore degli editori
    free(e->editori);

    // Liberiamo la struttura principale
    free(e);
}

/*
 * ======================================================================================
 * MAIN DI TEST (TEST HARNESS)
 * ======================================================================================
 */
int main() {
    /* --- 1. SETUP DATI ESTERNI (MOCK) --- */
    // Creiamo dei libri "esterni" che esistono indipendentemente dall'ADT
    struct libro b1 = {"Il Signore degli Anelli"};
    struct libro b2 = {"Lo Hobbit"};
    
    /* --- 2. CREAZIONE MANUALE ADT (Simulazione) --- */
    // Normalmente ci sarebbe una funzione di insert, qui lo facciamo a mano per il test
    editori_adt adt1 = malloc(sizeof(struct editori));
    adt1->n_editori = 1;
    adt1->editori = malloc(sizeof(editore_t) * 1);
    
    // Popoliamo il primo editore
    adt1->editori[0].nome = strdup("Mondadori");
    adt1->editori[0].n_libri = 2;
    adt1->editori[0].libri = malloc(sizeof(struct libro*) * 2);
    adt1->editori[0].libri[0] = &b1; // Puntiamo ai libri statici
    adt1->editori[0].libri[1] = &b2;

    printf("ADT 1 creato: %s con %d libri.\n", adt1->editori[0].nome, adt1->editori[0].n_libri);

    /* --- 3. TEST DUPLICAZIONE --- */
    printf("Duplicazione in corso...\n");
    editori_adt adt2 = EditoriDup(adt1);

    // Modifichiamo la copia per assicurarci che sia indipendente (Deep Copy strutturale)
    free(adt2->editori[0].nome);
    adt2->editori[0].nome = strdup("Einaudi"); // Cambio nome nella copia

    printf("ADT 1 Nome: %s (Atteso: Mondadori)\n", adt1->editori[0].nome);
    printf("ADT 2 Nome: %s (Atteso: Einaudi)\n", adt2->editori[0].nome);
    
    // Verifichiamo che i libri siano condivisi (Shallow Copy dei contenuti)
    printf("Indirizzo Libro 1 in ADT1: %p\n", (void*)adt1->editori[0].libri[0]);
    printf("Indirizzo Libro 1 in ADT2: %p\n", (void*)adt2->editori[0].libri[0]);

    /* --- 4. TEST DEALLOCAZIONE --- */
    printf("Deallocazione...\n");
    EditoriFree(adt1);
    EditoriFree(adt2);
    printf("Memoria liberata con successo.\n");

    return 0;
}