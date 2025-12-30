#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "automotive.h"

/* =========================================================================
 * SEZIONE 1: FUNZIONI AUSILIARIE (Non visibili dal main, statiche)
 * ========================================================================= */

/*
 * Scopo: Creare un nuovo nodo accessorio.
 * Concetti Chiave: Allocazione dinamica e inizializzazione.
 */
static Accessorio* crea_accessorio(char *nome, float prezzo) {
    Accessorio *new_node = (Accessorio*)malloc(sizeof(Accessorio));
    if (!new_node) {
        printf("Errore malloc accessorio\n");
        exit(1);
    }
    strcpy(new_node->nome, nome);
    new_node->prezzo = prezzo;
    new_node->next = NULL;
    return new_node;
}

/*
 * Scopo: Inserimento in testa a una lista (O(1)).
 * Concetti Chiave: Aggiornamento del puntatore di testa.
 */
static Accessorio* inserisci_accessorio(Accessorio *head, char *nome, float prezzo) {
    Accessorio *nuovo = crea_accessorio(nome, prezzo);
    nuovo->next = head;
    return nuovo; // La nuova testa è il nuovo nodo
}

/*
 * Scopo: Inserire un modello nel BST dei modelli.
 * Parametri: root (radice sottoalbero), nome (chiave), lista_acc (dati collegati).
 * Concetti Chiave: Ricorsione su BST. Se nome < root->nome vado a sx, altrimenti dx.
 */
static Modello* inserisci_modello_bst(Modello *root, char *nome, Accessorio *lista_acc) {
    if (root == NULL) {
        Modello *new_node = (Modello*)malloc(sizeof(Modello));
        strcpy(new_node->nome, nome);
        new_node->lista_accessori = lista_acc;
        new_node->left = new_node->right = NULL;
        return new_node;
    }
    if (strcmp(nome, root->nome) < 0)
        root->left = inserisci_modello_bst(root->left, nome, lista_acc);
    else if (strcmp(nome, root->nome) > 0)
        root->right = inserisci_modello_bst(root->right, nome, lista_acc);
    
    return root;
}

/*
 * Scopo: Inserire un produttore nel BST dei produttori.
 */
static Produttore* inserisci_produttore_bst(Produttore *root, char *nome, Modello *albero_modelli) {
    if (root == NULL) {
        Produttore *new_node = (Produttore*)malloc(sizeof(Produttore));
        strcpy(new_node->nome, nome);
        new_node->albero_modelli = albero_modelli;
        new_node->left = new_node->right = NULL;
        return new_node;
    }
    if (strcmp(nome, root->nome) < 0)
        root->left = inserisci_produttore_bst(root->left, nome, albero_modelli);
    else if (strcmp(nome, root->nome) > 0)
        root->right = inserisci_produttore_bst(root->right, nome, albero_modelli);
    
    return root;
}

/* =========================================================================
 * SEZIONE 2: CARICAMENTO DATI (File I/O)
 * ========================================================================= */

/*
 * Scopo: Legge il file degli accessori e restituisce la lista.
 */
static Accessorio* carica_accessori(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return NULL; // Se il file non esiste, nessun accessorio

    Accessorio *head = NULL;
    char nome[101];
    float prezzo;

    // fscanf restituisce il numero di campi letti. Continua finché ne legge 2.
    while (fscanf(fp, "%s %f", nome, &prezzo) == 2) {
        head = inserisci_accessorio(head, nome, prezzo);
    }
    fclose(fp);
    return head;
}

/*
 * Scopo: Legge il file modelli e costruisce l'albero dei modelli per un produttore.
 */
static Modello* carica_modelli(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return NULL;

    Modello *root_modelli = NULL;
    char nome_modello[101];
    char file_accessori[101];

    while (fscanf(fp, "%s %s", nome_modello, file_accessori) == 2) {
        // 1. Carico gli accessori per questo modello
        Accessorio *lista = carica_accessori(file_accessori);
        // 2. Inserisco il modello nell'albero
        root_modelli = inserisci_modello_bst(root_modelli, nome_modello, lista);
    }
    fclose(fp);
    return root_modelli;
}

/*
 * Scopo: Funzione Principale di Caricamento esposta nell'header.
 */
Produttore* carica_database(const char *nome_file_produttori) {
    FILE *fp = fopen(nome_file_produttori, "r");
    if (!fp) {
        printf("Errore: Impossibile aprire %s\n", nome_file_produttori);
        return NULL;
    }

    Produttore *root = NULL;
    char nome_prod[101];
    char file_modelli[101];

    while (fscanf(fp, "%s %s", nome_prod, file_modelli) == 2) {
        // 1. Carico l'albero dei modelli per questo produttore
        Modello *tree_mod = carica_modelli(file_modelli);
        // 2. Inserisco il produttore nell'albero principale
        root = inserisci_produttore_bst(root, nome_prod, tree_mod);
    }
    fclose(fp);
    return root;
}

/* =========================================================================
 * SEZIONE 3: RICERCA E VISUALIZZAZIONE
 * ========================================================================= */

/* Helper per stampare in-order (ordinato alfabeticamente) un albero di modelli */
static void stampa_modelli_inorder(Modello *root) {
    if (root != NULL) {
        stampa_modelli_inorder(root->left);
        printf(" - Modello: %s\n", root->nome);
        stampa_modelli_inorder(root->right);
    }
}

void stampa_modelli_produttore(Produttore *root, char *nome_produttore) {
    // Ricerca BST standard O(log P)
    while (root != NULL) {
        int cmp = strcmp(nome_produttore, root->nome);
        if (cmp == 0) {
            // Trovato!
            printf("Modelli per %s:\n", root->nome);
            stampa_modelli_inorder(root->albero_modelli);
            return;
        } else if (cmp < 0) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    printf("Produttore %s non trovato.\n", nome_produttore);
}

/* Helper per trovare un modello in un albero modelli */
static Modello* cerca_modello_bst(Modello *root, char *nome_modello) {
    while (root != NULL) {
        int cmp = strcmp(nome_modello, root->nome);
        if (cmp == 0) return root;
        if (cmp < 0) root = root->left;
        else root = root->right;
    }
    return NULL;
}

/* Helper ricorsivo per cercare un modello tra TUTTI i produttori */
static Modello* trova_modello_globale(Produttore *prod_root, char *nome_modello) {
    if (prod_root == NULL) return NULL;

    // 1. Cerco nel produttore corrente
    Modello *res = cerca_modello_bst(prod_root->albero_modelli, nome_modello);
    if (res != NULL) return res;

    // 2. Se non trovato, cerco a sinistra nei produttori
    res = trova_modello_globale(prod_root->left, nome_modello);
    if (res != NULL) return res;

    // 3. Se non trovato, cerco a destra
    return trova_modello_globale(prod_root->right, nome_modello);
}

void stampa_accessori_modello(Produttore *root, char *nome_modello) {
    // Devo cercare il modello. Il testo dice che i nomi sono univoci globalmente.
    // Ma non so di quale produttore sia, quindi devo scorrere i produttori.
    // Nota: Il testo chiedeva O(log N modelli) se conosco il produttore, 
    // ma qui chiede input solo modello, quindi devo cercare ovunque.
    
    Modello *mod = trova_modello_globale(root, nome_modello);
    
    if (mod) {
        printf("Accessori per %s:\n", mod->nome);
        Accessorio *curr = mod->lista_accessori;
        while (curr) {
            printf(" - %s (%.2f Euro)\n", curr->nome, curr->prezzo);
            curr = curr->next;
        }
    } else {
        printf("Modello %s non trovato nel database.\n", nome_modello);
    }
}

/* =========================================================================
 * SEZIONE 4: CANCELLAZIONE (Logica Complessa)
 * ========================================================================= */

/* Libera memoria lista accessori */
static void free_accessori(Accessorio *head) {
    while (head) {
        Accessorio *temp = head;
        head = head->next;
        free(temp);
    }
}

/* Libera memoria albero modelli */
static void free_modelli(Modello *root) {
    if (root) {
        free_modelli(root->left);
        free_modelli(root->right);
        free_accessori(root->lista_accessori);
        free(root);
    }
}

/* Trova il minimo in un BST (usato per la cancellazione) */
static Produttore* min_value_node(Produttore* node) {
    Produttore* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

/* Cancellazione standard nodo BST */
Produttore* cancella_produttore(Produttore *root, char *nome_prod) {
    if (root == NULL) return root;

    if (strcmp(nome_prod, root->nome) < 0)
        root->left = cancella_produttore(root->left, nome_prod);
    else if (strcmp(nome_prod, root->nome) > 0)
        root->right = cancella_produttore(root->right, nome_prod);
    else {
        // Nodo trovato. Cancellazione!
        
        // Caso 1: Nessun figlio o solo uno
        if (root->left == NULL) {
            Produttore *temp = root->right;
            free_modelli(root->albero_modelli); // PULIZIA MEMORIA!
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Produttore *temp = root->left;
            free_modelli(root->albero_modelli); // PULIZIA MEMORIA!
            free(root);
            return temp;
        }

        // Caso 2: Due figli. Prendo il successore in-order (minimo del sottoalbero destro)
        Produttore *temp = min_value_node(root->right);

        // Copio i dati
        strcpy(root->nome, temp->nome);
        // Attenzione: qui bisognerebbe gestire il passaggio di proprietà dell'albero modelli
        // Per semplicità didattica, assumiamo una copia superficiale dei puntatori e cancellazione nodo successivo
        // In un caso reale complesso, questo è delicato (shallow copy vs deep copy).
        // QUI facciamo una semplificazione: swap manuale dei puntatori è troppo complesso per questo livello,
        // ci limitiamo a dire che in un BST "scolastico" si copiano i campi info e si cancella il duplicato.
        
        // ATTENZIONE: Copiando il nome e l'albero modelli, dobbiamo evitare che l'albero modelli venga liberato
        // quando cancelliamo 'temp' nella chiamata ricorsiva.
        // Ma 'temp' ha i SUOI modelli. Quindi:
        free_modelli(root->albero_modelli); // Libero i modelli del nodo che sto sovrascrivendo
        root->albero_modelli = temp->albero_modelli; // Prendo i modelli del successore
        temp->albero_modelli = NULL; // Imposto a NULL il successore così la free non li cancella

        root->right = cancella_produttore(root->right, temp->nome);
    }
    return root;
}

/* --- Cancellazione Modello (Standard BST logic su albero modelli) --- */
static Modello* min_value_modello(Modello* node) {
    Modello* current = node;
    while (current && current->left != NULL) current = current->left;
    return current;
}

static Modello* delete_modello_node(Modello *root, char *nome) {
    if (root == NULL) return root;

    if (strcmp(nome, root->nome) < 0)
        root->left = delete_modello_node(root->left, nome);
    else if (strcmp(nome, root->nome) > 0)
        root->right = delete_modello_node(root->right, nome);
    else {
        // Trovato
        if (root->left == NULL) {
            Modello *temp = root->right;
            free_accessori(root->lista_accessori);
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Modello *temp = root->left;
            free_accessori(root->lista_accessori);
            free(root);
            return temp;
        }
        Modello *temp = min_value_modello(root->right);
        strcpy(root->nome, temp->nome);
        free_accessori(root->lista_accessori);
        root->lista_accessori = temp->lista_accessori;
        temp->lista_accessori = NULL; 
        root->right = delete_modello_node(root->right, temp->nome);
    }
    return root;
}

/* Visita tutti i produttori per trovare e cancellare il modello */
void cancella_modello_globale(Produttore *root, char *nome_modello) {
    if (root == NULL) return;
    
    // Tenta cancellazione nel produttore corrente
    root->albero_modelli = delete_modello_node(root->albero_modelli, nome_modello);
    
    // Ricorsione sugli altri
    cancella_modello_globale(root->left, nome_modello);
    cancella_modello_globale(root->right, nome_modello);
}

/* --- Cancellazione Accessorio --- */
static void delete_acc_from_list(Modello *mod, char *nome_acc) {
    Accessorio *curr = mod->lista_accessori;
    Accessorio *prev = NULL;

    while (curr != NULL) {
        if (strcmp(curr->nome, nome_acc) == 0) {
            if (prev == NULL) {
                mod->lista_accessori = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            printf("Accessorio cancellato.\n");
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

/* * Helper statico (definito fuori) per attraversare l'albero dei modelli 
 * e cancellare l'accessorio specificato da ogni modello.
 */
static void traversa_modelli_helper(Modello *m, char *nome_accessorio) {
    if (m != NULL) {
        // 1. Provo a cancellare l'accessorio dalla lista di questo modello
        delete_acc_from_list(m, nome_accessorio);
        
        // 2. Continuo la visita ricorsiva
        traversa_modelli_helper(m->left, nome_accessorio);
        traversa_modelli_helper(m->right, nome_accessorio);
    }
}

void cancella_accessorio_globale(Produttore *root, char *nome_accessorio) {
    // Caso base: albero produttori vuoto
    if (root == NULL) return;

    // 1. Lancio l'helper sull'albero dei modelli del produttore corrente
    traversa_modelli_helper(root->albero_modelli, nome_accessorio);

    // 2. Continuo la visita ricorsiva sugli altri produttori (BST Produttori)
    cancella_accessorio_globale(root->left, nome_accessorio);
    cancella_accessorio_globale(root->right, nome_accessorio);
}

/* =========================================================================
 * SEZIONE 5: INCORPORAZIONE (MERGE)
 * ========================================================================= */

/* Funzione helper per spostare nodi da un albero all'altro */
static void sposta_modelli(Modello *sorgente, Modello **destinazione_root) {
    if (sorgente == NULL) return;

    // Salvo i sottoalberi perché 'sorgente' verrà modificato nell'inserimento
    Modello *sx = sorgente->left;
    Modello *dx = sorgente->right;

    // Stacco il nodo sorgente dall'albero vecchio
    sorgente->left = NULL;
    sorgente->right = NULL;

    // Lo inserisco nel nuovo albero (logica BST riutilizzata ma adattata ai nodi esistenti)
    // Nota: qui per semplicità usiamo l'inserimento per copia o logica simile. 
    // Per un merge vero di nodi esistenti, dovremmo scrivere una insert che accetta Node* invece di char*.
    // Facciamo la via facile: ricreiamo il nodo nella destinazione e liberiamo il vecchio contenitore,
    // MA manteniamo la lista accessori (passaggio di proprietà).
    
    *destinazione_root = inserisci_modello_bst(*destinazione_root, sorgente->nome, sorgente->lista_accessori);
    
    // Libero il nodo "guscio" vecchio (la lista accessori è stata salvata nel nuovo nodo)
    free(sorgente); 

    // Ricorsione
    sposta_modelli(sx, destinazione_root);
    sposta_modelli(dx, destinazione_root);
}

Produttore* incorpora_produttori(Produttore *root, char *prod1, char *prod2) {
    printf("Fusione di %s in %s...\n", prod2, prod1);
    
    // 1. Trovo i puntatori ai nodi (non posso usare le funzioni di stampa, mi servono i nodi)
    // Implementazione rapida ricerca
    Produttore *p1 = root; 
    while(p1 && strcmp(p1->nome, prod1) != 0) 
        p1 = (strcmp(prod1, p1->nome) < 0) ? p1->left : p1->right;

    Produttore *p2 = root;
    while(p2 && strcmp(p2->nome, prod2) != 0) 
        p2 = (strcmp(prod2, p2->nome) < 0) ? p2->left : p2->right;

    if (!p1 || !p2) {
        printf("Errore: uno dei produttori non esiste.\n");
        return root;
    }

    // 2. Travaso tutti i modelli di p2 in p1
    sposta_modelli(p2->albero_modelli, &(p1->albero_modelli));
    
    // 3. Importante: Setto l'albero di p2 a NULL perché ho spostato o liberato i suoi contenuti
    p2->albero_modelli = NULL;

    // 4. Cancello p2 dal database principale
    root = cancella_produttore(root, prod2);

    printf("Fusione completata.\n");
    return root;
}

void libera_tutto(Produttore *root) {
    if (root) {
        libera_tutto(root->left);
        libera_tutto(root->right);
        free_modelli(root->albero_modelli);
        free(root);
    }
}