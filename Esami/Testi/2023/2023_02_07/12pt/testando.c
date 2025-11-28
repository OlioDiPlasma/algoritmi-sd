//Es 2

#include "albero.h"

#include <stdlib.h>



struct nodo {
    int info ;
    int n_figli;
    struct nodo **figli;
};


int conta_cammino_rec(T n, int *max_assoluto){

    if ( n == NULL){
        return 0;
    }

    int max_profondita_figli = 0;

    for (int i=0; i< n->n_figli ; i++){
        int profondita_figlio = conta_cammino_rec(n->figli[i], max_assoluto);

        if (profondita_figlio > max_profondita_figli){
            max_profondita_figli = profondita_figlio;
        }
    }

    if (n-> info >= 0 ){
        int cammino_corrente = 1 + max_profondita_figli;
        
        if (cammino_corrente > *max_assoluto){
            *max_assoluto = cammino_corrente;
        }
        return cammino_corrente;
    }
    else {
        return 0;
    }

}


int f(T t){
    int max_len = 0;
    if (t != NULL){
        conta_cammino_rec(t, &max_len);
    }
    return max_len;
}




int main(){


}