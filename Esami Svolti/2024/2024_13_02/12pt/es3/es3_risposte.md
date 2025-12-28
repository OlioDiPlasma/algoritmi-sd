**Modello di calcolo combinatorio**
Il problema richiede di selezionare una sequenza di parole da un elenco dato, senza ripetizioni, dove l'ordine è importante (concatenare "A" + "B" è diverso da "B" + "A").
Pertanto, il modello combinatorio di riferimento è quello delle Disposizioni Semplici.
Non sappiamo a priori quante parole comporranno la soluzione ottima (potrebbero essere tutte $n$, o solo 2), quindi cerchiamo la dispozione di $k$ elementi su $n$ (con $k$ variabile) che massimizza la lunghezza totale in caratteri.



**Pruning**
Se l'ultima parola inserita termina per Vocale, la successiva NON può iniziare per Vocale (deve iniziare per Consonante).
Se l'ultima parola inserita termina per Consonante, la successiva NON può iniziare per Consonante (deve iniziare per Vocale).