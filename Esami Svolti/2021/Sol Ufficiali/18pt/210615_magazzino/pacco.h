#ifndef PACCO_H_DEFINED
#define PACCO_H_DEFINED

#define LEN 256
typedef enum {errore, occupato, libero} status;

#include<string.h>
#include<stdlib.h>
#include<stdio.h>

typedef struct pacco *PACCO;

PACCO      PACCOinit(char *cod);
void       PACCOstore(FILE *out, PACCO p);
void       PACCOsetSlot(PACCO p, int slot);
void       PACCOsetScaffale(PACCO p, int sc);
void       PACCOsetCorridoio(PACCO p, int co);
char*      PACCOgetCod(PACCO p);
int        PACCOgetSlot(PACCO p);
int        PACCOgetScaffale(PACCO p);
int        PACCOgetCorridoio(PACCO p);

#endif
