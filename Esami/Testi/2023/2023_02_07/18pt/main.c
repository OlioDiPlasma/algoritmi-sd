#include <stdio.h>
#include "modello.h"
#include "solutore.h"
#include <stdio.h>
#include <stdlib.h>
#include "modello.h"
#include "solutore.h"

// 1. Aggiungi argc e argv negli argomenti del main
int main(int argc, char *argv[]) {
    
    // 2. Controllo degli argomenti (Nome programma + L + file1 + file2 = 4 argomenti)
    if (argc != 4) {
        printf("Errore! Uso corretto: %s <L> <file_grid> <file_bonus>\n", argv[0]);
        return 1;
    }

    // 3. Converti la stringa argv[1] (es. "3") in un intero
    int L = atoi(argv[1]); 
    
    printf("--- BREACH PROTOCOL SOLVER ---\n");
    printf("Cerco percorso di lunghezza %d...\n", L);

    // ... poi chiami le tue funzioni usando argv[2] e argv[3]
    GameData *game = carica_dati(L, argv[2], argv[3]);
    
    trova_ottimo(game);
    
    libera_dati(game);
    
    return 0;
}