**Obiettivo e Algoritmo**: Il progetto risolve il "Maximum Triangle Packing" (Esame 11/05/23) identificando il massimo numero di triangoli disgiunti nel grafo tramite Backtracking con pruning, utilizzando una Matrice di Adiacenza per garantire verifiche degli archi in $O(1)$.

**Struttura Codice**: main.c funge da driver per i test; grafo.c contiene l'ADT e la logica core, inclusa la generazione dei candidati (findMaxTrianglePacking) e la validazione delle soluzioni (verifySolution).

grafo.txt :
4       <-- Ci sono 4 vertici totali (0, 1, 2, 3)
0 1     <-- C'è una linea tra 0 e 1
1 2     <-- C'è una linea tra 1 e 2
2 0     <-- C'è una linea tra 2 e 0 (chiude il triangolo)

soluzione.txt
1       <-- La soluzione contiene 1 triangolo
0 1 2   <-- I vertici del triangolo sono 0, 1 e 2

**Utilizzo**: Compilare con gcc -o packing main.c grafo.c; l'eseguibile legge grafo.txt (numero vertici + lista archi) e produce soluzione.txt contenente il numero di triangoli trovati e le relative terne di vertici.