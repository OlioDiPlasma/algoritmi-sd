#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

/* ============================================================
 * Labirinto (maze solver) con backtracking
 * ------------------------------------------------------------
 * Dato un file che rappresenta un labirinto (matrice di caratteri),
 * il programma cerca un percorso dall’ingresso (in) all’uscita (out).
 *
 * Caratteri:
 *   X → muro
 *   . → cella libera
 *
 * L’algoritmo esplora tutte le direzioni ricorsivamente.
 * Quando trova la soluzione, termina.
 * Durante l’esplorazione, scrive ogni passo su "maze.out".
 * ============================================================ */

/* Dimensioni massime */
#define NR 100
#define NC 100
#define MAXSTR 100

/* File di output per la traccia delle mosse */
FILE *out = NULL;

/* Struttura per rappresentare un punto nella matrice */
typedef struct {
  int r, c;   // riga e colonna
} punto_t;

/* Matrice del labirinto e dimensioni effettive */
char L[NR][NC];
int nr, nc;

/* Prototipi */
punto_t sposta(punto_t p, int i);
void stampa(int m);
int mossa(punto_t p, punto_t o, int l);

/* ============================================================
 * MAIN
 * ============================================================ */
int main(int argc, char *argv[]) {
  FILE *fp;
  out = fopen("maze.out", "w");  // file dove scrivere i passaggi
  char riga[MAXSTR];
  int r, c;
  punto_t in, outp, p;

  /* Controllo numero argomenti */
  if (argc < 6) {
    printf("Numero di argomenti insufficiente!\n");
    printf("Formato corretto:\n");
    printf("%s <nome file> <r_in> <c_in> <r_out> <c_out>\n", argv[0]);
    return 1;
  }

  /* Apertura file del labirinto */
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    printf("Errore: impossibile aprire il file %s\n", argv[1]);
    return 1;
  }

  /* Lettura dimensioni (prima riga del file) */
  fgets(riga, MAXSTR, fp);
  sscanf(riga, "%d%d", &nr, &nc);

  /* Lettura del labirinto riga per riga */
  for (r = 0; r < nr; r++) {
    fgets(riga, MAXSTR, fp);
    for (c = 0; c < nc; c++)
      L[r][c] = riga[c];
  }

  /* Lettura coordinate ingresso e uscita dai parametri */
  in.r = atoi(argv[2]);
  in.c = atoi(argv[3]);
  outp.r = atoi(argv[4]);
  outp.c = atoi(argv[5]);

  /* Segna l’inizio e l’uscita */
  L[in.r][in.c] = '0';   // punto di partenza
  L[outp.r][outp.c] = '#'; // punto di arrivo

  /* Stampa iniziale del labirinto */
  stampa(0);

  /* Avvio della ricerca ricorsiva */
  p = in;
  if (mossa(p, outp, 0))
    printf("Soluzione trovata ✅\n");
  else
    printf("Soluzione NON trovata ❌\n");

  fclose(out);
  fclose(fp);
  return 0;
}

/* ============================================================
 * FUNZIONE mossa()
 * ------------------------------------------------------------
 * Esplora ricorsivamente le 4 direzioni possibili (su, giù, sx, dx)
 * a partire da un punto p, cercando di raggiungere il punto o (uscita).
 *
 * Parametri:
 *   p  → posizione corrente
 *   o  → uscita (obiettivo)
 *   l  → livello di profondità (passo corrente)
 *
 * Ritorna:
 *   1 se trova una soluzione
 *   0 se non trova alcun percorso
 * ============================================================ */
int mossa(punto_t p, punto_t o, int l) {
  int i, r;
  punto_t p1;
  l++; // aumenta livello di profondità

  // ----- CASO BASE -----
  if (p.r == o.r && p.c == o.c) {
    // raggiunta l’uscita
    return 1;
  }

  // ----- CASO RICORSIVO -----
  for (i = r = 0; i < 4; i++) {
    p1 = sposta(p, i);  // prova una delle 4 direzioni

    // se p1 è diverso da p → mossa valida
    if (p1.r != p.r || p1.c != p.c) {
      // segna il passo con il numero del livello (mod 10)
      L[p1.r][p1.c] = (char) ('0' + l % 10);
      stampa(l); // scrive su maze.out lo stato del labirinto

      r = mossa(p1, o, l); // chiamata ricorsiva
      if (r > 0) {
        // se ha trovato una soluzione → interrompi la ricerca
        break;
      } else {
        // BACKTRACK: torna indietro e libera la cella
        L[p1.r][p1.c] = '.';
      }
    }
  }

  // Scrive nel file un’indicazione di ritorno
  for (i = 0; i < l - 1; i++)
    fprintf(out, " ");
  fprintf(out, "R(%d)\n\n", l - 1);

  return r;
}

/* ============================================================
 * FUNZIONE sposta()
 * ------------------------------------------------------------
 * Tenta di muovere il punto p in una delle 4 direzioni:
 *   0 = sinistra, 1 = su, 2 = destra, 3 = giù
 * Se la mossa è valida (cella libera e dentro i limiti),
 * ritorna le nuove coordinate, altrimenti lascia p invariato.
 * ============================================================ */
punto_t sposta(punto_t p, int i) {
  int r, c;
  // incrementi riga/colonna per le 4 direzioni
  int spr[4] = { 0, -1, 0, 1 };
  int spc[4] = { -1, 0, 1, 0 };

  r = p.r + spr[i];
  c = p.c + spc[i];

  // controlla che sia dentro la matrice
  if (r >= 0 && c >= 0 && r < nr && c < nc)
    // la cella non deve essere muro ('X') né già visitata (numero)
    if (toupper(L[r][c]) != 'X' && !isdigit(L[r][c])) {
      p.r = r;
      p.c = c;
    }

  return p;
}

/* ============================================================
 * FUNZIONE stampa()
 * ------------------------------------------------------------
 * Stampa su file "maze.out" lo stato attuale del labirinto.
 * Ogni livello di profondità viene indentato di m spazi
 * per rendere visiva la struttura dell’esplorazione ricorsiva.
 * ============================================================ */
void stampa(int m) {
  int r, c;

  for (r = 0; r < nr; r++) {
    for (c = 0; c < m; c++)  // indentazione
      fprintf(out, " ");
    for (c = 0; c < nc; c++) // stampa una riga del labirinto
      fprintf(out, "%c", L[r][c]);
    fprintf(out, "\n");
  }
  fprintf(out, "\n");
}
