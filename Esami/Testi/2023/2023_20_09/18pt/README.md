# K-Capacitated Tree Partitioning Solver (Esame 20/09/2023)

Questo progetto risolve un problema di ottimizzazione su grafi che richiede di partizionare i vertici in insiemi disgiunti. Ogni insieme deve indurre un sottografo che sia un albero di almeno k vertici. L'obiettivo è minimizzare la somma totale dei pesi degli archi scelti (MST locali).

<img width="1181" height="553" alt="image" src="https://github.com/user-attachments/assets/1604bd17-455c-4516-8a93-8d1f01ed1511" /># 

##  Struttura del Progetto

Il codice è organizzato in moduli logici distinti per separare le strutture dati, gli algoritmi ausiliari e la logica di business:

* **`Graph.c / .h`** (Modulo Dati)
    Definisce l'ADT Grafo (pesato e non orientato). Gestisce il caricamento da file (GRAPHload), l'accesso alle informazioni sui vertici/archi e la gestione della memoria.

* **`uf.c / .h`** (Modulo Ausiliario)
    Implementa la struttura dati Union-Find (Disjoint Set). È essenziale per verificare la presenza di cicli, controllare la connessione dei componenti e implementare l'algoritmo di Kruskal.

* **`verifica.c`** (Modulo Verifica)
    Driver principale che carica il grafo, esegue i test di verifica su file predefiniti e lancia l'ottimizzazione, stampando i risultati a video.

* **`ottimizzazione.c`** (Modulo Algoritmo)
    Risolve il problema di ricerca dell'ottimo. Utilizza un approccio di Backtracking (Set Partitioning) per esplorare le partizioni dei vertici e calcola il costo minimo usando l'algoritmo di Kruskal sui sottografi indotti.

* **`main.c`** (Main)
    Driver principale che carica il grafo, esegue i test di verifica su file predefiniti e lancia l'ottimizzazione, stampando i risultati a video.
    
##  IMPORTANTE CONFRONTO
Ci sono differenze con la UF della libreria e la Graph, rispetto a quelle usate qua
Questo perchè quelle sono generiche e vanno adattate all'esame specifico
Differenze : 
- UF: 
    - versione usata qua multi-istanza gestisce più UF insieme, quella di libreria solo una 
    - versione usata qua libreria di funzioni, libreria ADT 1 CAT
    - versione qua rileva cicli, libreria unisce e basta
- Graph: 
    - versione qua semplice matrice adiacenza + vettore archi/edge, libreria usa matrice adiacenza + ST
    - versione qua legge vertici come interi, libreria legge vertici come stringhe
    - versione qua struttura opaca, libreria espone getter

###  File di input (.txt)

* **`grafo.txt` `**
    Il grafo di input (formato lista archi).
    Struttura : nodo sinistra, nodo destra, peso arco

* **`sol_ok.txt` `**
    Una soluzione valida di esempio (per il test di verifica).
    Struttura :
    riga 1 : numero di componenti (2)
    componente 1 : 
        - numero di archi (2)
        - elenco archi : 0-1 peso, 1-2 peso
    componente 2 : 
    ecc. 

Per compilare il progetto (assicurarsi di essere nella cartella corretta):
gcc -Wall -g -o esame main.c grafo.c verifica.c ottimizzazione.c uf.c

Per runnare da terminale:
.\esame.exe


COMMENTO SOLUZIONE

nella soluzione k=3 vuole dire che ha 3 vertici ogni componente (0-1-2, 3-4-5)
Partizione vertici: 0 0 0 1 1 1
Ho diviso il grafo in due parti. Nel primo gruppo (ID 0) ho messo i vertici 0, 1 e 2 (il primo triangolo). Nel secondo gruppo (ID 1) ho messo i vertici 3, 4 e 5 (il secondo triangolo)."
