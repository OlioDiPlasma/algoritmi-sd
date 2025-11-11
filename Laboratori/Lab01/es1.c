// Includiamo le librerie necessarie
#include <stdio.h>  // Serve per poter usare 'printf' e 'NULL'
#include <string.h> // Serve per poter usare 'strlen' (calcola lunghezza stringa)
#include <ctype.h>  // Serve per 'islower' (è minuscola?) e 'isupper' (è maiuscola?)

/*
 * Funzione matchCarattere (L'"Operaio")
 * -------------------------------------
 * Controlla se UN carattere 'c' della stringa sorgente
 * corrisponde a UN pezzo di pattern (es. '.', '\a', '[abc]').
 *
 * PARAMETRI:
 * char c: Il singolo carattere della stringa da testare (es. 'B')
 * char *pattern: Un puntatore all'inizio del pezzo di pattern (es. "\Aoto")
 * int *offset: Un puntatore a un intero (passato "per indirizzo")
 * La funzione scrive qui dentro quanto è lungo il pezzo di pattern
 * che ha analizzato (es. 2 per "\A").
 *
 * RITORNA:
 * 1 (vero) se il carattere 'c' matcha il pattern.
 * 0 (falso) se non matcha.
 */
int matchCarattere(char c, char *pattern, int *offset) {
    
    // CASO 1: Il pattern è un punto '.'
    if (pattern[0] == '.') { 
        *offset = 1; // Il pezzo di pattern è lungo 1 (solo il '.')
        return 1;    // Ritorna 1 (vero): il punto matcha QUALSIASI carattere.
    }
    
    // CASO 2: Il pattern inizia con un backslash '\' (carattere speciale)
    if (pattern[0] == '\\') { 
        
        // Sotto-caso 2a: Il pattern è '\a' (minuscola)
        if (pattern[1] == 'a') { 
            *offset = 2;        // Il pezzo di pattern è lungo 2 ('\' e 'a')
            return islower(c);  // Ritorna 1 se 'c' è minuscola, 0 altrimenti.
        }
        
        // Sotto-caso 2b: Il pattern è '\A' (maiuscola)
        if (pattern[1] == 'A') { 
            *offset = 2;        // Il pezzo di pattern è lungo 2 ('\' e 'A')
            return isupper(c);  // Ritorna 1 se 'c' è maiuscola, 0 altrimenti.
        }
        // Nota: se fosse \. o \[, questo codice non li gestisce,
        // ma si potrebbe aggiungere qui.
    }
    
    // CASO 3: Il pattern inizia con '[' (classe di caratteri)
    if (pattern[0] == '[') { 
        int negato = 0; // Flag: 0 = classe normale [abc], 1 = classe negata [^abc]
        int i = 1;      // Indice per scorrere *dentro* le parentesi. Parte da 1 (salta '[').
        
        // Controlla se è una classe negata
        if (pattern[1] == '^') { 
            negato = 1; // Se sì, imposta il flag "negato"
            i = 2;      // E fai partire l'indice da 2 (salta '[' e '^')
        }
        
        int trovato = 0; // Flag: 0 = 'c' non è stato trovato nella classe, 1 = è stato trovato.
        
        // LOOP 1: Cerca il carattere 'c' all'interno di [...]
        while (pattern[i] != ']' && pattern[i] != '\0') { // Continua finché non trovi ']' (o per sicurezza la fine della stringa)
            if (pattern[i] == c) { // Hai trovato il carattere 'c' dentro la classe?
                trovato = 1; // Se sì, imposta il flag
                break;       // Esci subito dal loop (non serve continuare a cercare)
            }
            i++; // Passa al prossimo carattere *dentro* la classe
        }
        
        // LOOP 2: Trova la parentesi ']' chiusa
        // (Questo serve se il LOOP 1 è uscito prima col 'break')
        // Esempio: c='a', pattern="[abcdef]". Il LOOP 1 si ferma su 'a'.
        // Questo LOOP 2 scorre "bcdef" fino ad arrivare alla ']'.
        while (pattern[i] != ']' && pattern[i] != '\0') { 
            i++; // Avanza...
        }
        
        // 'i' ora punta alla ']'. L'offset è 'i' + 1 per includere anche la ']'.
        // Esempio: "[abc]". i=4 (punta a ']'). offset = 5.
        *offset = i + 1; 
        
        // Ora decidiamo cosa restituire
        if (negato) {       // Se la classe era negata (es. [^abc])
            return !trovato; // Restituisci l'INVERSO di 'trovato'.
                            // (Match se NON l'hai trovato)
        }
        return trovato; // Altrimenti (classe normale [abc]), restituisci 'trovato'.
                        // (Match se l'hai trovato)
    }
    
    // CASO 4: Carattere letterale (Default)
    // Se non è '.', non è '\' e non è '[', allora è un carattere normale
    // come 'a', 'b', '7', ecc.
    *offset = 1;             // Il pezzo di pattern è lungo 1.
    return (c == pattern[0]); // Ritorna 1 (vero) solo se 'c' è UGUALE
                             // al carattere del pattern.
}

/*
 * Funzione matchDaPosizione (Il "Controllore")
 * -------------------------------------------
 * Verifica se l'INTERO pattern 'regexp' corrisponde alla stringa 'src'
 * A PARTIRE ESATTAMENTE DALL'INIZIO di 'src'.
 *
 * RITORNA:
 * 1 (vero) se tutto il pattern matcha dall'inizio.
 * 0 (falso) se anche solo un pezzo non matcha.
 */
int matchDaPosizione(char *src, char *regexp) {
    int i = 0; // Indice per scorrere la stringa 'src'
    int j = 0; // Indice per scorrere il pattern 'regexp'
    
    // Cicla finché non abbiamo finito di controllare tutto il pattern
    while (regexp[j] != '\0') { // '\0' è il carattere di fine stringa
        
        // Controllo di sicurezza:
        // Se la stringa 'src' finisce (src[i] == '\0')
        // ma il pattern 'regexp' non è ancora finito (regexp[j] != '\0')...
        if (src[i] == '\0') { 
            return 0; // ...allora non può essere un match. Ritorna 0 (falso).
        }
        
        int offset = 0; // Variabile per recuperare l'offset da matchCarattere
        
        // Chiama l'"Operaio" (matchCarattere)
        // Passagli:
        //   src[i]        -> il carattere corrente della stringa
        //   &regexp[j]    -> il puntatore al pezzo corrente del pattern
        //   &offset       -> l'indirizzo della nostra variabile 'offset'
        //
        // `if (! ... )` significa "Se matchCarattere ritorna 0 (falso)..."
        if (!matchCarattere(src[i], &regexp[j], &offset)) { 
            return 0; // ...allora il match da questa posizione è fallito. Ritorna 0 (falso).
        }
        
        // Se matchCarattere ha avuto successo (ha restituito 1):
        i++;           // Avanza di 1 nella STRINGA (controlliamo sempre 1 carattere alla volta)
        j += offset;   // Avanza nel PATTERN di quanto ci ha detto 'offset' (può essere 1, 2, 5...)
    }
    
    // Se sei uscito dal loop 'while', significa che hai finito il pattern (regexp[j] == '\0')
    // e ogni pezzo ha trovato un match.
    return 1; // Ritorna 1 (vero), match completo!
}

/*
 * Funzione cercaRegexp (Il "Capo Progetto")
 * ---------------------------------------
 * Cerca la PRIMA occorrenza del pattern 'regexp'
 * IN QUALSIASI PUNTO della stringa 'src'.
 *
 * RITORNA:
 * Un puntatore (char *) alla posizione in 'src' dove inizia il match.
 * NULL se non trova nessun match.
 */
char *cercaRegexp(char *src, char *regexp) {
    // Controllo di sicurezza: se i puntatori non sono validi...
    if (src == NULL || regexp == NULL) { 
        return NULL; // ...ritorna NULL (errore).
    }
    
    int len_src = strlen(src); // Calcola la lunghezza della stringa sorgente
    
    // Il "Motore" principale:
    // Prova a far partire il match da ogni possibile posizione 'i' della stringa 'src'
    for (int i = 0; i < len_src; i++) { 
        
        // Chiama il "Controllore" (matchDaPosizione)
        // Passagli:
        //   &src[i]   -> Un puntatore alla stringa 'src' CHE PARTE dalla posizione 'i'
        //              (es. se src="ciao", &src[1] è "iao")
        //   regexp  -> Il pattern da cercare
        //
        // `if ( ... )` significa "Se matchDaPosizione ritorna 1 (vero)..."
        if (matchDaPosizione(&src[i], regexp)) { 
            return &src[i]; // ...HAI TROVATO! Ritorna il puntatore a questa posizione
                            // e interrompi la funzione.
        }
    }
    
    // Se il ciclo 'for' è finito (hai provato tutte le posizioni)
    // e non sei mai uscito con 'return &src[i]',
    // significa che non hai trovato nessun match.
    return NULL; // Ritorna NULL (non trovato).
}

// Funzione main: serve solo per testare il codice.
// Non è richiesta dall'esercizio, ma è utile per provare.
int main() {
    char *risultato; // Una variabile puntatore per salvare il risultato di cercaRegexp
    
    // Test 1: punto (qualsiasi carattere)
    printf("Test 1 - .oto:\n"); 
    risultato = cercaRegexp("voto e noto", ".oto"); // Cerca
    if (risultato) // Controlla se 'risultato' è diverso da NULL
        printf("Trovato: %s\n", risultato); // Stampa "voto e noto" (perché %s stampa DAL puntatore FINO ALLA FINE)
    else 
        printf("Non trovato\n");
    
    // Test 2: classe di caratteri
    printf("\nTest 2 - [mn]oto:\n"); 
    risultato = cercaRegexp("il moto rapido", "[mn]oto"); // Cerca "moto" o "noto"
    if (risultato) // Trova "moto"
        printf("Trovato: %s\n", risultato); // Stampa "moto rapido"
    else 
        printf("Non trovato\n");
    
    // Test 3: classe negata
    printf("\nTest 3 - [^f]oto:\n"); 
    risultato = cercaRegexp("la foto bella", "[^f]oto"); // Cerca "oto" NON preceduto da 'f'
    if (risultato) // Prova "foto". 'f' fallisce [^f]. Non trova altro.
        printf("Trovato: %s\n", risultato);
    else 
        printf("Non trovato\n"); // Stampa "Non trovato"
    
    // Test 4: minuscola
    printf("\nTest 4 - \\aoto:\n"); 
    risultato = cercaRegexp("Voto e voto", "\\aoto"); // Cerca "oto" preceduto da minuscola
    if (risultato) // Salta "Voto" (maiuscola), trova "voto" (minuscola)
        printf("Trovato: %s\n", risultato); // Stampa "voto"
    else 
        printf("Non trovato\n");
    
    // Test 5: maiuscola
    printf("\nTest 5 - \\Aoto:\n"); 
    risultato = cercaRegexp("il Voto finale", "\\Aoto"); // Cerca "oto" preceduto da maiuscola
    if (risultato) // Trova "Voto"
        printf("Trovato: %s\n", risultato); // Stampa "Voto finale"
    else 
        printf("Non trovato\n");
    
    // Test 6: pattern complesso
    printf("\nTest 6 - c[aeiou].o:\n"); 
    risultato = cercaRegexp("il ciao mondo", "c[aeiou].o"); // Cerca 'c' + vocale + qualsiasi + 'o'
    if (risultato) // Trova "ciao"
        printf("Trovato: %s\n", risultato); // Stampa "ciao mondo"
    else 
        printf("Non trovato\n");
    
    // Test 7: nessun match
    printf("\nTest 7 - xyz:\n"); 
    risultato = cercaRegexp("il ciao mondo", "xyz"); // Cerca "xyz"
    if (risultato) // Non lo trova
        printf("Trovato: %s\n", risultato);
    else 
        printf("Non trovato\n"); // Stampa "Non trovato"
    
    return 0; // Termina il programma 'main' con successo
}