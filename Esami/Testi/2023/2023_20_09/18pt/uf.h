#ifndef UF_H
#define UF_H

/* * ======================================================================================
 * TIPO DI DATO ASTRATTO (ADT) DI I CATEGORIA
 * ======================================================================================
 * Qui definiamo 'UF' come un puntatore a una struct incompleta (struct uf_struct).
 * * CONCETTO CHIAVE: Information Hiding (Incapsulamento)
 * Il file che include questo header (es. verifica.c) saprà solo che esiste un oggetto
 * di tipo 'UF', ma non potrà accedere ai suoi campi interni (parent, rank) direttamente.
 * Dovrà obbligatoriamente passare attraverso le funzioni fornite (UF_init, UF_find, etc.).
 */
typedef struct uf_struct *UF; // Puntatore opaco

/* --- PROTOTIPI DELLE FUNZIONI --- */

/* Alloca e inizializza la struttura dati per n elementi */
UF UF_init(int n); //TECNICAMENTE DI LIBRERIA TUTTE E 4

/* Trova il rappresentante (radice) dell'insieme a cui appartiene l'elemento i */
int UF_find(UF uf, int i);

/* Unisce gli insiemi contenenti i e j. Ritorna 1 se unione avvenuta, 0 se già uniti */
int UF_union(UF uf, int i, int j);

/* Libera la memoria allocata */
void UF_free(UF uf);

#endif