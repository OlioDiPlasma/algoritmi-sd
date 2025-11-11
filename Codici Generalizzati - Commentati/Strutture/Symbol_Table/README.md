
***Symbol Table***
Struttura dati che associa chiavi a valori, cioè memorizza coppie (chiave, informazione associata).

***Implementazioni :*** 

Importante : in molte implementazioni ci sarà scritto magari bug il motivo è che spesso si assumono strutture con dati ideali senza possibilità di eccezioni
Vuol dire semplicemente che mancano dei controlli ma non sono necessari in una struttura generica

**Direct_Access**
L'indice dell'array è la chiave
Funziona solo con chiavi intere,piccole

**Lista Non Ordinata**
Ogni coppia(key,val) è un nodo di una lista collegata.
Facile da implementare, adatta a insiemi piccoli

**Lista ordinata**
I nodi sono tenuto in ordine di chiave
Permette iterazioni ordinate

**Vettore non ordinato**
Come lista non ordinata ma i dati sono dentro un array
Accesso diretto per indice

**Vettore ordinato**
Le coppie (key,val) sono ordinate per chiave in un array
Ok per ricerche frequenti non per inserimenti frequenti

**HASH TABLE**
Struttura dati che supporti operazioni di inserimento, ricerca e cancellazione in tempo medio molto veloce

Funziona associando chiavi a valori e usando una funzione di hash per calcolare rapidamente la posizione della chiave in un array.

Gestisce collisioni con diverse tecniche

Leggi direttamente il README della hashtable 