#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ======================================================================================
 * NOME FUNZIONE: eraseDuplicate
 * ======================================================================================
 * SCOPO:
 * Rimuove i caratteri duplicati da una stringa, mantenendo solo la prima occorrenza
 * di ciascun carattere. La modifica avviene direttamente sulla stringa passata
 * (in-place), preservando l'ordine relativo dei primi ritrovamenti.
 *
 * PARAMETRI:
 * * str : (char *) Puntatore alla stringa C (array di char terminato da '\0')
 * che deve essere modificata.
 *
 * CONCETTI CHIAVE:
 * 1. Modifica In-Place: Non usiamo malloc per creare una stringa risultato. Usiamo
 * la stessa memoria della stringa originale. Poiché il risultato sarà sempre
 * uguale o più corto dell'originale, non rischiamo overflow.
 * 2. Due Indici (Pattern Read/Write): Usiamo un indice 'i' che scorre tutta la
 * stringa (lettura) e un indice 'k' che avanza solo quando troviamo un carattere
 * nuovo (scrittura).
 * 3. Lookup Table (Tabella ASCII): Per verificare istantaneamente (complessità O(1))
 * se un carattere è già stato visto, usiamo un array di appoggio 'visti' di 256
 * interi (uno per ogni possibile valore di un char esteso). Questo evita cicli
 * annidati che renderebbero l'algoritmo lento.
 * ======================================================================================
 */
void eraseDuplicate(char *str) {

    /* --- 1. CONTROLLO DI SICUREZZA --- */
    /*
     * È buona norma difensiva controllare se il puntatore passato è NULL.
     * Se non puntiamo a nulla, terminiamo subito per evitare crash (Segmentation Fault).
     */
    if (str == NULL) {
        return;
    }

    /* --- 2. INIZIALIZZAZIONE STRUTTURE DATI --- */

    /*
     * 'visti' è la nostra tabella di verità.
     * Dimensione 256 copre tutto il set ASCII standard ed esteso.
     * Inizializziamo a {0} che significa "falso" (carattere non ancora visto).
     */
    int visti[256] = {0};

    /*
     * 'k' è l'indice di SCROTTURA (Write Index).
     * Rappresenta la posizione dove andremo a scrivere il prossimo carattere "valido".
     * Inizialmente è 0 perché la nuova stringa parte dall'inizio.
     */
    int k = 0;

    /* --- 3. CICLO DI SCANSIONE (LOGICA PRINCIPALE) --- */

    /*
     * Usiamo 'i' come indice di LETTURA (Read Index).
     * Il ciclo for scorre la stringa finché non incontra il terminatore '\0'.
     * 'str[i]' accede al carattere nella posizione 'i'.
     */
    for (int i = 0; str[i] != '\0'; i++) {

        /*
         * DETTAGLIO SINTATTICO:
         * Effettuiamo un cast a (unsigned char). Questo è importante perché in C
         * il tipo 'char' può essere segnato (negativo). Se str[i] fosse -10,
         * accedere a visti[-10] causerebbe un errore di memoria.
         * 'unsigned char' garantisce un indice tra 0 e 255.
         */
        unsigned char carattereCorrente = (unsigned char)str[i];

        /*
         * Verifichiamo nella nostra tabella se il valore in quella cella è 0.
         * 0 = Mai visto prima.
         * 1 = Già visto (duplicato).
         */
        if (visti[carattereCorrente] == 0) {
            
            /* --- 4. GESTIONE CARATTERE NUOVO --- */

            /*
             * 1. Copiamo il carattere dalla posizione di lettura 'i'
             * alla posizione di scrittura 'k'.
             * Nota: Se i e k sono uguali (inizio stringa), copiamo su se stesso
             * (operazione innocua). Se abbiamo saltato duplicati, k sarà minore di i.
             */
            str[k] = str[i];

            /*
             * 2. Avanziamo l'indice di scrittura 'k' per prepararlo al prossimo
             * carattere valido. L'operatore ++ postfisso incrementa DOPO l'uso,
             * ma qui è su una riga separata per chiarezza.
             */
            k++;

            /*
             * 3. Marchiamo il carattere come "visto" nella tabella.
             * Da ora in poi, se incontreremo questo carattere, l'if sarà falso.
             */
            visti[carattereCorrente] = 1;
        }
        /*
         * Se visti[...] era 1, entriamo nel ramo else implicito:
         * Non facciamo nulla. 'i' avanzerà col ciclo, ma 'k' resterà fermo.
         * Effetto: il carattere duplicato viene "saltato" e verrà sovrascritto
         * dalle successive scritture.
         */
    }

    /* --- 5. TERMINAZIONE DELLA STRINGA --- */

    /*
     * DETTAGLIO SINTATTICO CRUCIALE:
     * Una stringa in C è definita solo se termina con '\0'.
     * Poiché la nuova stringa (lunga 'k') è probabilmente più corta dell'originale,
     * dobbiamo inserire manualmente il terminatore alla fine dei dati validi.
     * Se dimenticassimo questa riga, la stampa mostrerebbe "spazzatura" residua
     * della vecchia stringa.
     */
    str[k] = '\0';
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 * Scopo: Dimostrare l'uso della funzione con allocazione dinamica corretta.
 * ======================================================================================
 */
int main() {
    /* --- 1. DEFINIZIONE DATI DI PROVA --- */
    /* Usiamo una stringa letterale costante come sorgente */
    const char *input_iniziale = "aa;;;bbbab;";

    printf("--- TEST FUNZIONE eraseDuplicate ---\n");
    printf("Stringa letterale di partenza: \"%s\"\n", input_iniziale);

    /* --- 2. ALLOCAZIONE DINAMICA --- */
    /*
     * GESTIONE MEMORIA (MALLOC):
     * Non possiamo passare "aa;;;bbbab;" direttamente alla funzione eraseDuplicate
     * perché le stringhe tra virgolette sono costanti (read-only) e la funzione
     * tenta di modificarle (provocherebbe crash).
     * Dobbiamo allocare memoria scrivibile nell'HEAP.
     *
     * sizeof(char) è sempre 1, ma lo scriviamo per didattica.
     * strlen(input_iniziale) conta i caratteri visibili.
     * + 1 è FONDAMENTALE per ospitare il terminatore '\0' finale.
     */
    char *stringa_modificabile = (char *)malloc((strlen(input_iniziale) + 1) * sizeof(char));

    /* Controllo se la malloc ha fallito (es. memoria finita) */
    if (stringa_modificabile == NULL) {
        fprintf(stderr, "Errore: Memoria insufficiente.\n");
        return 1;
    }

    /*
     * Copiamo i dati dalla costante alla memoria heap appena allocata.
     * strcpy copia inclusivo del '\0'.
     */
    strcpy(stringa_modificabile, input_iniziale);

    /* --- 3. ESECUZIONE --- */
    printf("Stringa prima della funzione:  \"%s\"\n", stringa_modificabile);
    
    /* Chiamata alla funzione C */
    eraseDuplicate(stringa_modificabile);

    /* --- 4. VERIFICA RISULTATI --- */
    printf("Stringa dopo la funzione:      \"%s\"\n", stringa_modificabile);

    /* Verifica attesa: "a;b" */
    if (strcmp(stringa_modificabile, "a;b") == 0) {
        printf("ESITO: Corretto (la stringa corrisponde all'esempio).\n");
    } else {
        printf("ESITO: Errato.\n");
    }

    /* --- 5. PULIZIA DELLA MEMORIA (FREE) --- */
    /*
     * GESTIONE MEMORIA (FREE):
     * Ogni malloc deve avere una corrispondente free.
     * Se non lo facciamo, creiamo un "Memory Leak" (perdita di memoria).
     * Liberiamo lo spazio puntato da stringa_modificabile.
     */
    free(stringa_modificabile);
    
    /*
     * Buona norma: dopo la free, il puntatore punta ancora al vecchio indirizzo
     * (dangling pointer). Impostarlo a NULL previene usi accidentali successivi.
     */
    stringa_modificabile = NULL;

    return 0;
}