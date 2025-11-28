
#ifndef MODELLO_H
#define MODELLO_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_N 20 

#define TOKEN_LEN 3

typedef struct{
    int r,c;
} Coord;

typedef struct {
    int id;
    int valore;
    int len_seq;
    char **sequenza; 
} Bonus;

typedef struct{
    int N;
    char grid[MAX_N][MAX_N][TOKEN_LEN];
    int num_bonus;

    Bonus *lista_bonus;
    int L;

} GameData;


GameData* carica_dati(int L,const char* file_grid, const char* file_bonus);
void libera_dati(GameData *game);



#endif