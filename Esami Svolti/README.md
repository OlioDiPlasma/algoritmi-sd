
**Comando generico per compilare multi-file:**
gcc -Wall -g -o tester(nome exe che creiamo) main.c file1.c(eventuali file .c aggiuntivi)

**IMPORTANTI:**
4 tipi: 
-   disposizioni (semplici/ripetute)
-   combinazioni (semplici/ripetute)
-   powerset  
-   partizioni


- 11/05/2023 (powerset e combinazioni semplici), 
- 09/02/2022 (percorso su matrice con disposizioni semplici) 
- 27/06/2022 (partizioni di un insieme);



**Esempio all'esame di multi modulo**
-------ST.h---------- 


//interfaccia di ST con definzione del puntatore + funzioni 




-------ST.c---------- 


-------Grafo.h---------- 
//interfaccia di Grafo con definzione del puntatore + funzioni 



-------Grafo.c---------- 


**Lista argomenti esami** 
**2014**

2014_03_09
-   12 :
-   18 : 
  
2014_06_02
-   12 : 
-   18 : 

2014_24_02
-   12 : 
-   18 : 

2014_27_06  
-   12 :
-   18 :
  
//////////////////////////////////////////////////////7  
**2015**

2015_02_02 
-   12 :
-   18 : 
  
2015_02_09
-   12 : 
-   18 : 

2015_16_06
-   12 : 
-   18 : 

2015_23_02
-   12 :
-   18 :
  
//////////////////////////////////////////////////////7  
**2016**

2016_09_09
-   12 :
-   18 : 
  
2016_14_06
-   12 : 
-   18 : 

2016_22_02
-   12 : 
-   18 : 

2016_26_01
-   12 :
-   18 :
  
//////////////////////////////////////////////////////7  
**2017**

2017_04_02 
-   12 :
-   18 : 
  
2017_18_09
-   12 : 
-   18 : 

2017_22_02
-   12 : 
-   18 : 

2017_26_06  
-   12 :
-   18 :
  
//////////////////////////////////////////////////////7   
**2018**

2018_13_02 
-   12 :
-   18 : 
  
2018_13_09
-   12 : 
-   18 : 

2018_21_06
-   12 : 
-   18 : 

2018_29_01
-   12 :
-   18 :
  
//////////////////////////////////////////////////////7  
**2019**

2019_02_07 
-   12 :
-   18 : 
  
2019_18_09
-   12 : 
-   18 : 

2019_22_02
-   12 : 
-   18 : 

2019_31_01
-   12 :
-   18 :
  

//////////////////////////////////////////////////////7  
**2020**

2020_28_01
-   12 :
-   18 : 
  
2020_21_02
-   12 : 
-   18 : 

2020_22_06
-   12 : 
-   18 : 

2020_03_09
-   12 :
-   18 :

//////////////////////////////////////////////////////7  
**2021**

2021_26_01 
-   12 :
-   18 : 
  
2021_16_02
-   12 : 
-   18 : 

2021_15_06
-   12 : 
-   18 : 

2021_31_09  
-   12 :
-   18 :
  
//////////////////////////////////////////////////////7  
**2022**

2022_02_09 
-   12 : vettori duplicati, ADT albero, disposizioni con ripetizione, powerset
-   18 : 
  
2022_09_02
-   12 : 
-   18 : 

2022_27_01
-   12 : 
-   18 : 

2022_27_06
-   12 :
-   18 :
  
//////////////////////////////////////////////////////7  
**2023**

2023_07_02 
-   12 : matrice, alberi, matrice con backtracking
-   18 : hacking, custom, backtracking

2023_11_05
-   12 : matrice, list + node ADT,  Backtracking (Ricerca in profondità)
-   18 : triangle packing, grafo non orientato e non pesato

2023_20_09
-   12 : matrice , list+node ADT,  Disposizioni Semplici
-   18 : partizionamento, grafo non orientato e pesato

2023_21_02_NOTEORIA  
-   12 : matrice, list + node ADT,  Ricorsione con Backtracking
-   18 : incarichi/attività

2023_30_06  
-   12 : matrice, list + node,  Decision Tree
-   18 : puzzle
  
//////////////////////////////////////////////////////7  
**2024**

*Appello 13 Febbraio 2024*
Traccia 12 Punti:
- Liste (Merge): Fusione di due liste ordinate sommando i valori per chiavi uguali (senza duplicati).
- BST: Linearizzazione dei nodi in un vettore ordinato per profondità e valore.
- Stringhe e Ricorsione: Concatenazione di parole rispettando vincoli fonetici (vocali/consonanti) per ottenere la stringa più lunga.
Traccia 18 Punti:
- Grafi e Stringhe: Cammini su grafo con nodi aventi nomi non univoci. Verifica validità stringa su cammino e ricerca cammino a valore massimo con numero limitato di ripetizioni e vincoli fonetici.
Teoria: Heap (trace), DP (Catena matrici), Visite alberi, Hash Table (Double hashing), Kruskal (MST), Dijkstra.

*Appello 26 Febbraio 2024*
Traccia 12 Punti:
- Matrici e Stringhe: Ricerca di parole all'interno di una matrice di testo.
- Heap e Alberi: Conversione da Heap a Albero Binario (struttura pointer-based) e estrazione.
- Calcolo Combinatorio: Generazione di numeri in base B con vincoli sulle cifre e somma minima.
Traccia 18 Punti:
- Pianificazione Attività: Selezione di un sottoinsieme di attività con precedenze per massimizzare il profitto (Weighted Interval Scheduling / Knapsack su grafo).
Teoria: Greedy (Attività), LIS (DP), BST estesi (Rango), Quicksort, Punti di articolazione, DAG (cammini massimi), Bellman-Ford.

*Appello 04 Luglio 2024 e 13 Settembre 2024*(Nota: Le tracce di programmazione appaiono identiche o molto simili tra questi due appelli nei documenti analizzati).Traccia 12 Punti:
- Vettori: Differenza insiemistica tra due vettori ordinati (diffVett).
- BST: Ricerca della foglia a profondità massima e stampa del percorso inverso.Matrici (Labirinto): 
- Calcolo cammino minimo tra due caselle libere (BFS su griglia).
Traccia 18 Punti:
- Ottimizzazione (Location Problem):  Localizzazione sedi Pronto Soccorso. Selezione $M$ sedi per coprire città entro distanza massima e garantire carico minimo.
Teoria: Huffman, DP (Catene montaggio), Ricostruzione alberi, IBST, DFS, Classificazione archi.

  
//////////////////////////////////////////////////////7  
**2025**

*Appello 27 Gennaio 2025*
Traccia 12 Punti (Programmazione Semplificata):
- Vettori: Verifica di sottomatrice/sottosequenza (algoritmo lineare tipo "merge").
- BST (Alberi Binari di Ricerca): Ricerca del cammino radice-foglia con il massimo numero di nodi aventi due figli.
- Stringhe e Ricorsione: Verifica concatenabilità di stringhe (prefissi/suffissi) e ricerca della sequenza di concatenazione più lunga (permutazioni con pruning).
Traccia 18 Punti (Programmazione Completa + Teoria):
- Grafi e Ottimizzazione: Problema di connessione "fibra ottica". Gestione di grafi con abilitazione/disabilitazione archi. Calcolo di alberi ricoprenti (Spanning Trees) su componenti connesse minimizzando una funzione di costo mista (media e differenza max-min).
Teoria: Quicksort (trace), Programmazione Dinamica (Catena di matrici), Visite alberi (pre/in/post-order), Hash Table (Quadratic probing), BST (inserzione/cancellazione), Algoritmo di Dijkstra.

*Appello 21 Febbraio 2025*
Traccia 12 Punti:
- ADT e Allocazione Dinamica: Gestione di un ADT "Editori" (array di strutture con stringhe e puntatori esterni), funzioni di duplicazione (deep copy) e deallocazione.
- BST: Verifica di bilanciamento dell'albero basato sulla dimensione dei sottoalberi.
- Ricorsione su Vettori: Partizionamento di un vettore in due insiemi per minimizzare la differenza delle somme (problema di partizione).
Traccia 18 Punti:
- Pianificazione (Grafi/DAG): Problema delle dipendenze tra attività. Verifica esistenza cicli (DAG vs Albero) e stampa sorgenti/pozzi.
- Ottimizzazione Risorse: Calcolo del minimo numero di persone per svolgere attività sovrapposte (Interval Partitioning) e assegnazione ottima per bilanciare i compensi.
Teoria: Risoluzione ricorrenze, Heap (inserimento/estrazione), LIS (Longest Increasing Subsequence), BST estesi (statistiche d'ordine).

*Appello 25 Giugno 2025*
Traccia 12 Punti:
- Liste: Cancellazione di nodi da una lista in base alla lunghezza della stringa contenuta.
- BST: Duplicazione completa di un albero binario di ricerca.
- Grafi: Verifica se un grafo orientato è simmetrico e privo di cappi (self-loops).
Traccia 18 Punti:
- Ottimizzazione Vincolata: Problema del trasporto oggetti con vincoli di peso, volume e incompatibilità (coppie vincolate). Minimizzazione dei carichi (giorni) e bilanciamento costi (Bin Packing / Set Partitioning variations).
Teoria: Ricorrenze, Programmazione Dinamica (Catene di montaggio), Conversione espressioni (Infissa a Prefissa), Partizionamento BST (mediana), Hash Table (Double Hashing), Algoritmo di Prim.

*Appello Straordinario (05 Agosto 2025)*
Traccia 12 Punti:
- Liste Ordinate: Ricerca, modifica valore e riposizionamento di un elemento mantenendo l'ordine.
- BST: Verifica della proprietà fondamentale dei BST (ordinamento chiavi).
- Grafi: Verifica se una sequenza di vertici costituisce un cammino Hamiltoniano.
Traccia 18 Punti:
- Matrici e Stringhe (Crucipuzzle): Gestione griglia di caratteri, ricerca parole in varie direzioni, verifica validità soluzione.
- Ottimizzazione: Ricerca ricorsiva dell'insieme di parole che massimizza il punteggio totale rispettando vincoli di incrocio.

*Appello 09 Settembre 2025*
Traccia 12 Punti:
- Liste: Conteggio nodi comuni (intersezione) tra due liste.
- BST: Calcolo della distanza (lunghezza cammino) tra due nodi dati le chiavi.
- Grafi: Analisi relazioni "amici di amici" (vicini di livello 2 esclusi i diretti).
Traccia 18 Punti:
- Ottimizzazione Trasporti: Problema dei Minibus (Persone, Valigie, Vincoli "travel-together"). Minimizzazione numero bus e chilometri totali (Set Partitioning).
Teoria: Codici di Huffman, Algoritmi Greedy (Selezione attività), Ricostruzione albero da visite, IBST (Alberi di intervalli), DFS (classificazione archi), Punti di articolazione.

