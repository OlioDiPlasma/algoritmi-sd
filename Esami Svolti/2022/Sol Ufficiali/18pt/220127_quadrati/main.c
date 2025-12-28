#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<stdio.h>
#include<ctype.h>

#define DBG 0

#define file_griglia "griglia.txt"

/*** VERS 1 ***/
#define file_prop "proposta_ok.txt"
// #define file_prop "proposta_ko_overlap.txt"
// #define file_prop "proposta_ko_bianco.txt"
// #define file_prop "proposta_ko_outofbound.txt"

/*** VERS 2 ***/
// #define file_prop_v2 "proposta_v2_ok.txt"
// #define file_prop_v2 "proposta_v2_ko_overlap.txt"
#define file_prop_v2 "proposta_v2_ko_quadrato.txt"

#define BLACK_C '#' // Cella nera (per stampa)
#define BLACK_N -1 // Cella nera (per lettura)
#define WHITE 0 // Cella bianca

/***

  UTIL VARIE

***/

int **alloca2d(int R, int C) {
  int i, **m = calloc(R, sizeof(int*));
  for(i=0;i<R;i++)
    m[i] = calloc(C, sizeof(int));
  return m;
}

void copia2d(int R, int C, int **src, int **dst) {
  int i, j;
  for(i=0;i<R;i++) {
    for(j=0;j<C;j++)
      dst[i][j] = src[i][j];
  }
}

void stampa2d(int **m, int R, int C) {
  // Nella stampa della griglia, le celle nere le stampiamo come # per distinguerle dalle aree quadrate
  int i, j;
  for(i=0;i<R;i++) {
    for(j=0;j<C;j++) {
      if (m[i][j] == -1) printf("%c\t", BLACK_C);
      else {
        printf("%d\t", m[i][j]);
      }
    }
    printf("\n");
  }
}

void libera2d(int **m, int R) {
  if (!m) return;
  int i;
  for(i=0;i<R;i++) {
    if (m[i]) free(m[i]);
  }
  free(m);
}

void reset2d(int **m, int R, int C) {
  int i, j;
  for(i=0;i<R;i++)
    for(j=0;j<C;j++)
      if (m[i][j] < 0) m[i][j] *= -1;
}

/***

  INPUT GRIGLIA

***/

int** leggiGriglia(char *fn, int *R, int *C) {
  // Nella lettura della griglia, convertiamo le celle nere in -1 per comodità
  FILE *in = fopen(fn, "r");
  int i, j, **griglia;
  fscanf(in, "%d %d\n", R, C);
  griglia = alloca2d(*R, *C);
  for(i=0;i<*R;i++) {
    for(j=0;j<*C;j++) {
      fscanf(in, "%d", &griglia[i][j]);
      if (griglia[i][j]) griglia[i][j] = BLACK_N;
    }
  }
  fclose(in);
  return griglia;
}

/***

  GESTIONE GRIGLIA

***/

int fill(int **m, int R, int C, int r, int c, int l, int val) {
  int i, j, cnt = 0;
  for(i=r;i<r+l && i<R;i++) {
    for(j=c;j<c+l && j<C;j++) {
      if (m[i][j] == WHITE) {
        m[i][j] = val;
        cnt++;
      } else {
        return 0;
      }
    }
  }
  // Se il fill finisse out-of-bound (es: la regione quadrata non è interamente contenibile nella mappa), dichiara comunque fallimento
  // In sostanza verifica di aver davvero colorato un quadrato di lato pari al valore specificato
  if (cnt != l*l)
      return 0;
  return 1;
}

void undo(int **src, int **dest, int R, int C, int r, int c, int l) {
  // Sfrutta la griglia originale per fare reset dello stato corrente, senza doversi preoccupare di "decolorare" la regione
  int i, j;
  for(i=r;i<r+l && i<R;i++) {
    for(j=c;j<c+l && j<C;j++) {
      dest[i][j] = src[i][j];
    }
  }
}

/***

  VERIFICA - VERS. 1
  Le regioni sono memorizzate come terna <r> <c> <l> a rappresentare l'angolo top-left della regione e la relativa dimensione

***/

void verify(char *fn, int **griglia, int R, int C) {
  int i, j, reg, r, c, l, nr, ok = 1, **proposta;
  FILE *in = fopen(fn, "r");

  proposta = alloca2d(R, C);
  copia2d(R, C, griglia, proposta);

  fscanf(in, "%d\n", &nr);
  for(reg=1;reg<=nr;reg++) {
    // Legge ogni i-esima regione come terna <r> <c> <l>
    fscanf(in, "%d %d %d\n", &r, &c, &l);
    // Usa la stessa funzione fill usata anche nella funzione ricorsiva per riempire una certa regione. In caso di fallimento, fill ritorna 0
    // fill fallisce se la regione si sovrappone a celle nere o ad altre zone già colorate
    // fill fallsice anche nel caso in cui non sia possibile colorare l*l celle, ad esempio perché siamo ai bordi
    ok *= fill(proposta, R, C, r, c, l, reg);
    if (ok != 1)
      break;
  }

  if (ok) {
    // Assicurati che non siano rimaste zone "bianche"
    for(i=0;i<R && ok;i++) {
      for(j=0;j<C && ok;j++)
        if(proposta[i][j] == WHITE) {
          ok = 0;
        }
    }
  }

  printf("Proposta%svalida [%d regioni]\n", ok ? " " : " NON ", ok ? nr : -1);
  stampa2d(proposta, R, C);
  libera2d(proposta, R);
}

/***

  VERIFICA - VERS. 2
  La griglia completata è letta esplicitamente come matrice.
  Questo formato di input è decisamente meno conveniente dato che richiede di fare tutta una serie di controlli extra per garantire
  la correttezza della soluzione, nello specifico il fatto che le regioni siano davvero quadrate.
  ASSUNZIONE: per le celle nere appare -1 nel file. Eventuali celle bianche valgono 0. Le regioni ammettono un qualsiasi indice >= 1

***/

void verify_v2(char *fn, int **griglia, int R, int C) {
  int i, j, nr = 0, r, c, h, w, ok = 1, cnt = 0, **proposta;
  FILE *in = fopen(fn, "r");

  proposta = alloca2d(R, C);
  copia2d(R, C, griglia, proposta);

  fscanf(in, "%d %d\n", &r, &c);

  // La mappa letta non è compatibile con la griglia originale
  if (r != R || c != C)
    ok = 0;

  if (ok) {
    for(i=0;i<R && ok;i++) {
      for(j=0;j<C && ok;j++) {
        fscanf(in, "%d", &proposta[i][j]);
        // La proposta non rispetta la griglia originale (celle nere)
        if(griglia[i][j] == BLACK_N && proposta[i][j] != griglia[i][j])
          ok = 0;
        // La proposta non copre completamente la regione bianca
        if (proposta[i][j] == WHITE)
          ok = 0;
      }
    }
  }

  int **visited = alloca2d(R, C);
  if (ok) {
    // Le regioni devono essere quadrate
    for(i=0;i<R && ok;i++) {
      for(j=0;j<C && ok;j++) {
        if(proposta[i][j] == BLACK_N)
          continue;
        if(visited[i][j] == 1)
          continue;
        int curr = proposta[i][j];
        cnt = 0;
        for(r=i, h=0;r<R && proposta[r][j] == curr;r++) h++;
        for(c=j, w=0;j<C && proposta[i][c] == curr;c++) w++;
        if (h != w) {
          // Già ai bordi superiore e sinistro il quadrato fallisce
          ok = 0;
        }
        // Controlla se l'intera regione quadrata è libera
        for(r=i;r<i+h && ok;r++) {
          for(c=j;c<j+w && ok;c++) {
            if (proposta[r][c] == BLACK_N || proposta[r][c] != curr || visited[r][c])
              // Incontra una cella nera o una regione diversa da quella attuale o una porzione di questa regione si sovrappone con sezioni già visitate
              ok = 0;
            visited[r][c] = 1;
            cnt++;
          }
        }

        if (cnt != h*w) ok = 0;

        // Controlla che non ci siano celle con lo stesso valore di questa regione direttamente adiacenti ma fuori dall'area attesa
        // Eventuali regioni con lo stesso numero ma staccate sono fatte "passare" se valide (poiché basterebbe cambiare l'id della regione)
        for(r=i-1, c=j-1;r<R && r<i+h+1 && ok;r++) {
          if (r >= 0 && c >= 0)
            if (proposta[r][c] == curr)
              ok = 0;
        }
        for(r=i-1, c=j+w;r<R && r<i+h+1 && ok;r++) {
          if (r >= 0 && c < C)
            if (proposta[r][c] == curr)
              ok = 0;
        }
        for(r=i-1, c=j-1;c<C && c<j+w+1 && ok;c++) {
          if (r >= 0 && c >= 0)
            if (proposta[r][c] == curr)
              ok = 0;
        }
        for(r=i+h, c=j-1;c<C && c<j+w+1 && ok;c++) {
          if (r < R && c >= 0)
            if (proposta[r][c] == curr)
              ok = 0;
        }

        nr++;
      }
    }
  }

  printf("Proposta%svalida [%d regioni]\n", ok ? " " : " NON ", ok ? nr : -1);
  stampa2d(proposta, R, C);
  libera2d(proposta, R);
  libera2d(visited, R);
}


/***

  GENERAZIONE SOLUZIONE

***/

int findMinHW(int **m, int R, int C, int r, int c) {
  // Trova la dimensione massima per la regione quadrata iniziate in una certa posizione [r,c] valutando il minimo tra altezza e ampiezza potenziali
  int i, h=0, w=0;
  for(i=r;i<R && m[i][c] == WHITE;i++) h++;
  for(i=c;i<C && m[r][i] == WHITE;i++) w++;
  return h < w ? h : w;
}

void solve_r(int **m, int R, int C, int pos, int **sol, int curr_val, int **best, int *best_val) {
  int r, c, ok, l, L;
  if (pos >= R*C) {
    // SOL trovata
    if (*best_val > curr_val) {
      #if DBG
      printf("Migliora %d > %d\n", *best_val, curr_val);
      stampa2d(sol, R, C);
      #endif
      *best_val = curr_val;
      copia2d(R, C, sol, best);
    }
    return;
  }

  if (curr_val > *best_val) return;

  r = pos / C;
  c = pos % C;

  /*
   * Se la cella è libera, prova a "posizionare" una regione quadrata con il prossimo progressivo libero disponibile, per tutte le dimensioni ammissibili in questo punto
   * NOTA BENE:
   *    Il fill potrebbe fallire se ci fossero delle regioni nere dentro all'area (ma non incontrate sulla prima riga/colonna usate per stimare H/W).
   *    Lasciamo che la funzione fill ce lo dica e facciamo fallire il riempimento. Nel qual caso passiamo direttamente al backtrack e alla diminuzione del valore tentato per l.
   * Altrimenti, se c'è già un numero nello schema di partenza, salta la cella.
   */
  if (sol[r][c] == WHITE) {
    L = findMinHW(sol, R, C, r, c);
    curr_val += 1;
    for(l=L;l>0;l--)  {
      // printf("Regione quadrata %d x %d di indice %d in posizione %d %d\n", l, l, curr_val, r, c);
      ok = fill(sol, R, C, r, c, l, curr_val);
      if(ok) solve_r(m, R, C, pos+l, sol, curr_val, best, best_val);
      undo(m, sol, R, C, r, c, l);
    }
    curr_val -= 1;
  } else {
    solve_r(m, R, C, pos+1, sol, curr_val, best, best_val);
  }
}

// Fa una scansione per righe della matrice e posizione per posizione piazza un singolo numero.
void solve(int **m, int R, int C) {
  int **sol = alloca2d(R, C), **best = alloca2d(R, C);
  int best_val = INT_MAX;
  copia2d(R, C, m, sol);

  solve_r(m, R, C, 0, sol, 0, best, &best_val);

  printf("Soluzione migliore usa %d regioni\n", best_val);
  stampa2d(best, R, C);

  libera2d(sol, R);
  libera2d(best, R);
}

/***

  MAIN

***/

int main(int argc, char **argv) {
  int R, C, **griglia;

  griglia = leggiGriglia(file_griglia, &R, &C);

  #if DBG
  printf("*** Schema Iniziale ***\n");
  stampa2d(griglia, R, C);
  #endif

  printf("*** FASE 1 - Verifica Vers. 1 ***\n");
  verify(file_prop, griglia, R, C);

  printf("*** FASE 1 - Verifica Vers. 2 ***\n");
  verify_v2(file_prop_v2, griglia, R, C);

  printf("*** FASE 2 - Generazione ***\n");
  solve(griglia, R, C);

  printf("*** Terminazione ***\n");
  libera2d(griglia, R);

  return 0;
}
