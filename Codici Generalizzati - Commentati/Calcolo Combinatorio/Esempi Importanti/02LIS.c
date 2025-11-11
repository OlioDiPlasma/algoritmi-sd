#include <stdlib.h>
#include <stdio.h>

#define N 16   // Numero totale di elementi nel vettore

/* =======================
   PROTOTIPI DELLE FUNZIONI
   ======================= */
int check(int *val, int k, int *sol, int len);
void powerset(int pos, int *val, int *sol, int k, int *maxlen, int *bestsol);

/* ============================================================
 * SCOPO DEL PROGRAMMA:
 * Trova UNA DELLE più lunghe **sottosequenze crescenti** (LIS)
 * contenute nel vettore `val[]`, usando il principio del POWERSET.
 *
 * Cioè: genera tutti i sottoinsiemi di `val[]` e tiene traccia
 * di quello che è crescente e di lunghezza massima.
 * ============================================================ */
int main(void) {

  int i;
  int val[N] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
  int sol[N], bestsol[N], maxlen = 0;

  // Chiamata iniziale alla funzione ricorsiva per generare il powerset
  powerset(0, val, sol, N, &maxlen, bestsol);

  // Stampa del risultato finale
  printf("One of the longest increasing sequences has length %d and is:\n", maxlen);
  printf("{ ");
  for (i = 0; i < N; i++)
    if (bestsol[i] != 0)   // Stampa solo gli elementi inclusi
      printf("%d ", val[i]);
  printf("}\n");

  return 0;
}

/* ============================================================
 * FUNZIONE powerset()
 * ------------------------------------------------------------
 * Genera tutte le combinazioni (sottoinsiemi) di elementi di val[]
 * utilizzando un array binario `sol[]` di lunghezza k:
 *   - sol[i] = 0 → elemento val[i] escluso
 *   - sol[i] = 1 → elemento val[i] incluso
 *
 * Ad ogni passo, quando `pos == k`, viene verificato se la
 * sottosequenza scelta è crescente, e se sì, confrontata con
 * la migliore trovata finora.
 * ============================================================ */
void powerset(int pos, int *val, int *sol, int k, int *maxlen, int *bestsol) {
  int j, len = 0, ris;

  // ----- CASO BASE: abbiamo deciso per tutti gli elementi -----
  if (pos >= k) {
    // Conta quanti elementi sono stati selezionati (sol[i] == 1)
    for (j = 0; j < k; j++)
      if (sol[j] != 0)
        len++;

    // Controlla se la sottosequenza scelta è crescente
    ris = check(val, k, sol, len);

    // Se è crescente e più lunga della migliore trovata, aggiornala
    if (ris == 1)
      if (len >= *maxlen) {
        for (j = 0; j < k; j++)
          bestsol[j] = sol[j];
        *maxlen = len;
      }
    return;
  }

  // ----- CASO RICORSIVO -----
  // Scelta 1: NON includere val[pos]
  sol[pos] = 0;
  powerset(pos + 1, val, sol, k, maxlen, bestsol);

  // Scelta 2: INCLUDERE val[pos]
  sol[pos] = 1;
  powerset(pos + 1, val, sol, k, maxlen, bestsol);

  return;
}

/* ============================================================
 * FUNZIONE check()
 * ------------------------------------------------------------
 * Verifica se la sottosequenza scelta (marcata in sol[])
 * è **strettamente crescente**.
 *
 * Esempio:
 *   val = [0, 8, 4, 12]
 *   sol = [1, 0, 1, 1]  → sottosequenza = [0, 4, 12]
 *   → crescente ⇒ ritorna 1
 *
 * Parametri:
 *   val[] - vettore originale
 *   k     - lunghezza di val[]
 *   sol[] - vettore binario (0/1) che indica cosa è scelto
 *   len   - numero di elementi scelti
 * ============================================================ */
int check(int *val, int k, int *sol, int len) {
  int i = 0, j, t, ok = 1;

  // Scorri gli elementi scelti confrontando coppie consecutive
  for (t = 0; t < len - 1; t++) {
    // Trova il primo elemento selezionato
    while ((sol[i] == 0) && (i < k - 1))
      i++;

    // Trova il successivo elemento selezionato
    j = i + 1;
    while ((sol[j] == 0) && (j < k))
      j++;

    // Se la sequenza non è crescente, fallisce
    if (val[i] > val[j])
      ok = 0;

    i = j; // passa al successivo
  }

  return ok;
}
