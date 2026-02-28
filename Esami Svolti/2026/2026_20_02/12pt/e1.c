#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXS 40

typedef struct {
  char name[MAXS];
  float val;
} Item;
typedef char *Key;

int KEYcmp(Key k1, Key k2) {
  return strcmp(k1,k2);
}
Key KEYget(Item *item) { // E' necessario il puntatore ad Item
  return item->name;
}
void ITEMprint(Item *item) {
  printf("name: %s, val: %f\n", item->name, item->val);
}

float ProcessItems(Item *vet, int n, char **keys_p, Item **maxpp) {
  int totLen = 0;
  float sum=0.0;
  char *keys;
  Item *maxp = NULL;
  for (int i=0; i<n; i++) {
    sum += vet[i].val; // calcola somma
    totLen += strlen(vet[i].name); // lunghezza totale delle stringhe
    if (maxp==NULL || KEYcmp(KEYget(&vet[i]),KEYget(maxp))>0)
      maxp = &vet[i]; // massimo
  }
  keys = malloc(totLen*sizeof(char)+1); // aggiungi 1 per '\0'
  keys[0]='\0'; // inizializza con stringa vuota
  for (int i=0; i<n; i++) 
    strcat(keys,vet[i].name); // concatena stringhe
  
  *keys_p = keys;
  *maxpp = maxp;
  return sum;
}


int main (void) {
  Item vet[]={{"Parigi",2.2},{"Roma",2.75},{"Londra",8.8},{"Berlino",3.75}};
  char *keys; float sumVal;
  Item *maxp;

  int n=sizeof(vet)/sizeof(Item);
  sumVal = ProcessItems(vet,n,&keys,&maxp);
  printf("La somma dei valori e': %f\n", sumVal);
  printf("La concatenazione delle stringhe e': %s\n", keys);
  printf("Il massimo e': "); ITEMprint(maxp);
  free(keys);
  return 0;
}

