gcc -Wall -o esame Graph.c main.c UF.c

Appunti : la UF non è quella standard è estesa perchè nel calcolo combinatorio, quando scendiamo nella ricorsione facciamo delle scelte (uniamo due insiemi). Quando torniamo indietro (backtrack), dobbiamo annullare quella scelta. Una UF standard non ha il tasto "undo". Qui implementiamo uno stack interno per ricordare le operazioni e annullarle.

Si chiede inoltre di specificare il modello di calcolo combinatorio usato e lo spazio in cui si cercano le soluzioni. Si dica
inoltre se la funzione di ottimizzazione viene applicata ad ogni componente connessa individualmente (unendo
successivamente le soluzioni ottime trovate) oppure su tutto il grafo (le risposta va motivata) : 
Il modello combinatorio adottato è quello delle combinazioni semplici di archi con pruning, dove lo spazio delle soluzioni è costituito dai sottoinsiemi di dimensione $k=V-nCC$ privi di cicli (foreste)
L'ottimizzazione viene eseguita globalmente su tutto il grafo perché il termine $(C_{max} - C_{min})$ della funzione obiettivo dipende da archi che potrebbero appartenere a componenti connesse diverse
Di conseguenza, non è applicabile una strategia greedy (come Kruskal) o per singola componente, rendendo necessaria una ricerca esaustiva ricorsiva sull'insieme totale degli archi.

Risultato testato : ok