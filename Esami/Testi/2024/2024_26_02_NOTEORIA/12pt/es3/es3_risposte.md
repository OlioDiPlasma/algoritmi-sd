**Modello di calcolo combinatorio**
Immagina di dover generare un numero di 3 cifre in base 10.Il Modello Combinatorio di base sono le Disposizioni con Ripetizione: hai $10^3 = 1000$ possibili combinazioni.L'algoritmo ricorsivo esplora un "albero delle decisioni":Al livello 0 scelgo la prima cifra (1-9).Al livello 1 scelgo la seconda (0-9)....e così via.

**Pruning**
Nel nostro caso abbiamo due "forbici" per potare:

Potatura sul Vincolo Ripetizioni: Se sto per inserire una cifra che viola la regola "massimo una cifra ripetuta", mi fermo.
Potatura sulla Somma: Se la somma attuale più il massimo che posso ottenere con le cifre rimanenti è comunque inferiore a minS, mi fermo. È inutile continuare.