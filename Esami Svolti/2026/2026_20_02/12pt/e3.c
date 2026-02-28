#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cntNum(int base, int nd, int maxRip);

int main (void) {
  int base=10;
  int maxRip=3;
  int nDigits=9;

  int res = cntNum(base, nDigits, maxRip);

  printf("counter of solutions: %d\n", res);
  return 0;
}

/*
  la soluzione e' una variante del modello delle disposizioni con ripetizioni,
  in cui il numero di ripetizioni e' limitato a maxRip. SI usa a tale scopo
  il vettore mark. Il vettore val non viene usato in quanto le cifre numeriche
  conciderebbero con gli indici.
  Si fa pruning sulla differenza tra cifre adiacenti. Il conteggio delle cifre distinte
  viene aggiornato ad ogni chiamata ricorsiva e verificato nel caso terminale.
*/  
int disp_rip(int pos, int *sol,int *mark, int n, int k, int m, int cntDist){
  int i, cnt=0;
  if (pos >= k)
    return (cntDist>k/2)? 1 : 0;

  for (i=0; i<n; i++){
    if ((mark[i] < m) &&
        (pos==0 || abs(i-sol[pos-1])<3)) { // pruning
      if (mark[i]++ == 0) cntDist++; // conta cifre distinte
      sol[pos] = i;
      cnt += disp_rip(pos+1, sol, mark, n, k, m, cntDist);
      if (--mark[i]==0) cntDist--;
    }
  }
  return cnt;
}

int cntNum(int base, int nDigits, int maxRip) {
  int *sol=malloc(nDigits*sizeof(int));
  int *mark=calloc(base,sizeof(int));
  int nSol = disp_rip(0,sol,mark,base,nDigits,maxRip,0);
  free(sol);
  free(mark);
  return nSol;
}
