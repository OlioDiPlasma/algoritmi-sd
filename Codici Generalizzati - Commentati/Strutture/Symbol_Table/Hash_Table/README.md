
**Hash Table** 
Ricordiamo che è un implementazione della ST non un ADT

Tecniche principali di gestione delle collisioni :

**Linear Chaining** - liste, molti elementi
Ogni slot dell’array punta a una lista di elementi che collidono sulla stessa posizione.

**Open Addressing** - array, pochi elementi
Tutti gli elementi sono memorizzati direttamente nell’array, senza liste.
Se collisione → si cerca un altro slot secondo una sequenza di probe

-  Double hashing : La distanza tra gli step dipende da una seconda funzione di hash.  Migliore distribuzione, riduce clustering

-  Linear probing: Si scorre l’array uno a uno fino a trovare uno slot libero. Semplice ma può causare clustering (più chiavi finiscono vicine tra loro nello stesso intervallo dell'array causando una concentrazione di collisioni).

-  Quadratic probing: Gli step di ricerca seguono una funzione quadratica. Riduce clustering lineare
