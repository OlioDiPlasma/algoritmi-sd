#include <stdio.h>

#define N 8               // dimensione della scacchiera (8x8)
#define ND (2*N-1)        // numero massimo di diagonali

// Array globali
int riga[N];              // riga[i] = colonna dove metto la regina nella riga i
int mark[N];              // mark[c] = 1 se la colonna c è già occupata
int diag[ND];             // diag[i+j] = 1 se la diagonale principale è occupata
int antidiag[ND];         // antidiag[i-j+(N-1)] = 1 se la diagonale opposta è occupata
int num_sol = 0;          // contatore delle soluzioni trovate

// Prototipi
void perm_sempl(int q);
void stampa(void);

/* =============================================================
 * MAIN
 * ============================================================= */
int main() {
  int i;

  // Inizializzazione delle strutture
  for (i=0; i<N; i++) {
    riga[i] = -1;   // nessuna regina ancora piazzata
    mark[i] = 0;    // nessuna colonna occupata
  }

  for (i=0; i<ND; i++) {
    diag[i] = 0;        // nessuna diagonale principale occupata
    antidiag[i] = 0;    // nessuna diagonale secondaria occupata
  }

  // Avvia la ricerca ricorsiva
  perm_sempl(0);

  printf("\nNumero di soluzioni trovate: %d\n", num_sol);
  return 0;
}

/* =============================================================
 * perm_sempl(q)
 * -------------------------------------------------------------
 * Funzione ricorsiva che cerca di posizionare una regina nella
 * riga 'q'. Se trova una posizione valida, passa alla riga successiva.
 *
 * q = indice della riga corrente
 * ============================================================= */
void perm_sempl(int q) {
  int c;

  // Caso base: tutte le righe riempite → soluzione completa
  if (q >= N) {
    num_sol++;
    stampa();
    return;
  }

  // Prova a mettere una regina in ogni colonna possibile
  for (c = 0; c < N; c++)
    if (
        mark[c] == 0 &&                  // colonna libera
        diag[q + c] == 0 &&              // diagonale principale libera
        antidiag[q - c + (N - 1)] == 0   // diagonale secondaria libera
       ) {

      // --- SCELTA ---
      mark[c] = 1;
      diag[q + c] = 1;
      antidiag[q - c + (N - 1)] = 1;
      riga[q] = c;                       // metto la regina nella colonna c

      // --- CHIAMATA RICORSIVA ---
      perm_sempl(q + 1);                 // passa alla prossima riga

      // --- BACKTRACKING ---
      mark[c] = 0;
      diag[q + c] = 0;
      antidiag[q - c + (N - 1)] = 0;
      // riga[q] non serve azzerarla: verrà riscritta
    }

  return;
}

/* =============================================================
 * stampa()
 * -------------------------------------------------------------
 * Visualizza una soluzione della scacchiera:
 * - un numero indica la colonna della regina in quella riga
 * - '*' indica una cella vuota
 * ============================================================= */
void stampa() {
  int r, c;
  printf("\nRisultato\n");

  for (r = 0; r < N; r++) {
    for (c = 0; c < N; c++)
      if (c == riga[r])
        printf("%d", riga[r]);  // posizione della regina
      else
        printf("*");            // cella vuota
    printf("\n");
  }
}
