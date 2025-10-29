

# 01 — Allocazione Dinamica

## 1.1 Parte Teorica
### 1.1.1 Allocazione
Allocare una variabile vuol dire associarvi una porzione di memoria 
Può essere :
-   implicita : automatica e statica (non cambia) gestita dal sistema
-   esplicita : specificata dal programmatore
-   dinamica : permette di cambiare la dimensione della struttura dati "al volo"

### 1.1.2 Variabili
Le variabili sono soggette a precise regole di:
- esistenza
- memoria
- visibilità

Le variabili possono essere:
- globali : definite fuori dalle funzioni, sono quindi permanenti e visibili dovunque nel file (generalmente messe nell'intestazione)
    - Vantaggi : accessibili a tutte le funzioni, semplici da usare
    - Svantaggi : rendono il codice meno modulabile, leggibile e affidabile
- locali : definite all'interno delle funzioni, sono visibili solo nelle funzioni in cui sono dichiarate e sono temporanee
L'utilizzo di uno o dell'altro dipende dalla casistica bisogna sempre considerare la visibilità che vogliamo abbia una variabile


### 1.1.3 Ruoli degli strumenti 
-   Compilatore: è il programma che esegue l'analisi (lessicale,sintattica e semantica) del codice sorgente generando un codice oggetto(che contiene riferimenti a funzione di libreria) in linguaggio macchina
-   Linker: individua i collegamenti a funzioni di libreria e riferimenti tra più file oggetti e li unisce. Genera il codice eseguibile
-   Loader: modulo del sistema operativa che carica in memoria centrale il codice eseguibile

Come viene caricato un programma in memoria?
Quando si esegue un programma, il sistema operativo lo carica in memoria creando un processo e dividendo la memoria in varie aree:
- Codice (text): istruzioni del programma
- Dati (data/bss): variabili globali e statiche
- Stack: variabili locali (allocazione automatica)
- Heap: memoria dinamica (allocata con malloc, new, ecc.)
Le variabili locali vengono create automaticamente nello stack all’ingresso di una funzione e distrutte alla sua uscita.

### 1.1.4 Regole di allocazione automatica 
Le variabili globali e locali hanno una dimensione nota
Quindi anche vettori e matrici devono avere una dimensione calcolabile

Variabili locali:
    - vengono raggruppate in uno stack frame
    - allocate nello stack ad ogni chiamata della funzione e deallocate automaticamente all'uscita della funzione
    - se ci mettiamo davanti static diventano sostanzialmente globali
Variabili globali:
    - allocate all'avvio del programma
    - restano in vita per tutto il programma



### 1.1.5 Regole di allocazione dinamica
La memoria in C viene allocate dinamicamente tramite la funzione malloc. 
Per usare la malloca dobbiamo includere <stdlib.h> 

```
void *malloc (size_t size);
``` 
    - size è il numero di byte da allocate
    - il valore di ritorno è un puntatore che conteiene l'indirizzo iniziale della memoria allocate (void in questo caso)



```
codice
``` 




## 1.1 Spiegazione Esercizi
In questa sezione ci saranno esercizi per capire i concetti teorici applicati, l'ideale è mettere a sinistra questo testo e a destra il file aperto con la soluzione

### 1.1.1a Esercizio: Ricorsione e Condizione di Terminazione (Par. 1 & 2)  `es01.c`
Scr
### 1.1.1b Spiegazione dell'esercizio
- Ri 