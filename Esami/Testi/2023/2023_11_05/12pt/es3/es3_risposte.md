**Scelta del modello combinatorio adottato.**
Backtracking (Ricerca in profondità)
Problema NP-Completo non esiste formula matematica diretta per risolverlo quindi esploriamo albero delle decisioni :
-   prendiamo il primo oggetto non ancora coperto
-   proviamo una alla volte tutte le colonne che contengono quell'oggetto
-   ricorsione


**Criteri di pruning adottati o il motivo della loro assenza. **
-   Per ottimalità (Bound) : Se il numero di insiemi che ho selezionato finora (cnt) è già maggiore o uguale al miglior risultato trovato finora (best_cnt), mi fermo. Non ha senso continuare, cerco il minimo.
-   Per validità (Constraint) : Se il set che sto provando contiene un oggetto che è già stato coperto da un set precedente, scarto subito quel ramo. Il testo richiede insiemi "disgiunti".