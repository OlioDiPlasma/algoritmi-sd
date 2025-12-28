#include <stdio.h>

/* ============================================================================
 * FUNZIONE: my_atof (ASCII to Float)
 * ============================================================================
 * Scopo:
 * Converte una stringa rappresentante un numero reale in un valore float.
 * Gestisce segno (+/-), parte intera, punto decimale e parte frazionaria.
 *
 * Parametri:
 * - s: puntatore alla stringa (array di char) da convertire.
 *
 * Concetti Chiave:
 * - Aritmetica dei caratteri (ASCII):
 * In C, i caratteri sono numeri. '0' ha valore 48, '1' ha 49...
 * Per ottenere il valore numerico di un char 'c', facciamo (c - '0').
 * Esempio: '3' - '0' diventa 51 - 48 = 3 (intero).
 *
 * - Scansione con puntatori:
 * Invece di usare un indice i (s[i]), incrementeremo direttamente il
 * puntatore (s++) per scorrere la stringa. È più "C style" ed efficiente.
 * ============================================================================
 */
float my_atof(char *s) {
    /* --- 1. INIZIALIZZAZIONE --- */
    float risultato = 0.0f; // Accumulatore per il numero finale
    int segno = 1;          // 1 per positivo, -1 per negativo
    
    /* --- 2. GESTIONE DEL SEGNO --- */
    /*
     * Controlliamo il primo carattere puntato da s.
     * La traccia dice che il primo carattere è sempre il segno (+ o -).
     */
    if (*s == '-') {
        segno = -1;
        s++; // Spostiamo il puntatore al prossimo carattere
    } else if (*s == '+') {
        segno = 1;
        s++; // Spostiamo il puntatore al prossimo carattere
    }

    /* --- 3. PARTE INTERA --- */
    /*
     * Ciclo finché il carattere puntato (*s) è una cifra tra '0' e '9'.
     * Ci fermiamo se troviamo il punto '.' o il terminatore di stringa '\0'.
     */
    while (*s >= '0' && *s <= '9') {
        /*
         * LOGICA DI ACCUMULO:
         * Spostiamo le cifre già lette a sinistra (moltiplicando per 10)
         * e aggiungiamo la nuova cifra convertita da ASCII a int.
         */
        risultato = (risultato * 10.0f) + (*s - '0');
        
        s++; // Avanziamo al prossimo carattere
    }

    /* --- 4. GESTIONE DEL PUNTO DECIMALE --- */
    if (*s == '.') {
        s++; // Saltiamo il punto e passiamo alla parte frazionaria
        
        /* --- 5. PARTE DECIMALE --- */
        float divisore = 10.0f; // Iniziamo dai decimi (1/10)
        
        /*
         * Continuiamo a leggere cifre finché ce ne sono.
         * Nota: La traccia garantisce che il formato sia corretto,
         * quindi ci fermeremo solo al terminatore stringa '\0'.
         */
        while (*s >= '0' && *s <= '9') {
            /*
             * Convertiamo la cifra e la dividiamo per il divisore corrente.
             * Esempio: cifre decimali "45"
             * 1. Leggo '4': aggiungo 4/10 = 0.4. Divisore diventa 100.
             * 2. Leggo '5': aggiungo 5/100 = 0.05. Totale parziale .45
             */
            risultato = risultato + (*s - '0') / divisore;
            
            divisore *= 10.0f; // Aumentiamo il divisore (10 -> 100 -> 1000...)
            s++; // Avanziamo al prossimo carattere
        }
    }

    /* --- 6. RISULTATO FINALE --- */
    return risultato * segno;
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * Scopo:
 * Creare stringhe di test come da esempio e verificare la conversione.
 *
 * Gestione Memoria:
 * Qui usiamo stringhe letterali ("..."). Risiedono in un'area di memoria
 * statica (read-only). Non serve malloc/free in questo esercizio specifico
 * perché non stiamo costruendo stringhe dinamicamente, ma solo leggendole.
 */
int main() {
    /* Dati di prova presi direttamente dalla traccia */
    char *test1 = "+123.45";
    char *test2 = "-3.5678";
    char *test3 = "-678.2";

    printf("--- INIZIO TEST my_atof ---\n\n");

    /* Test 1 */
    float val1 = my_atof(test1);
    printf("Stringa: %s\n", test1);
    printf("Float  : %f\n", val1);
    printf("---------------------------\n");

    /* Test 2 */
    float val2 = my_atof(test2);
    printf("Stringa: %s\n", test2);
    printf("Float  : %f\n", val2);
    printf("---------------------------\n");

    /* Test 3 */
    float val3 = my_atof(test3);
    printf("Stringa: %s\n", test3);
    printf("Float  : %f\n", val3);
    printf("---------------------------\n");

    return 0;
}