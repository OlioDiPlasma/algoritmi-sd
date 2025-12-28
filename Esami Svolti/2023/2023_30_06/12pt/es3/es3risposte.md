**Domanda 1 :** "Si giustifichi la scelta del modello combinatorio adottato."

Il problema è stato modellato come un albero di esplorazione delle decisioni (Decision Tree), tipico dei problemi NP-completi di partizionamento.

1. Natura del problema: Ad ogni passo $i$ (corrispondente all'elemento $i$-esimo del vettore), dobbiamo prendere una decisione: a quale degli $x$ sottoinsiemi assegnare l'elemento corrente?
2. Spazio degli Stati: Questo genera uno spazio delle soluzioni esponenziale pari a $x^d$ (dove $x$ è il numero di sottoinsiemi e $d$ la dimensione del vettore).
3. Algoritmo: Si è adottato un approccio Recursive Backtracking (Ricerca in Profondità - DFS). Questo modello permette di scendere in profondità assegnando elementi ai sottoinsiemi e, nel caso si raggiunga una configurazione invalida (somma > target), di "tornare indietro" (backtrack) annullando l'ultima scelta e provando un sottoinsieme alternativo.

Questa scelta è obbligata poiché non esistono algoritmi greedy o formule chiuse che garantiscano la soluzione corretta per il problema della partizione generica.

**Domanda 2 :** "Si descrivano i criteri di pruning adottati o il motivo della loro assenza."

Sono stati adottati criteri di pruning (potatura) fondamentali per ridurre drasticamente il tempo di esecuzione ed evitare l'esplorazione di rami inutili dell'albero:
1. Pruning Globale (Pre-calcolo): Prima di avviare la ricorsione, si verifica se la somma totale degli elementi del vettore (total_sum) è divisibile per il numero di sottoinsiemi $x$. Se total_sum % x != 0, il problema è matematicamente irrisolvibile e l'algoritmo termina immediatamente.
2. Pruning Locale (Overflow del Target): Durante la ricorsione, prima di inserire un elemento in un sottoinsieme, si verifica se somma_corrente + elemento > target. Se la condizione è vera, si evita di scendere in quel ramo ricorsivo, poiché quel sottoinsieme supererebbe la somma consentita, rendendo la soluzione non valida.
3. Simmetria dei Contenitori Vuoti (Opzionale ma raccomandato): Se l'inserimento di un elemento nel sottoinsieme corrente fallisce e tale sottoinsieme era vuoto (somma 0), l'algoritmo interrompe il ciclo sui sottoinsiemi rimanenti. Essendo i sottoinsiemi vuoti tutti indistinguibili tra loro, fallire sul primo vuoto implica che si fallirà su tutti gli altri vuoti identici.