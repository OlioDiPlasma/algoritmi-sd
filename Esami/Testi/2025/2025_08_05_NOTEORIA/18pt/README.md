gcc -Wall -g -o esame main.c GRID.c WORDS.c SOL.c

Il Test "Critico": Le Sovrapposizioni
Nel test attuale (5x5), le parole non si toccano.

ALB scorre sulla colonna 0.

CUBO parte dalla colonna 1.

KK è sopra ALB.

EVNA è sotto ALB.

Per un esame da 30 e lode, dobbiamo verificare che il vincolo "Massimo 1 carattere in comune" funzioni davvero. Se il codice accettasse due parole che si sovrappongono per 2 lettere consecutive, verremmo penalizzati.