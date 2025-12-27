1. Il Modello Combinatorio: Powerset (Insieme delle Parti)

Perché abbiamo scelto questo modello? 
La traccia ci chiede di identificare un insieme di oggetti. Quando dobbiamo selezionare un sottoinsieme da un insieme di $N$ elementi, il modello naturale è il Powerset.Ogni oggetto rappresenta un "livello" dell'albero di ricorsione. A ogni livello, abbiamo una scelta binaria:Includo l'oggetto corrente nella soluzione?Escludo l'oggetto corrente?Questo copre tutte le possibili combinazioni ($2^N$).




3. I Criteri di Pruning (Potatura)
Il Pruning è fondamentale per non esplorare soluzioni inutili. Qui ne abbiamo due:

Costo: tot_c + v[pos].cost > C_max. Se aggiungere l'oggetto mi fa sforare il budget, è inutile prenderlo e vedere cosa succede dopo. Taglio il ramo subito.

Tipo: used_types[v[pos].type] == 1. Se ho già una "Spada" (Tipo 1) nello zaino, la traccia vieta di prenderne un'altra. Taglio il ramo "prendi" e vado direttamente al ramo "lascia".