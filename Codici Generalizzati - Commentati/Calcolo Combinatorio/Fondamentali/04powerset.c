#include <stdio.h>
#include <stdlib.h>

// Dichiarazioni delle funzioni per calcolare il powerset con 3 metodi diversi
int powerset_disp_rip(int pos, int *val, int *sol, int n, int count);
int powerset_div_conq(int pos, int *val, int *sol, int n, int start, int count);
int powerset_comb_semp(int* val, int n, int* sol);
int powerset_r(int* val, int k, int* sol, int n, int pos, int start);

int main(void) {
  int i, n, cnt, *val, *sol;

  // Legge il numero di elementi dell'insieme
  printf("Input number of elements: ");
  scanf("%d", &n);

  val = malloc(n*sizeof(int));      // Array per gli elementi
  sol = calloc(n,sizeof(int));      // Array soluzione (inizializzato a 0)

  // Legge gli elementi dell'insieme
  printf("Input elements: \n");
  for (i=0; i<n; i++) {
    printf("val[%d] = ", i);
    scanf("%d", &val[i]);
  }

  // Stampa l'insieme di partenza
  printf("The powerset of set {");
  for(i =0; i<n; i++)
    printf(" %d ", val[i]);
  printf(" } is \n");

  // METODO 1: Disposizioni con ripetizioni (0/1 per ogni elemento)
  printf("arrangements with repetitions\n");
  cnt = powerset_disp_rip(0, val, sol, n, 0);
  printf ("number of sets: %d \n", cnt);

  // METODO 2: Divide et impera
  printf("divide and conquer\n");
  cnt = powerset_div_conq(0, val, sol, n, 0, 0);
  printf ("number of sets: %d \n", cnt);

  // METODO 3: Combinazioni semplici (genera tutti i sottoinsiemi per cardinalità)
  printf("simple combinations\n");
  cnt = powerset_comb_semp(val, n, sol);
  printf ("number of sets: %d \n", cnt);

  free(val);
  free(sol);

  return 0;
}

/* ===================================================================
 * METODO 1: POWERSET TRAMITE DISPOSIZIONI CON RIPETIZIONE
 * ===================================================================
 * Idea: Per ogni elemento decidi se includerlo (1) o escluderlo (0)
 * 
 * Esempio con {1, 2, 3}:
 * sol = [0,0,0] → { }          (nessun elemento)
 * sol = [0,0,1] → { 3 }        (solo il terzo elemento)
 * sol = [0,1,0] → { 2 }        (solo il secondo)
 * sol = [0,1,1] → { 2, 3 }     (secondo e terzo)
 * sol = [1,0,0] → { 1 }        (solo il primo)
 * sol = [1,0,1] → { 1, 3 }     (primo e terzo)
 * sol = [1,1,0] → { 1, 2 }     (primo e secondo)
 * sol = [1,1,1] → { 1, 2, 3 }  (tutti)
 * 
 * Totale: 2^n sottoinsiemi (per n=3: 2^3 = 8)
 */
int powerset_disp_rip(int pos, int *val, int *sol, int n, int count) {
  int i;

  // CASO BASE: abbiamo deciso per tutti gli n elementi
  if (pos >= n) {
    printf("{ ");
    // Stampa solo gli elementi per cui sol[i] = 1
    for (i=0; i<n; i++)
      if (sol[i]!=0)  // Se l'elemento i è stato "scelto"
        printf("%d ", val[i]);
    printf("} \n");
    return count+1;
  }

  // CASO RICORSIVO: per l'elemento in posizione 'pos' ci sono 2 scelte
  
  // Scelta 1: NON includere l'elemento (sol[pos] = 0)
  sol[pos] = 0;
  count = powerset_disp_rip(pos+1, val, sol, n, count);
  
  // Scelta 2: INCLUDERE l'elemento (sol[pos] = 1)
  sol[pos] = 1;
  count = powerset_disp_rip(pos+1, val, sol, n, count);
  
  return count;
}

/* ALBERO DELLE DECISIONI per {1, 2, 3}:
 *
 *                        pos=0
 *                    /           \
 *              sol[0]=0        sol[0]=1
 *              (escludi 1)     (includi 1)
 *                /                  \
 *            pos=1                  pos=1
 *           /     \                /     \
 *      sol[1]=0  sol[1]=1    sol[1]=0  sol[1]=1
 *         |         |            |         |
 *      pos=2     pos=2        pos=2     pos=2
 *      /   \     /   \        /   \     /   \
 *   0/1   0/1  0/1  0/1     0/1  0/1  0/1  0/1
 *    
 * Ogni ramo dell'albero genera un sottoinsieme diverso
 * Totale foglie: 2^3 = 8 sottoinsiemi
 */

/* ===================================================================
 * METODO 2: POWERSET CON DIVIDE ET IMPERA
 * ===================================================================
 * Idea: Per ogni elemento, genera ricorsivamente:
 *   1. Tutti i sottoinsiemi che lo CONTENGONO
 *   2. Tutti i sottoinsiemi che NON lo contengono (saltandolo)
 * 
 * È una variante del metodo delle combinazioni ma con un approccio
 * divide-and-conquer più esplicito
 */
int powerset_div_conq(int pos, int *val, int *sol, int n, int start, int count) {
   int i;
   
   // CASO BASE: abbiamo considerato tutti gli elementi
   if (start >= n) {
      printf("{ ");
      // Stampa gli elementi scelti (da 0 a pos-1)
      for (i = 0; i < pos; i++)
         printf("%d ", sol[i]);
      printf("} \n");
      return count+1;
   }
   
   // PARTE 1: Genera sottoinsiemi CHE CONTENGONO elementi da 'start' in poi
   for (i = start; i < n; i++) {
      sol[pos] = val[i];  // Scegli l'elemento i
      // Ricorsione: continua con elementi successivi (i+1)
      count = powerset_div_conq(pos+1, val, sol, n, i+1, count);
   }
   
   // PARTE 2: Genera il sottoinsieme vuoto (salta tutti gli elementi rimanenti)
   // Questo viene fatto passando start=n, che attiva il caso base
   count = powerset_div_conq(pos, val, sol, n, n, count);
   
   return count;
}

/* ESEMPIO DI ESECUZIONE con {1, 2, 3}:
 * 
 * powerset_div_conq(pos=0, start=0)
 *   ├─ for i=0..2: prova a includere val[i]
 *   │   ├─ sol[0]=1 → powerset_div_conq(pos=1, start=1)
 *   │   │   ├─ sol[1]=2 → powerset_div_conq(pos=2, start=2)
 *   │   │   │   ├─ sol[2]=3 → stampa {1,2,3}
 *   │   │   │   └─ start=n → stampa {1,2}
 *   │   │   ├─ sol[1]=3 → stampa {1,3}
 *   │   │   └─ start=n → stampa {1}
 *   │   ├─ sol[0]=2 → genera sottoinsiemi con 2
 *   │   └─ sol[0]=3 → genera sottoinsiemi con 3
 *   └─ start=n → stampa { } (insieme vuoto)
 */

/* ===================================================================
 * METODO 3: POWERSET TRAMITE COMBINAZIONI SEMPLICI
 * ===================================================================
 * Idea: Genera tutti i sottoinsiemi organizzati per cardinalità:
 *   - Sottoinsiemi di dimensione 0: { }
 *   - Sottoinsiemi di dimensione 1: {1}, {2}, {3}
 *   - Sottoinsiemi di dimensione 2: {1,2}, {1,3}, {2,3}
 *   - Sottoinsiemi di dimensione 3: {1,2,3}
 * 
 * Usa le combinazioni semplici C(n,k) per ogni k da 0 a n
 */
int powerset_comb_semp(int* val, int n, int* sol){
   int count = 0, i;
   
   // Stampa l'insieme vuoto (cardinalità 0)
   printf("{ }\n");
   count++;
   
   // Per ogni possibile cardinalità da 1 a n
   for(i = 1; i <= n; i++){
      // Genera tutte le combinazioni di i elementi
      count += powerset_r(val, n, sol, i, 0, 0);
   }
   
   return count;
}

/* FUNZIONE AUSILIARIA: Genera tutte le combinazioni di 'i' elementi
 * 
 * val: array degli elementi
 * n: numero totale di elementi
 * sol: array soluzione
 * i: numero di elementi da scegliere (dimensione del sottoinsieme)
 * pos: posizione corrente in sol
 * start: indice da cui iniziare a considerare elementi
 */
int powerset_r(int* val, int n, int* sol, int i, int pos, int start){
   int count = 0, j;
   
   // CASO BASE: abbiamo scelto 'i' elementi
   if (pos == i){
      printf("{ ");
      for (j = 0; j < i; j++)
         printf("%d ", sol[j]);
      printf("}\n");
      return 1;
   }

   // CASO RICORSIVO: scegli un elemento da 'start' a 'n-1'
   for (j = start; j < n; j++){
      sol[pos] = val[j];  // Scegli l'elemento j
      // Ricorsione: scegli il prossimo elemento da j+1 in poi
      count += powerset_r(val, n, sol, i, pos+1, j+1);
   }
   
   return count;
}

/* ESEMPIO con {1, 2, 3}:
 * 
 * powerset_comb_semp() chiama:
 *   1. Stampa { }
 *   2. powerset_r(..., i=1) → genera C(3,1) = 3 sottoinsiemi:
 *      {1}, {2}, {3}
 *   3. powerset_r(..., i=2) → genera C(3,2) = 3 sottoinsiemi:
 *      {1,2}, {1,3}, {2,3}
 *   4. powerset_r(..., i=3) → genera C(3,3) = 1 sottoinsieme:
 *      {1,2,3}
 * 
 * Totale: 1 + 3 + 3 + 1 = 8 = 2^3 sottoinsiemi
 * 
 * Formula generale: Σ C(n,k) per k=0..n = 2^n
 */

/* ===================================================================
 * CONFRONTO TRA I 3 METODI
 * ===================================================================
 * 
 * 1. DISPOSIZIONI CON RIPETIZIONE (più intuitivo)
 *    - Pro: Semplice da capire (on/off per ogni elemento)
 *    - Pro: Codice conciso
 *    - Contro: Usa un array sol[] di dimensione n anche per insiemi piccoli
 * 
 * 2. DIVIDE ET IMPERA (più efficiente in memoria)
 *    - Pro: Memorizza solo gli elementi scelti
 *    - Pro: Approccio elegante
 *    - Contro: Logica leggermente più complessa
 * 
 * 3. COMBINAZIONI SEMPLICI (più strutturato)
 *    - Pro: Genera i sottoinsiemi ordinati per dimensione
 *    - Pro: Utile se vuoi solo sottoinsiemi di una certa cardinalità
 *    - Contro: Richiede più chiamate ricorsive
 * 
 * Tutti e 3 hanno complessità O(2^n) perché generano tutti i 2^n sottoinsiemi
 */