# (Esame 30/06/2023)

Questo programma risolve una variante dello "Sliding Puzzle" in cui bisogna collegare due terminali (Sorgente e Destinazione) creando un canale continuo spostando le tessere sulla griglia.

##  Struttura del Progetto

Il codice è organizzato in 2 unità principali più il driver di test:

* **`puzzle.c / .h`** (Modulo Core & Logica)
    - Dati: Definisce le strutture Game (griglia) e Tile (tessera con connessioni N/S/E/W).
    - I/O: Gestisce il caricamento da file (load_game) e la liberazione della memoria (free_game).
    - Fisica: Implementa la logica di spostamento tessere (apply_move) gestendo i buchi.
    - Solver: Contiene l'algoritmo di verifica connessione (DFS) e l'algoritmo di ricerca della soluzione ottima (Iterative Deepening Backtracking).

* **`main.c`** (Main Driver)
    Gestisce il flusso dell'esame: Caricamento -> Verifica Mosse -> Reset Memoria -> Ottimizzazione.

* **`grid.txt` / `mosse.txt`**
    configurazione iniziale della griglia (Dimensione e matrice di 0/1) e sequenza di mosse (es: R U U) per il test di verifica.


**Per compilare il progetto (assicurarsi di essere nella cartella corretta):**
gcc -Wall -g -o tester main.c puzzle.c

.\tester.exe
