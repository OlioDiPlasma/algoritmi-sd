#include "modello.h"


GameData* carica_dati(int L,const char* file_grid, const char* file_bonus){

    GameData *game = (GameData*)malloc(sizeof(GameData));

    if (!game ) exit(1);


    game->L = L;

    FILE *fp_g = fopen(file_grid,"r");
    fscanf(fp_g, "%d", &game->N);

    for (int i=0; i<game->N ; i++){
        for (int j=0 ; j<game->N; j++){
            fscanf(fp_g, "%s", game->grid[i][j]);
        }
    }
    fclose(fp_g);

    FILE *fp_b = fopen(file_bonus,"r");
    fscanf(fp_b, "%d", &game->num_bonus);
    game ->lista_bonus = (Bonus *)malloc(game->num_bonus * sizeof(Bonus));

    for (int i=0; i<game ->num_bonus ; i++){
        game -> lista_bonus[i].id = i;

        fscanf (fp_b, "%d %d" , &game ->lista_bonus[i].len_seq, &game->lista_bonus[i].valore);

        game -> lista_bonus[i].sequenza = (char **)malloc(game->lista_bonus[i].len_seq * sizeof(char*));

        for (int k=0 ; k<game->lista_bonus[i].len_seq; k++) {


            game->lista_bonus[i].sequenza[k] = (char *)malloc(TOKEN_LEN * sizeof(char));
            fscanf(fp_b, "%s", game->lista_bonus[i].sequenza[k]);
        }

        fclose(fp_b);

        return game;


    }


}
void libera_dati(GameData *game){

    for (int i=0; i<game->num_bonus;i++){
        for (int k=0; k<game->lista_bonus[i].len_seq;k++){
            free(game -> lista_bonus[i].sequenza[k]);
        }
        free(game->lista_bonus);
    }
    free(game);


}
