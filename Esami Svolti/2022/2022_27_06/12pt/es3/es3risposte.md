È un problema di Disposizioni con Ripetizione con vincoli.
Dobbiamo riempire $k$ caselle. In ogni casella possiamo mettere una lettera (52 opzioni totali: 26 minuscole + 26 maiuscole). Tuttavia, i vincoli ci costringono a "tagliare" (pruning) rami dell'albero delle decisioni che non sono valide


PER DEBUG USATO I<3 QUESTO SI APPLICA A I<26

Dimostrazione Matematica del RisultatoStiamo cercando sequenze di 3 caratteri ($k=3$) con alfabeto di 26 lettere.Vincoli:Max 1 minuscola (perché $k/2 = 1.5 \to 1$).Nessuna ripetizione consecutiva ($p=1$), quindi il carattere attuale deve essere diverso dal precedente.

Le configurazioni valide di Maiuscole (U) e Minuscole (l) sono 4:U U U (Tutte maiuscole)U U l (1 minuscola alla fine)U l U (1 minuscola in mezzo)l U U (1 minuscola all'inizio)

Calcoliamo le combinazioni per il caso U U U:1° posizione: ho 26 scelte (A...Z).2° posizione: ho 25 scelte (tutte tranne quella usata prima).3° posizione: ho 25 scelte (tutte tranne quella usata nella 2° pos).$$26 \times 25 \times 25 = 16.250 \text{ soluzioni}$$Poiché le 4 configurazioni (UUU, UUl, UlU, lUU) hanno strutturalmente la stessa libertà di scelta (cambia solo il case, ma il numero di opzioni è identico):

$$16.250 \times 4 = 65.000$$