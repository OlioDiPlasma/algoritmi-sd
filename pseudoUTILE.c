


//ALCUNI PSEUDOCIDICI PER CAPIRE

void backtrack(int indice, Stato *stato) {
    
    // 1. SEI ARRIVATO ALLA FINE? (Caso Base)
    // È il momento di controllare se la soluzione costruita è valida o migliore.
    if (indice == FINE) {
        aggiorna_soluzione();
        return;
    }

    // 2. QUALI SONO LE MIE OPZIONI ORA? (Loop delle scelte)
    // Es: provare i colori, provare i secchi, provare a prendere/lasciare
    for (ogni scelta possibile per l'elemento 'indice') {
        
        // 3. È LEGALE? (Pruning)
        if (scelta_valida()) {
            
            // 4. DO (Fai la mossa) -> Cambia lo stato
            applica_scelta(); 

            // 5. RECURSE (Salta nel futuro) -> Delega al prossimo livello
            backtrack(indice + 1, stato);

            // 6. UNDO (Torna al passato) -> Ripristina lo stato
            // Fondamentale: deve essere l'esatto opposto del punto 4.
            annulla_scelta(); 
        }
    }
}


metti_A();
   -> PAUSA (Chiamo funzione per B)
      metti_B();
         -> PAUSA (Chiamo funzione per C)
            metti_C();
            ... finisce C ...
         -> RIPRENDI (Sono in B)
      togli_B(); // UNDO di B
   -> RIPRENDI (Sono in A)
togli_A(); // UNDO di A