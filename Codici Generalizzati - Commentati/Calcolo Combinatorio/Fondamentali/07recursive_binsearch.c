#include <stdio.h>
#include <stdlib.h>

#define V 8  // Numero massimo di elementi del vettore

// ===================================================================
// PROTOTIPO DELLA FUNZIONE
// ===================================================================
// BinSearch:
//   Implementa la ricerca binaria (ricorsiva) su un array ORDINATO.
//
// Parametri:
//   v[] : array di interi ordinati in modo crescente
//   l   : indice sinistro dell'intervallo di ricerca
//   r   : indice destro dell'intervallo di ricerca
//   k   : chiave (valore) da cercare
//
// Restituisce:
//   - l'indice (posizione) in cui si trova la chiave k, se presente
//   - -1 se la chiave non è trovata
// ===================================================================
int BinSearch(int v[], int l, int r, int k);

int main() {
  int i, index, k, v[V];

  // ===============================================================
  // LETTURA DEGLI ELEMENTI DELL’ARRAY
  // ===============================================================
  printf("Ricerca binaria ricorsiva su vettore ordinato\n\n");

  for (i = 0; i < V; i++) {
    printf("Input v[%d]: ", i);
    scanf("%d", &v[i]);
  }

  // ⚠️ IMPORTANTE:
  // L’algoritmo di ricerca binaria funziona SOLO se il vettore è ORDINATO.
  // Se l’utente inserisce numeri non ordinati, il risultato sarà errato.

  // ===============================================================
  // LETTURA DELLA CHIAVE DA CERCARE
  // ===============================================================
  printf("\nInput key da cercare: ");
  scanf("%d", &k);

  // ===============================================================
  // CHIAMATA ALLA FUNZIONE BinSearch()
  // ===============================================================
  // Ricerca la chiave k nel vettore v tra gli indici [0, V-1]
  index = BinSearch(v, 0, i - 1, k);

  // ===============================================================
  // STAMPA DELL’ESITO DELLA RICERCA
  // ===============================================================
  if (index != -1)
    printf("Chiave trovata all'indice: %d\n", index);
  else
    printf("Chiave non trovata\n");

  return 0;
}

// ===================================================================
// FUNZIONE BinSearch()
// ===================================================================
//
// IDEA DI BASE:
//   Dato un array ORDINATO v[l..r] e una chiave k:
//
//   1️⃣ Si calcola l’indice centrale m = (l + r) / 2
//   2️⃣ Se v[m] == k → trovato! restituisco m
//   3️⃣ Se k < v[m]  → cerco nella metà SINISTRA (intervallo [l, m-1])
//   4️⃣ Se k > v[m]  → cerco nella metà DESTRA (intervallo [m+1, r])
//
// CASO BASE:
//   Quando l > r → l’intervallo è vuoto, quindi la chiave non esiste.
//
// COMPLESSITÀ:
//   - Tempo: O(log n)
//   - Spazio: O(log n) (per via della ricorsione)
// ===================================================================
int BinSearch(int v[], int l, int r, int k) {
  int m;

  // Caso base: intervallo vuoto → chiave non trovata
  if (l > r)
    return -1;

  // Calcolo dell’indice centrale
  m = (l + r) / 2;

  // Confronto con l’elemento centrale
  if (k == v[m])
    return m;  // Caso 1: chiave trovata
  else if (k < v[m])
    return BinSearch(v, l, m - 1, k);  // Caso 2: cerca nella parte sinistra
  else
    return BinSearch(v, m + 1, r, k);  // Caso 3: cerca nella parte destra
}

/* ===================================================================
 * ESEMPIO DI ESECUZIONE
 * ===================================================================
 * Input:
 *   v = [2, 5, 8, 10, 13, 18, 25, 30]
 *   key = 13
 *
 * Passaggi ricorsivi:
 *   1. l=0, r=7 → m=(0+7)/2=3 → v[3]=10 → 13 > 10 → cerco a destra
 *   2. l=4, r=7 → m=(4+7)/2=5 → v[5]=18 → 13 < 18 → cerco a sinistra
 *   3. l=4, r=4 → m=(4+4)/2=4 → v[4]=13 → trovato!
 *
 * Output:
 *   Chiave trovata all'indice: 4
 * ===================================================================
 */
