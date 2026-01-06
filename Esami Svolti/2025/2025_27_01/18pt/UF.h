#ifndef UF_H
#define UF_H

/* --- DEFINIZIONE TIPO OPACO --- */
typedef struct check_union_find *UF;

/* --- PROTOTIPI --- */
/* Inizializza la struttura dati con N elementi disgiunti */
UF  UFinit(int N);

/* Libera la memoria */
void UFfree(UF uf);

/* Ritorna 1 se p e q sono connessi, 0 altrimenti */
int UFfind(UF uf, int p, int q);

/* Unisce gli insiemi contenenti p e q. Salva l'operazione per il backtrack */
void UFunion(UF uf, int p, int q);

/* Annulla l'ultima operazione di union (Tasto "UNDO") */
void UFbacktrack(UF uf);

#endif