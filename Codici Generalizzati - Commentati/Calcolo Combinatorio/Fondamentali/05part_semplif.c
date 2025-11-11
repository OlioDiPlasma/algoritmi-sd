#include <stdio.h>
#include <stdlib.h>
#define MAX 6

// ===================================================================
// DICHIARAZIONE DELLA FUNZIONE RICORSIVA
// ===================================================================
// disp_ripet:
//   Genera tutte le possibili assegnazioni di n oggetti a k blocchi,
//   permettendo ripetizioni (ogni oggetto può andare in uno dei k blocchi).
//   Quando tutti gli oggetti sono assegnati, stampa la partizione.
//
// Parametri:
//   pos → posizione corrente (oggetto che stiamo assegnando)
//   val → array degli oggetti (val[i] = valore dell'i-esimo oggetto)
//   sol → array soluzione (sol[i] = blocco assegnato all'oggetto i)
//   n   → numero di oggetti
//   k   → numero di blocchi
// ===================================================================
void disp_ripet(int pos, int *val, int *sol, int n, int k);

int main(void) {
  int i, n, k, *sol, *val;

  // Messaggio introduttivo
  printf("\nCalcolo delle partizioni (semplificate) di n oggetti in k blocchi\n");

  // Lettura del numero di oggetti e blocchi
  printf("Inserisci il numero n di oggetti: ");
  scanf("%d", &n);
  printf("Inserisci il numero k di blocchi: ");
  scanf("%d", &k);

  // Allocazione dinamica degli array:
  //  val[] contiene i valori degli oggetti
  //  sol[] contiene l’assegnazione (indice del blocco) per ciascun oggetto
  val = malloc(n * sizeof(int));
  sol = malloc(n * sizeof(int));

  if (val == NULL || sol == NULL) {
    printf("Errore di allocazione memoria.\n");
    exit(EXIT_FAILURE);
  }

  // Inizializzo sol[] a -1 (significa: oggetto non ancora assegnato)
  for (i = 0; i < n; i++)
    sol[i] = -1;

  // Lettura dei valori degli oggetti
  for (i = 0; i < n; i++) {
    printf("Inserisci elemento %d: ", i);
    scanf("%d", &val[i]);
  }

  printf("\nLe partizioni di %d oggetti su %d blocchi sono:\n", n, k);
  printf("(Attenzione: non si eliminano le simmetrie tra blocchi!)\n\n");

  // Chiamata ricorsiva iniziale
  disp_ripet(0, val, sol, n, k);

  // Libero la memoria allocata
  free(val);
  free(sol);

  return 0;
}

// ===================================================================
// FUNZIONE RICORSIVA disp_ripet()
// ===================================================================
//
// IDEA:
//  Ogni oggetto può essere assegnato a uno dei k blocchi.
//  Si esplorano quindi tutte le k^n possibili assegnazioni.
//  A ogni passo:
//    - pos indica quale oggetto stiamo assegnando
//    - sol[pos] indica il blocco scelto (0..k-1)
//
// CASO BASE:
//   quando pos == n → tutti gli oggetti sono stati assegnati
//   → controlliamo se ogni blocco contiene almeno un oggetto
//     (altrimenti la "partizione" non è valida)
//
// CASO RICORSIVO:
//   per l’oggetto in posizione 'pos', proviamo tutti i k blocchi possibili
//   e chiamiamo ricorsivamente disp_ripet() per pos+1.
//
// ===================================================================
void disp_ripet(int pos, int *val, int *sol, int n, int k) {
  int i, j, ok = 1;
  int *occ; // occ[i] conterà quanti oggetti sono nel blocco i

  // ---------------------------------------------------------------
  // CASO BASE: abbiamo assegnato tutti gli oggetti
  // ---------------------------------------------------------------
  if (pos >= n) {
    // Alloco un array per contare quanti oggetti sono in ciascun blocco
    occ = calloc(k, sizeof(int));

    // Conta quante volte ogni blocco appare in sol[]
    for (j = 0; j < n; j++)
      occ[sol[j]]++;

    // Verifico che ogni blocco abbia almeno un elemento
    i = 0;
    while (i < k && ok) {
      if (occ[i] == 0)
        ok = 0; // se un blocco è vuoto, la partizione non è valida
      i++;
    }

    // Se un blocco è vuoto, scarto la soluzione
    if (ok == 0) {
      free(occ);
      return;
    }

    // Altrimenti: stampo la partizione
    printf("partizione: ");
    for (i = 0; i < k; i++) {
      printf("{ ");
      // stampo tutti gli oggetti che appartengono al blocco i
      for (j = 0; j < n; j++) {
        if (sol[j] == i)
          printf("%d ", val[j]);
      }
      printf("}  ");
    }
    printf("\n");

    // Libero memoria e torno indietro nella ricorsione
    free(occ);
    return;
  }

  // ---------------------------------------------------------------
  // CASO RICORSIVO: assegno l’oggetto 'pos' a uno dei k blocchi
  // ---------------------------------------------------------------
  for (i = 0; i < k; i++) {
    sol[pos] = i; // assegno l'oggetto al blocco i
    disp_ripet(pos + 1, val, sol, n, k);
  }
}

/* ===================================================================
 * ESEMPIO DI ESECUZIONE (n=3, k=2)
 * ===================================================================
 *
 * Oggetti: val = [1, 2, 3]
 * Blocchi: 0 e 1
 *
 * Albero delle decisioni:
 *
 *          pos=0
 *      /           \
 *   sol[0]=0     sol[0]=1
 *    /   \         /   \
 * pos=1 pos=1   pos=1 pos=1
 *  / \    / \     / \   / \
 * 0  1   0  1    0  1  0  1
 * ...
 * (fino a pos=3)
 *
 * Ogni foglia dell’albero rappresenta un’assegnazione:
 *   es. sol = [0,1,0] significa:
 *       oggetto 1 → blocco 0
 *       oggetto 2 → blocco 1
 *       oggetto 3 → blocco 0
 *
 * Si contano solo le soluzioni in cui ogni blocco ha ≥1 oggetto.
 *
 * Output esempio:
 * partizione: { 1 3 } { 2 }
 * partizione: { 1 2 } { 3 }
 * partizione: { 2 3 } { 1 }
 *
 * (Totale = 3 partizioni valide su 2 blocchi)
 *
 * Nota: le simmetrie NON sono eliminate,
 * quindi scambiare i blocchi produce partizioni equivalenti
 * ma comunque stampate entrambe.
 * ===================================================================
 */
