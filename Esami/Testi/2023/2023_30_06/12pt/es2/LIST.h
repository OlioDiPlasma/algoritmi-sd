#ifndef LIST_H
#define LIST_H

/* --- DEFINIZIONE DEI TIPI --- */

/* * NODE (Quasi ADT): 
 * Lo dichiariamo come struttura opaca nel .h se vogliamo nascondere tutto, 
 * oppure lo lasciamo visibile nel .c. La traccia chiede "come ADT I e quasi ADT", 
 * quindi esponiamo solo i typedef qui.
 */
typedef struct node NODE;

/* * LIST (ADT I Categoria):
 * È un puntatore a una struttura "wrapper" (rappresentazione interna).
 * L'utente gestirà sempre oggetti di tipo LIST, mai struct list_rep*.
 */
typedef struct list_rep *LIST;

/* --- PROTOTIPO DELLA FUNZIONE RICHIESTA --- */

/* * Scopo: Compattare la lista eliminando nodi a indici multipli di 3.
 * Parametro: l (La lista da elaborare).
 */
void f(LIST l);

/* --- FUNZIONI DI UTILITÀ (Per il test nel main) --- */
LIST creaListaVuota();
void aggiungiInCoda(LIST l, int val);
void stampaLista(LIST l);
void distruggiLista(LIST l);

#endif