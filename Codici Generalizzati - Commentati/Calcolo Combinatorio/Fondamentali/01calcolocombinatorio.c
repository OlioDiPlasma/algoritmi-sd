#include <stdio.h>
#include <stdlib.h>

// Struttura per il principio di moltiplicazione
// Rappresenta un "livello" di decisione con le sue scelte possibili
typedef struct { 
  int *scelte;      // Array delle scelte disponibili a questo livello
  int num_scelte;   // Numero di scelte disponibili
} Livello;

// Dichiarazioni delle funzioni
int princ_molt(int pos, Livello *val, int *sol, int k, int count);
int disp_sempl(int pos, int *val, int *sol, int *mark, int n, int k, int count);
int disp_ripet(int pos, int *val, int *sol, int n, int k, int count);
int perm_sempl(int pos, int *val, int *sol, int *mark, int n,  int count);
int perm_ripet(int pos, int *dist_val, int *sol, int *mark, int n,  int n_dist, int count);
int comb_sempl(int pos, int *val, int *sol, int n, int k, int start, int count);
int comb_ripet(int pos, int *val, int *sol, int n, int k, int start, int count);
void Merge(int *val, int l, int q, int r);
void MergeSort(int *val, int l, int r);

int main(void){
  int i, j, n, n_dist, k, total, *val, *dist_val, *sol, *mark;
  Livello *valor;

  // Stampa del menu principale
  printf("\n");
  printf("\n Calcolo Combinatorio su interi\n");
  printf("===============\n");
  printf("1. Principio di moltiplicazione\n");
  printf("2. Disposizioni semplici\n");
  printf("3. Disposizioni ripetute\n");
  printf("4. Permutazioni semplici\n");
  printf("5. Permutazioni ripetute\n");
  printf("6. Combinazioni semplici\n");
  printf("7. Combinazioni ripetute\n");
  printf("8. Exit\n");
  printf("Selezionare : ");
  
  // Legge la scelta dell'utente
  if(scanf("%d",&i)<=0) {
    printf("Solo interi!\n");
    exit(0);
  }
  else {
    switch(i) {
      // CASO 1: PRINCIPIO DI MOLTIPLICAZIONE
      case 1:     
                  printf("Inserisci il numero di decisioni n: ");
                  scanf("%d", &k);  // k = numero di livelli/decisioni
                  
                  // Alloca array di strutture Livello
                  valor = malloc(k * sizeof(Livello));
                  
                  // Per ogni livello, chiede quante scelte ci sono
                  for(i = 0; i < k; i++) {
                    printf("Inserisci il numero di scelte per il passo %d: ", i);
                    scanf("%d", &valor[i].num_scelte);
                    // Alloca spazio per le scelte di questo livello
                    valor[i].scelte = malloc(valor[i].num_scelte * sizeof(int));
                  }
                  
                  // Legge tutte le scelte per ogni livello
                  for (i = 0; i < k; i++)
                    for (j = 0; j < valor[i].num_scelte; j++) {
                      printf("inserisci scelta %d-esima a livello %d: ", j, i);
                      scanf("%d", &valor[i].scelte[j]);
                  }
                  
                  // Array per memorizzare la soluzione corrente
                  sol = malloc(k * sizeof(int));

                  printf("L'applicazione del principio di moltiplicazione da' le seguenti soluzioni: \n");
                  total = princ_molt(0, valor, sol, k, 0);
                  printf("e in totale sono %d \n", total);
                  
                  // Libera la memoria
                  free(sol);
                  for (i = 0; i < k; i++)
                    free(valor[i].scelte);
                  free(valor);
                  break;
                  
      // CASO 2: DISPOSIZIONI SEMPLICI (D(n,k) senza ripetizioni)
      case 2:     
                  printf("Inserisci n: ");
                  scanf("%d", &n);  // n = numero totale di elementi
                  printf("Inserisci k: ");
                  scanf("%d", &k);  // k = elementi da scegliere
                  
                  val = malloc(n * sizeof(int));      // Array elementi
                  sol = malloc(k * sizeof(int));      // Array soluzione
                  mark = calloc(n, sizeof(int));      // Array per marcare elementi usati
                  
                  // Inizializza soluzione a -1
                  for (i=0; i < k; i++)
                    sol[i]= -1;
                  
                  // Legge gli n elementi
                  for (i=0; i<n; i++) {
                    printf("Inserisci elemento %d: ", i);
                    scanf("%d", &val[i]);
                  }
                  
                  printf("Le disposizioni semplici di %d interi  ", n);
                  for (i=0; i <n; i++)
                    printf("%d ", val[i]);
                  printf(" a %d a %d sono:\n", k, k);
                  
                  total = disp_sempl(0, val, sol, mark, n, k, 0);
                  printf("e in totale sono %d \n", total);
                  
                  free(sol);
                  free(val);
                  free(mark);
                  break;
                  
      // CASO 3: DISPOSIZIONI RIPETUTE (D'(n,k) con ripetizioni)
      case 3:     
                  printf("Inserisci n: ");
                  scanf("%d", &n);
                  printf("Inserisci k: ");
                  scanf("%d", &k);
                  
                  val = malloc(n * sizeof(int));
                  sol = malloc(k * sizeof(int));
                  
                  for (i=0; i < k; i++)
                    sol[i]= -1;
                  
                  for (i=0; i<n; i++) {
                    printf("Inserisci elemento %d: ", i);
                    scanf("%d", &val[i]);
                  }
                  
                  printf("Le disposizioni ripetute di %d interi  ", n);
                  for (i=0; i <n; i++)
                    printf("%d ", val[i]);
                  printf("a %d a %d sono:\n", k, k);
                  
                  total = disp_ripet(0, val, sol, n, k, 0);
                  printf("e in totale sono %d \n", total);
                  
                  free(sol);
                  free(val);
                  break;
                  
      // CASO 4: PERMUTAZIONI SEMPLICI (P(n) senza ripetizioni)
      case 4:     
                  printf("Inserisci n: ");
                  scanf("%d", &n);
                  
                  val = malloc(n * sizeof(int));
                  sol = malloc(n * sizeof(int));
                  mark = calloc(n, sizeof(int));  // Marca gli elementi già usati
                  
                  for (i=0; i < n; i++)
                    sol[i]= -1;
                  
                  for (i=0; i<n; i++) {
                    printf("Inserisci elemento %d: ", i);
                    scanf("%d", &val[i]);
                  }
                  
                  printf("Le permutazioni semplici di %d interi  ", n);
                  for (i=0; i <n; i++)
                    printf("%d ", val[i]);
                  printf("sono:\n");
                  
                  total = perm_sempl(0, val, sol, mark, n, 0);
                  printf("e in totale sono %d \n", total);
                  
                  free(sol);
                  free(val);
                  free(mark);
                  break;
                  
      // CASO 5: PERMUTAZIONI RIPETUTE (con elementi duplicati)
      case 5:     
                  printf("Inserisci n: ");
                  scanf("%d", &n);
                  
                  val = malloc(n * sizeof(int));
                  dist_val = malloc(n * sizeof(int));  // Valori distinti
                  sol = malloc(n * sizeof(int));
                  
                  for (i=0; i<n; i++) {
                    printf("Inserisci elemento %d: ", i);
                    scanf("%d", &val[i]);
                  }
                  
                  printf("Le permutazioni distinte di %d interi con ripetizione ", n);
                  for (i=0; i <n; i++)
                    printf("%d ", val[i]);
                  printf("sono:\n");
                  
                  // Ordina l'array per raggruppare gli elementi uguali
                  MergeSort(val, 0, n-1);
                  
                  // Conta quanti valori distinti ci sono
                  n_dist = 1;
                  for (i=1; i<n; i++)
                    if(val[i-1] != val[i])
                      n_dist++;
                  
                  // mark[i] conterrà la frequenza del valore distinto i
                  mark = calloc(n_dist,sizeof(int));
                  
                  // Estrae i valori distinti
                  j = 0;
                  dist_val[j++] = val[0];
                  for (i=1; i<n; i++)
                    if(val[i-1] != val[i])
                      dist_val[j++]= val[i];
                  
                  // Conta le frequenze di ogni valore distinto
                  i = 1;
                  j = 0;
                  mark[j] = 1;  // Il primo elemento ha frequenza 1
                  while(i < n) {
                    // Conta quante volte appare l'elemento corrente
                    while (val[i-1] == val[i]) {
                      i++;
                      mark[j]++;
                    }
                    j++;
                    mark[j]++;
                    i++;
                  }
                  
                  for (i=0; i < n; i++)
                    sol[i]= -1;
                  
                  total = perm_ripet(0, dist_val, sol, mark, n, n_dist, 0);
                  printf("e in totale sono %d \n", total);
                  
                  free(sol);
                  free(val);
                  free(dist_val);
                  free(mark);
                  break;
                  
      // CASO 6: COMBINAZIONI SEMPLICI (C(n,k) senza ripetizioni)
      case 6:     
                  printf("Inserisci n: ");
                  scanf("%d", &n);
                  printf("Inserisci k: ");
                  scanf("%d", &k);
                  
                  val = malloc(n * sizeof(int));
                  sol = malloc(k * sizeof(int));
                  
                  for (i=0; i < k; i++)
                    sol[i]= -1;
                  
                  for (i=0; i<n; i++) {
                    printf("Inserisci elemento %d: ", i);
                    scanf("%d", &val[i]);
                  }
                  
                  printf("Le combinazioni  semplici  di %d interi  ", n);
                  for (i=0; i <n; i++)
                    printf("%d ", val[i]);
                  printf(" a %d a %d sono:\n", k, k);
                  
                  total = comb_sempl(0, val, sol, n, k, 0, 0);
                  printf("e in totale sono %d \n", total);
                  
                  free(sol);
                  free(val);
                  break;
                  
      // CASO 7: COMBINAZIONI RIPETUTE (C'(n,k) con ripetizioni)
      case 7:     
                  printf("Inserisci n: ");
                  scanf("%d", &n);
                  printf("Inserisci k: ");
                  scanf("%d", &k);
                  
                  val = malloc(n * sizeof(int));
                  sol = malloc(k * sizeof(int));
                  
                  for (i=0; i < k; i++)
                    sol[i]= -1;
                  
                  for (i=0; i<n; i++) {
                    printf("Inserisci elemento %d: ", i);
                    scanf("%d", &val[i]);
                  }
                  
                  printf("Le combinazioni  ripetute  di %d interi  ", n);
                  for (i=0; i <n; i++)
                    printf("%d ", val[i]);
                  printf(" a %d a %d sono:\n", k, k);
                  
                  total = comb_ripet(0, val, sol, n, k, 0, 0);
                  printf("e in totale sono %d \n", total);
                  
                  free(sol);
                  free(val);
                  break;
                  
      case 8:     
                  break;
                  
      default:    
                  printf("Opzione non valida\n");
    }
  }
  return 0;
}

/* PRINCIPIO DI MOLTIPLICAZIONE
 * Genera tutte le combinazioni possibili scegliendo una opzione per ogni livello
 * pos: posizione corrente (livello di decisione)
 * val: array di livelli con le loro scelte
 * sol: soluzione parziale corrente
 * k: numero totale di livelli
 * count: contatore delle soluzioni trovate
 */
int princ_molt(int pos, Livello *val, int *sol, int k, int count) {
  int i;
  
  // CASO BASE: abbiamo riempito tutti i livelli
  if (pos >= k) {
    // Stampa la soluzione completa
    for (i = 0; i < k; i++)
      printf("%d ", sol[i]);
    printf("\n");
    return count+1;  // Incrementa il contatore
  }
  
  // CASO RICORSIVO: prova tutte le scelte del livello corrente
  for (i = 0; i < val[pos].num_scelte; i++) {
    sol[pos] = val[pos].scelte[i];  // Scegli l'opzione i
    count = princ_molt(pos+1, val, sol, k, count);  // Vai al prossimo livello
  }
  
  return count;
}

/* DISPOSIZIONI SEMPLICI
 * Arrangiamenti di k elementi da n, senza ripetizioni e ordine importante
 * pos: posizione corrente nella soluzione
 * val: array di elementi disponibili
 * sol: soluzione parziale
 * mark: array per marcare gli elementi già usati (1=usato, 0=disponibile)
 * n: numero totale di elementi
 * k: numero di elementi da scegliere
 * count: contatore soluzioni
 */
int disp_sempl(int pos, int *val, int *sol, int *mark, int n, int k, int count) {
  int i;
  
  // CASO BASE: abbiamo scelto k elementi
  if (pos >= k) {
    for (i=0; i<k; i++)
      printf("%d ", sol[i]);
    printf("\n");
    return count+1;
  }
  
  // CASO RICORSIVO: prova tutti gli elementi non ancora usati
  for (i=0; i<n; i++) {
    if (mark[i] == 0) {  // Se l'elemento i non è stato ancora usato
      mark[i] = 1;       // Marcalo come usato
      sol[pos] = val[i]; // Inseriscilo nella soluzione
      count = disp_sempl(pos+1, val, sol, mark, n, k, count);
      mark[i] = 0;       // BACKTRACK: rimarca come disponibile
    }
  }
  
  return count;
}

/* DISPOSIZIONI RIPETUTE
 * Come le disposizioni semplici ma gli elementi possono ripetersi
 * Non serve l'array mark perché possiamo riusare gli elementi
 */
int disp_ripet(int pos, int *val, int *sol, int n, int k, int count) {
  int i;
  
  // CASO BASE
  if (pos >= k) {
    for (i=0; i<k; i++)
      printf("%d ", sol[i]);
    printf("\n");
    return count+1;
  }
  
  // CASO RICORSIVO: possiamo scegliere qualsiasi elemento, anche ripetuto
  for (i = 0; i < n; i++) {
    sol[pos] = val[i];  // Nessun controllo su mark
    count = disp_ripet(pos+1, val, sol, n, k, count);
  }
  
  return count;
}

/* PERMUTAZIONI SEMPLICI
 * Tutti gli arrangiamenti di n elementi (k=n), senza ripetizioni
 * È un caso particolare delle disposizioni semplici con k=n
 */
int perm_sempl(int pos, int *val, int *sol, int *mark, int n,  int count) {
  int i;
  
  // CASO BASE: abbiamo usato tutti gli n elementi
  if (pos >= n) {
    for (i=0; i<n; i++)
      printf("%d ", sol[i]);
    printf("\n");
    return count+1;
  }

  // CASO RICORSIVO
  for (i=0; i<n; i++)
    if (mark[i] == 0) {  // Elemento non ancora usato
      mark[i] = 1;
      sol[pos] = val[i];
      count = perm_sempl(pos+1, val, sol, mark, n,  count);
      mark[i] = 0;  // BACKTRACK
    }
  
  return count;
}

/* PERMUTAZIONI RIPETUTE
 * Permutazioni di elementi dove alcuni possono essere uguali
 * dist_val: array dei valori distinti
 * mark: frequenze disponibili per ogni valore distinto
 * n: numero totale di elementi (incluse ripetizioni)
 * n_dist: numero di valori distinti
 */
int perm_ripet(int pos, int *dist_val, int *sol, int *mark, int n, int n_dist, int count) {
  int i;
  
  // CASO BASE
  if (pos >= n) {
    for (i=0; i<n; i++)
      printf("%d ", sol[i]);
    printf("\n");
    return count+1;
  }
  
  // CASO RICORSIVO: prova tutti i valori distinti
  for (i=0; i<n_dist; i++) {
    if (mark[i] > 0) {  // Se ci sono ancora "copie" di questo valore
      mark[i]--;        // Usa una copia
      sol[pos] = dist_val[i];
      count = perm_ripet(pos+1, dist_val, sol, mark, n, n_dist, count);
      mark[i]++;        // BACKTRACK: ripristina la copia
    }
  }
  
  return count;
}

/* COMBINAZIONI SEMPLICI
 * Sottoinsiemi di k elementi da n, senza ripetizioni, ordine NON importante
 * start: indice da cui iniziare a considerare elementi (per evitare duplicati)
 */
int comb_sempl(int pos, int *val, int *sol, int n, int k, int start,  int count) {
  int i;
  
  // CASO BASE
  if (pos >= k) {
    for (i=0; i<k; i++)
      printf("%d ", sol[i]);
    printf("\n");
    return count+1;
  }
  
  // CASO RICORSIVO: considera solo elementi da start in poi
  // Questo garantisce che non generiamo duplicati (es. {1,2} e {2,1})
  for (i=start; i<n; i++) {
    sol[pos] = val[i];
    count = comb_sempl(pos+1, val, sol, n, k, i+1, count);  // Prossima chiamata parte da i+1
  }
  
  return count;
}

/* COMBINAZIONI RIPETUTE
 * Come le combinazioni semplici ma gli elementi possono ripetersi
 * La differenza è che passiamo 'start' (non 'start+1') alla ricorsione,
 * permettendo di riusare lo stesso elemento
 */
int comb_ripet(int pos, int *val, int *sol, int n, int k, int start,  int count) {
  int i;
  
  // CASO BASE
  if (pos >= k) {
    for (i=0; i<k; i++)
      printf("%d ", sol[i]);
    printf("\n");
    return count+1;
  }
  
  // CASO RICORSIVO
  for (i=start; i<n; i++) {
    sol[pos] = val[i];
    count = comb_ripet(pos+1, val, sol, n, k, start, count);  // start resta uguale!
    start++;  // Ma incrementiamo start per la prossima iterazione del ciclo
  }
  
  return count;
}

/* MERGE - Funzione ausiliaria per MergeSort
 * Fonde due sotto-array ordinati in un unico array ordinato
 * val: array da ordinare
 * l: indice sinistro
 * q: indice medio
 * r: indice destro
 */
void Merge(int *val, int l, int q, int r) {
  int i, j, k, *B;
  int len;
  
  len = r - l + 1;
  B = malloc(len * sizeof(int));  // Array temporaneo
  
  i = l;      // Indice per il sotto-array sinistro [l..q]
  j = q+1;    // Indice per il sotto-array destro [q+1..r]
  
  // Fonde i due sotto-array in B
  for(k = l; k <= r; k++)
    if (i > q)  // Sotto-array sinistro esaurito
      B[k] = val[j++];
    else if (j > r)  // Sotto-array destro esaurito
      B[k] = val[i++];
    else if ((val[i] < val[j]) || (val[i] == val[j]))  // Confronta
      B[k] = val[i++];
    else
      B[k] = val[j++];
  
  // Copia B in val
  for (k = l; k <= r; k++)
    val[k] = B[k];
  
  return;
}

/* MERGESORT - Algoritmo di ordinamento ricorsivo
 * Divide l'array a metà, ordina ricorsivamente le due metà, poi le fonde
 * Complessità: O(n log n)
 */
void MergeSort(int *val, int l, int r) {
  int q;
  
  // CASO BASE: un solo elemento (o array vuoto)
  if (r <= l)
    return;
  
  // CASO RICORSIVO
  q = (l + r)/2;           // Trova il punto medio
  MergeSort(val, l, q);    // Ordina la metà sinistra
  MergeSort(val, q+1, r);  // Ordina la metà destra
  Merge(val, l, q, r);     // Fondi le due metà ordinate
}