# (Esame 04/07/2024) - Localizzazione Pronto Soccorso

Questo programma risolve un problema di Ricerca Operativa per la localizzazione ottima di sedi di emergenza (Facility Location Problem). Dato un insieme di città e le distanze tra loro, il software verifica la validità delle sedi scelte e calcola l'assegnazione ottima dei pazienti per minimizzare la distanza media.

## Struttura del Progetto

Il codice è organizzato in 3 unità principali più il driver di test:

* **`pronto_soccorso.h`** (Header & Definizioni)
    - [cite_start]**ADT di Prima Classe:** Definisce le strutture dati fondamentali richieste dal testo : `ELENCO` (nomi città), `DISTMATR` (matrice distanze), `SEDI` (configurazione PS), `SERVIZI` (soluzione ottima).
    - Espone i prototipi delle funzioni pubbliche.

* **`gestione_dati.c`** (Input/Output & Memoria)
    - [cite_start]**Caricamento:** Implementa `caricaDati` per leggere dal file il numero di città, i nomi e la matrice delle distanze, gestendo l'allocazione dinamica .
    - **Pulizia:** Implementa `liberaTutto` per deallocare correttamente matrici e stringhe, prevenendo memory leaks.

* **`business_logic.c`** (Algoritmi Core)
    - [cite_start]**Verifica (`checkSedi`):** Controlla se una combinazione di sedi rispetta i vincoli rigidi: copertura di tutte le città entro `MAXD` e carico minimo `MINS` per ogni sede 
    - **Ottimizzazione (`bestPart`):** Implementa un algoritmo di partizionamento ricorsivo (Backtracking) con pruning. [cite_start]Assegna ogni città a una sede valida cercando di minimizzare la distanza media globale 
    - **Validazione (`checkPart`):** Verifica nel caso terminale che l'assegnazione rispetti i vincoli di capacità.

* **`main.c`** (Main Driver)
    - Simola l'esame creando dati di test "hardcoded" (basati sull'esempio del foglio d'esame: 6 città, sedi A e C). //non avevo voglia di fare input apposta va bene così tanto è una lettura fie
    - Esegue la sequenza: Configurazione -> Verifica Sedi -> Ricerca Ottimo -> Stampa Risultati -> Free.

## Visualizzazione File

18pt/
├─ business_logic.c     // Logica ricorsiva e verifica vincoli
├─ gestione_dati.c      // Caricamento file e allocazione dinamica
├─ main.c               // Test con dati dell'esempio d'esame
├─ pronto_soccorso.h    // Header file con struct e prototipi
├─ README.md            // Questo file
└─ tester.exe           // Eseguibile finale


**Per compilare il progetto (assicurarsi di essere nella cartella corretta):**
gcc -Wall -g -o tester main.c .\business_logic.c .\gestione_dati.c 

.\tester.exe
