
**Commento** 
Vorrei fare un ringraziamento https://github.com/giacomodandolo/Algoritmi_e_Strutture_Dati/tree/main/Strutture_Dati da cui ho rubato i codici

# Fondamentali
Questi sono i "concetti" principali, che definiscono un set 

## `00show_recursion_tree.c`
Fondamentale per visualizzare come funziona la ricorsione e il backtracking.

## `01calcolocombinatorio.c`
Contiene i template per : 
-   `princ_molt` (Principio di Moltiplicazione)
-   `disp_sempl` (Disposizioni Semplici)
-   `disp_ripet` (Disposizioni Ripetute)
-   `perm_sempl` (Permutazioni Semplici)
-   `comb_sempl` (Combinazioni Semplici)
-   `comb_ripet` (Combinazioni Ripetute - ricorda di correggere il bug!)

## `04powerset.c`
È il template specifico per generare sottoinsiemi (scelte binarie "prendo / non prendo"), che è la base per problemi come lo zaino e il subset sum.

## `05part_semplif.c`
È il template per i problemi di partizionamento (dividere N elementi in K gruppi).

## `06recursive_binsearch.c`
È l'esempio perfetto di algoritmo Divide et Impera.

## `02recursive_factorial.c`
Esempio di ricorsione

## `Factorial with explicit stack/`
Mostra il concetto fondamentale di come trasformare una ricorsione in un'iterazione usando uno stack esplicito.


# Esempi Importanti
Questa cartella contiene le applicazioni classiche degli algoritmi di base. Sono i problemi tipici da esame che mostrano come aggiungere vincoli e pruning (ottimizzazioni) ai modelli di base.

## `12knapsack_pruning/`
L'applicazione più classica del modello powerset con l'aggiunta del pruning per ottimizzare la ricerca della soluzione migliore (Problema dello Zaino).

## `4_3_1_SubsetSumPruning.c`
Un altro classico problema di ottimizzazione (Somma di Sottoinsiemi) basato sul powerset con pruning.

## `02LIS.c`
Un problema classico (Longest Increasing Subsequence) che ha una logica ricorsiva a sé stante, fondamentale per la programmazione dinamica.

## `08otto-regine-perm-sempl-ott.c`
l'esempio perfetto di come applicare un modello base (Permutazioni Semplici) aggiungendo dei vincoli (il controllo sulle diagonali).

## `10sudoku.c`
Un classico problema di backtracking con vincoli complessi (controllo su righe, colonne e blocchi).

## `12cavallo.c`
Un altro classico problema di backtracking (Il tour del cavallo) su una matrice/grafo.

## `5_3_Labirinto.c`
L'esempio base di DFS (visita in profondità) per la ricerca di un cammino in una matrice.

## `ConvexHull/`
Famoso algoritmo "Divide et Impera" (Inviluppo Convesso), 