#include <stdio.h>
#include <stdlib.h>

/* ============================================================
 * PROTOTIPI DELLE FUNZIONI
 * ============================================================ */
void Merge(int *A, int *B, int l, int q, int r);
void MergeSort(int *A, int *B, int l, int r);
int promising(int *val, int i, int part_sum, int rem_sum, int X);
void sumset(int pos, int *val, int *sol, int part_sum, int rem_sum, int X);

/* ============================================================
 * SCOPO DEL PROGRAMMA:
 * --------------------
 * Dato un insieme di numeri positivi val[0..k-1] e un valore X,
 * trovare TUTTI i sottoinsiemi i cui elementi sommano esattamente a X.
 *
 * Usa una versione ottimizzata del powerset con:
 *   - Ordinamento iniziale dei numeri (MergeSort)
 *   - Potatura tramite funzione "promising"
 *
 * ESEMPIO:
 *   val = {5, 10, 12, 13, 15, 18}, X = 30
 *   → Sottoinsiemi validi:
 *      {12, 18}
 *      {5, 10, 15}
 * ============================================================ */
int main(void) {
  int i, k, rem_sum = 0;
  int *sol, *val, *tmp, X;

  // Input: numero di elementi
  printf("Inserisci il numero di elementi:  ");
  scanf("%d", &k);

  // Allocazione dinamica
  val = malloc(k * sizeof(int));   // vettore dei valori
  tmp = malloc(k * sizeof(int));   // array di appoggio per MergeSort
  sol = malloc(k * sizeof(int));   // vettore delle scelte (0/1)

  // Inizializza sol[] a 0
  for (i = 0; i < k; i++)
      sol[i] = 0;

  // Lettura dei valori
  printf("Inserisci gli elementi: \n");
  for (i = 0; i < k; i++) {
    printf("val[%d] = ", i);
    scanf("%d", &val[i]);
    rem_sum += val[i];   // Calcola somma totale (serve per la potatura)
  }

  // Input del valore target
  printf("Inserisci la somma X: ");
  scanf("%d", &X);

  // Ordina i valori (perché il pruning funziona meglio se sono ordinati)
  MergeSort(val, tmp, 0, k - 1);

  // Se la somma totale è minore di X → nessuna soluzione possibile
  if (rem_sum < X)
    printf("Nessuna soluzione possibile.\n");
  else {
    printf("Le soluzioni sono:\n");
    sumset(0, val, sol, 0, rem_sum, X);
  }

  // Libera la memoria
  free(val);
  free(tmp);
  free(sol);
  return 0;
}

/* ============================================================
 * FUNZIONE promising()
 * ------------------------------------------------------------
 * Serve per la **potatura (branch and bound)**.
 *
 * Dice se vale la pena continuare a esplorare la soluzione parziale
 * corrente o se possiamo già scartarla.
 *
 * Ritorna true (1) se:
 *   - anche sommando tutti i numeri rimanenti si può ancora arrivare a X
 *     → part_sum + rem_sum >= X
 *   - e se aggiungendo il prossimo elemento non si supera X
 *     → part_sum + val[pos] <= X
 * ============================================================ */
int promising(int *val, int pos, int part_sum, int rem_sum, int X) {
  return (part_sum + rem_sum >= X) && (part_sum + val[pos] <= X);
}

/* ============================================================
 * FUNZIONE sumset()
 * ------------------------------------------------------------
 * Ricorsione che genera tutti i sottoinsiemi di val[]
 * e stampa quelli la cui somma è esattamente X.
 *
 * parametri:
 *   pos       → indice dell’elemento corrente
 *   val[]     → vettore dei numeri
 *   sol[]     → vettore binario (1 = incluso, 0 = escluso)
 *   part_sum  → somma parziale dei numeri inclusi
 *   rem_sum   → somma dei numeri rimanenti
 *   X         → somma target
 * ============================================================ */
void sumset(int pos, int *val, int *sol, int part_sum, int rem_sum, int X) {
  int j;

  // ----- CASO BASE: trovato un sottoinsieme valido -----
  if (part_sum == X) {
    printf("\n{\t");
    for (j = 0; j < pos; j++)
      if (sol[j])         // stampa solo gli elementi inclusi
        printf("%d\t", val[j]);
    printf("}\n");
    return;
  }

  // ----- CASO RICORSIVO CON POTATURA -----
  if (promising(val, pos, part_sum, rem_sum, X)) {
    // Scelta 1: includere val[pos]
    sol[pos] = 1;
    sumset(pos + 1, val, sol, part_sum + val[pos], rem_sum - val[pos], X);

    // Scelta 2: escludere val[pos]
    sol[pos] = 0;
    sumset(pos + 1, val, sol, part_sum, rem_sum - val[pos], X);
  }
}

/* ============================================================
 * FUNZIONI DI ORDINAMENTO (MergeSort)
 * ------------------------------------------------------------
 * Serve per ordinare il vettore val[], così che la funzione
 * "promising" possa lavorare in modo più efficace (i valori
 * sono in ordine crescente → pruning più forte).
 * ============================================================ */
void MergeSort(int *A, int *B, int l, int r) {
  int q;
  if (r <= l) return;
  q = (l + r) / 2;
  MergeSort(A, B, l, q);
  MergeSort(A, B, q + 1, r);
  Merge(A, B, l, q, r);
}

void Merge(int *A, int *B, int l, int q, int r) {
  int i = l, j = q + 1, k = 0;

  while (i <= q && j <= r) {
    if (A[i] < A[j]) B[k++] = A[i++];
    else B[k++] = A[j++];
  }

  while (i <= q)
    B[k++] = A[i++];

  while (j <= r)
    B[k++] = A[j++];

  for (i = l, j = 0; i <= r; i++, j++)
    A[i] = B[j];
}
