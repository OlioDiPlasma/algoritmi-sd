#ifndef BST_H
#define BST_H

/* --- DEFINIZIONE TIPO OPACO (ADT I CLASSE) --- */
/*
 * BT è un puntatore a una struttura "incompleta".
 * Il compilatore sa che esiste, ma non ne conosce i campi.
 * Questo impedisce al main di accedere a t->root direttamente.
 */
typedef struct bt_wrapper *BT;

/* --- PROTOTIPO DELLA FUNZIONE RICHIESTA --- */

/*
 * Funzione: f (wrapper)
 * Scopo: Verificare se l'albero t è un Binary Search Tree (BST).
 * Parametri:
 * - BT t: L'albero da analizzare.
 * Ritorna: 1 se è un BST, 0 altrimenti.
 */
int f(BT t);

/* --- FUNZIONI DI UTILITÀ PER IL TEST --- */
/* Servono solo per creare dati di prova nel main senza violare l'incapsulamento */
BT crea_albero_valido();
BT crea_albero_invalido();
void libera_albero(BT t);

#endif