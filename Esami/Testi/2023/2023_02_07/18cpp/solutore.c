#include "solutore.h"
#include <stdbool.h>


static int max_punteggio = -1;
static Coord *miglior_percorso = NULL;


//
static int calcola_punteggio(GameData *game, char **buffer_tokens){
    int tot=0;


    for (int b=0 ; b<game->num_bonus ; b++){
        Bonus bonus = game -> lista_bonus[b];

        if (bonus.len_seq > game->L) continue;

        bool trovato = false;

        for (int i=0; i<game->L - bonus.len_seq ; i++){
            bool match = true;

            for (int k=0; k<bonus.len_seq ; k++){
                if (strcmp(buffer_tokens[i+k],bonus.sequenza[k]) != 0){
                    match=false;
                    break;
                }
            }
            if (match) { trovato=true ; break;}


        }        
        if (trovato) tot += bonus.valore;

    }
    return tot;
}


void trova_ottimo(GameData *game);


