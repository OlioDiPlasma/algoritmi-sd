#include <stdio.h>
#include <string.h>
#include <ctype.h> // Necessario per la funzione tolower()

/*
 * ======================================================================================
 * FUNZIONE: is_vowel
 * ======================================================================================
 * SCOPO:
 * Funzione helper (di supporto) che determina se un singolo carattere è una vocale.
 * Gestisce sia le maiuscole che le minuscole.
 *
 * PARAMETRI:
 * - char c: Il carattere da analizzare.
 *
 * RITORNA:
 * - 1 (vero) se il carattere è una vocale (a, e, i, o, u).
 * - 0 (falso) altrimenti.
 *
 * CONCETTI CHIAVE:
 * - Normalizzazione: Usiamo tolower() per trattare 'A' e 'a' allo stesso modo,
 * riducendo il numero di controlli necessari.
 * ======================================================================================
 */
int is_vowel(char c) {
    /* --- 1. NORMALIZZAZIONE --- */
    /* Convertiamo il carattere in minuscolo per semplificare il confronto.
       'A' diventa 'a', 'b' resta 'b'. */
    char lower_c = tolower(c);

    /* --- 2. VERIFICA --- */
    /* Controlliamo se il carattere è una delle 5 vocali standard. */
    if (lower_c == 'a' || lower_c == 'e' || lower_c == 'i' || 
        lower_c == 'o' || lower_c == 'u') {
        return 1; // Trovata vocale
    }
    
    return 0; // Non è una vocale
}

/*
 * ======================================================================================
 * FUNZIONE: conta_sottostringhe_2_vocali (Corrisponde alla funzione "C" del testo)
 * ======================================================================================
 * SCOPO:
 * Scorre una stringa S e conta quante sottostringhe consecutive di lunghezza 'n'
 * contengono esattamente 2 vocali.
 *
 * PARAMETRI:
 * - char *S: Puntatore al primo carattere della stringa da analizzare.
 * - int n: La lunghezza della sottostringa (finestra) da considerare.
 *
 * RITORNA:
 * - int: Il numero totale di sottostringhe che soddisfano la condizione.
 *
 * CONCETTI CHIAVE:
 * - Sliding Window (Finestra Scorrevole): Non creiamo fisicamente nuove stringhe
 * con malloc. Invece, usiamo indici per guardare una porzione della stringa originale.
 * Questo è molto più efficiente in termini di memoria.
 * - Aritmetica dei limiti: Il ciclo esterno deve fermarsi quando la sottostringa
 * uscirebbe fuori dalla stringa principale.
 * ======================================================================================
 */
int conta_sottostringhe_2_vocali(char *S, int n) {
    /* --- 1. VALIDAZIONE INIZIALE --- */
    /* Se la stringa è NULL o n è negativo/zero, non ha senso procedere. */
    if (S == NULL || n <= 0) {
        return 0;
    }

    /* Calcoliamo la lunghezza totale della stringa per definire i limiti del ciclo.
       NOTA: strlen conta i caratteri fino al terminatore '\0' escluso. */
    int len = strlen(S);

    /* Se la stringa è più corta della finestra n richiesta, 
       non esistono sottostringhe di quella lunghezza. */
    if (len < n) {
        return 0;
    }

    int count_totale = 0; // Contatore delle sottostringhe valide trovate

    /* --- 2. CICLO PRINCIPALE (SCORRIMENTO) --- */
    /*
     * DETTAGLIO SINTATTICO:
     * La condizione (i <= len - n) è CRUCIALE.
     * Immagina S="abcde" (len=5) e n=3.
     * Le sottostringhe iniziano agli indici:
     * i=0 ("abc"), i=1 ("bcd"), i=2 ("cde").
     * Se i andasse oltre 2 (es. 3), cercheremmo di leggere "de..." finendo fuori memoria.
     * (len - n) -> (5 - 3) = 2. Quindi i arriva fino a 2 incluso.
     */
    for (int i = 0; i <= len - n; i++) {
        
        int vocali_nella_finestra = 0;

        /* --- 3. ANALISI SOTTOSTRINGA (LOOP INTERNO) --- */
        /*
         * Qui analizziamo la "finestra" corrente che inizia all'indice 'i'
         * e ha lunghezza 'n'.
         * Usiamo l'indice relativo 'j' che va da 0 a n-1.
         */
        for (int j = 0; j < n; j++) {
            /*
             * DETTAGLIO SINTATTICO:
             * S[i + j] accede al carattere all'offset j rispetto alla posizione di partenza i.
             * Esempio: se i=1 (seconda lettera della stringa principale) e j=0,
             * stiamo leggendo il primo carattere della sottostringa corrente.
             */
            if (is_vowel(S[i + j])) {
                vocali_nella_finestra++;
            }
        }

        /* --- 4. VERIFICA CONDIZIONE --- */
        /* Se abbiamo trovato esattamente 2 vocali in questo blocco di n caratteri... */
        if (vocali_nella_finestra == 2) {
            count_totale++;
        }
    }

    return count_totale;
}

/*
 * ======================================================================================
 * FUNZIONE: main
 * ======================================================================================
 * SCOPO:
 * Ambiente di test (Test Driver) per verificare la correttezza della funzione.
 * Non richiede input utente, usa dati hardcoded come da esempio.
 * ======================================================================================
 */
int main() {
    /* --- 1. DEFINIZIONE DATI DI PROVA --- */
    /* Creiamo la stringa come array di caratteri statico. 
       Il compilatore aggiunge automaticamente '\0' alla fine. */
    char stringa_test[] = "forExample";
    int n = 4;

    printf("--- TEST INIZIO ---\n");
    printf("Stringa S: \"%s\"\n", stringa_test);
    printf("Lunghezza sottostringa n: %d\n", n);

    /* --- 2. CHIAMATA ALLA FUNZIONE --- */
    /* Passiamo l'indirizzo del primo elemento dell'array (stringa_test decade a puntatore) */
    int risultato = conta_sottostringhe_2_vocali(stringa_test, n);

    /* --- 3. STAMPA RISULTATI --- */
    printf("Numero di sottostringhe con esattamente 2 vocali: %d\n", risultato);
    
    /* Verifica rispetto all'esempio del testo */
    if (risultato == 4) {
        printf("ESITO: CORRETTO [Come da esempio nel testo]\n");
    } else {
        printf("ESITO: ERRATO [Atteso 4]\n");
    }

    /* Test aggiuntivo (Caso limite) */
    char test2[] = "aeiou";
    int n2 = 2;
    printf("\n--- TEST AGGIUNTIVO ---\n");
    printf("Stringa: \"%s\", n: %d\n", test2, n2);
    // Sottostringhe len 2: "ae"(2), "ei"(2), "io"(2), "ou"(2) -> Totale 4
    int ris2 = conta_sottostringhe_2_vocali(test2, n2);
    printf("Risultato: %d (Atteso: 4)\n", ris2);

    printf("--- FINE TEST ---\n");

    return 0;
}