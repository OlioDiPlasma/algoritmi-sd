# Breach Protocol Solver (Esame 07/02/2023)

Questo programma risolve il minigioco di hacking trovando il percorso nella matrice che massimizza il punteggio dei bonus ottenibili, rispettando i vincoli di movimento alternato (riga/colonna).

##  Struttura del Progetto

Il codice è organizzato in 3 moduli logici:

* **`modello.c / .h`** (Modulo Dati)
    Definisce le strutture dati (Griglia, Bonus) e gestisce il caricamento da file (`carica_dati`, `libera_dati`).

* **`solutore.c / .h`** (Modulo Algoritmo)
    Contiene la logica di risoluzione: verifica delle mosse e algoritmo di **Backtracking** ricorsivo per trovare l'ottimo globale (`trova_ottimo`).

* **`main.c`** (Main)
    Gestisce il flusso principale: inizializzazione, chiamata al solutore e pulizia della memoria.

* **`grid.txt` / `bonus.txt`**
    File di input contenenti rispettivamente la matrice di gioco e la lista dei bonus.

Per compilare il progetto (assicurarsi di essere nella cartella corretta):
gcc -Wall -g -o gioco main.c modello.c solutore.c

Per runnare da terminale:
.\gioco.exe 3 grid.txt bonus.txt
// 3 lunghezza buffer MODIFICABILE

