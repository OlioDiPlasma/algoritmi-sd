/*
 * FILE: aritmetica.c
 * SCOPO: Implementazione della logica di risoluzione e verifica.
 */

#include "aritmetica.h"

/* --- VARIABILI GLOBALI (STATICHE AL FILE) PER IL BACKTRACKING --- */
/* * CONCETTO CHIAVE: Staticita'
 * Usiamo static per rendere queste variabili visibili solo in questo file.
 * Questo evita di passare troppi parametri nella funzione ricorsiva,
 * rendendo il codice piu' leggibile per gli studenti.
 */
static int mappa[NUM_LETTERE];      /* Mappa lettera ('A') -> cifra (0-9). Valore -1 se non assegnata */
static int cifre_usate[NUM_CIFRE];  /* Array booleano: 1 se la cifra e' gia' usata, 0 altrimenti */
static char lettere_uniche[11];     /* Stringa contenente le lettere distinte trovate */
static int n_uniche = 0;            /* Numero di lettere distinte */

/* =========================================================================
 * FUNZIONI DI UTILITA'
 * ========================================================================= */

/*
 * Funzione: stringa_a_numero
 * Scopo: Converte una stringa (es: "SEND") nel valore numerico corrispondente
 * usando la mappa corrente (es: S=9, E=5... -> 95...).
 * Parametri:
 * - s: la stringa da convertire.
 * Ritorna:
 * - Il valore intero (long per sicurezza) o -1 se la conversione fallisce.
 */
long stringa_a_numero(char *s) {
    long num = 0;
    int i;
    /* --- 1. SCANSIONE STRINGA --- */
    for (i = 0; s[i] != '\0'; i++) {
        /*
         * DETTAGLIO SINTATTICO: Aritmetica dei caratteri
         * s[i] - 'A' ci da' l'indice nell'array mappa (0 per A, 1 per B...).
         * Esempio: se s[i] e' 'C' (ASCII 67) e 'A' e' 65, il risultato e' 2.
         */
        int cifra = mappa[s[i] - 'A'];
        
        /* Controllo validita' */
        if (cifra == -1) return -1; 

        /* Costruiamo il numero posizionale: num * 10 + cifra */
        num = num * 10 + cifra;
    }
    return num;
}

/*
 * Funzione: estrai_lettere
 * Scopo: Identifica tutte le lettere uniche presenti nelle tre stringhe.
 * Parametri:
 * - s1, s2, s3: le tre stringhe input.
 */
void estrai_lettere(char *s1, char *s2, char *s3) {
    int presente[NUM_LETTERE] = {0}; /* Init a 0 */
    int i;
    char *temp[3];
    temp[0] = s1; temp[1] = s2; temp[2] = s3;

    n_uniche = 0;
    
    /* --- 1. CICLO SULLE 3 STRINGHE --- */
    for (int k = 0; k < 3; k++) {
        char *str = temp[k];
        for (i = 0; str[i] != '\0'; i++) {
            int idx = str[i] - 'A';
            if (!presente[idx]) {
                presente[idx] = 1;
                lettere_uniche[n_uniche] = str[i];
                n_uniche++;
            }
        }
    }
    lettere_uniche[n_uniche] = '\0'; /* Terminatore stringa */
}

/* =========================================================================
 * FUNZIONI CORE (18 PUNTI)
 * ========================================================================= */

/* * Funzione: wrapper_backtrack
 * Scopo: Funzione ricorsiva che prova ad assegnare cifre alle lettere.
 * Parametri:
 * - k: indice della lettera corrente in 'lettere_uniche' che stiamo processando.
 * - s1, s2, s3: le stringhe originali per il controllo finale.
 * Ritorna:
 * - 1 se soluzione trovata, 0 altrimenti.
 */
int wrapper_backtrack(int k, char *s1, char *s2, char *s3) {
    
    /* --- 1. CASO BASE (FOGLIA DELL'ALBERO) --- */
    if (k == n_uniche) {
        /* Tutte le lettere hanno una cifra. Verifichiamo la somma */
        long n1 = stringa_a_numero(s1);
        long n2 = stringa_a_numero(s2);
        long n3 = stringa_a_numero(s3);

        /* Controllo vincolo: prima cifra non puo' essere 0 */
        if (mappa[s1[0]-'A'] == 0 || mappa[s2[0]-'A'] == 0 || mappa[s3[0]-'A'] == 0) {
            return 0;
        }

        if (n1 + n2 == n3) {
            printf("\n--- SOLUZIONE TROVATA ---\n");
            printf("%s (%ld) + %s (%ld) = %s (%ld)\n", s1, n1, s2, n2, s3, n3);
            /* Stampa mappatura completa */
            for(int i=0; i<n_uniche; i++) {
                printf("%c = %d, ", lettere_uniche[i], mappa[lettere_uniche[i]-'A']);
            }
            printf("\n");
            return 1; /* Trovato! Interrompiamo la ricerca */
        }
        return 0;
    }

    /* --- 2. PASSO RICORSIVO --- */
    /* Lettera corrente da assegnare */
    char lettera_corr = lettere_uniche[k];
    int idx_mappa = lettera_corr - 'A';

    /* Proviamo tutte le cifre da 0 a 9 */
    for (int cifra = 0; cifra <= 9; cifra++) {
        
        /* Se la cifra non e' usata */
        if (!cifre_usate[cifra]) {
            
            /* --- ASSEGNAZIONE (DO) --- */
            mappa[idx_mappa] = cifra;
            cifre_usate[cifra] = 1;

            /* --- RICORSIONE --- */
            if (wrapper_backtrack(k + 1, s1, s2, s3)) {
                return 1; /* Risalita rapida se trovato */
            }

            /* --- BACKTRACK (UNDO) --- */
            /* Se siamo qui, la strada non era buona. Annulliamo le modifiche */
            cifre_usate[cifra] = 0;
            mappa[idx_mappa] = -1;
        }
    }

    return 0; /* Nessuna soluzione trovata in questo ramo */
}

/*
 * Implementazione Generatore (Modalita' 2)
 */
void risolvi_generatore(char *s1, char *s2, char *s3) {
    /* Inizializzazione strutture */
    for(int i=0; i<NUM_LETTERE; i++) mappa[i] = -1;
    for(int i=0; i<NUM_CIFRE; i++) cifre_usate[i] = 0;

    printf("Analisi stringhe: %s + %s = %s\n", s1, s2, s3);

    /* 1. Trova le lettere uniche */
    estrai_lettere(s1, s2, s3);
    
    if (n_uniche > 10) {
        printf("Errore: Troppe lettere distinte (>10). Impossibile risolvere.\n");
        return;
    }

    printf("Lettere distinte trovate (%d): %s\n", n_uniche, lettere_uniche);
    printf("Avvio calcolo soluzione...\n");

    /* 2. Avvia ricorsione */
    if (!wrapper_backtrack(0, s1, s2, s3)) {
        printf("Nessuna soluzione trovata.\n");
    }
}

/*
 * Implementazione Verificatore (Modalita' 1)
 */
void risolvi_verificatore(char *s1, char *s2, char *s3) {
    /* Reset strutture */
    for(int i=0; i<NUM_LETTERE; i++) mappa[i] = -1;
    estrai_lettere(s1, s2, s3);

    printf("Inserisci la mappatura per le seguenti lettere: %s\n", lettere_uniche);
    printf("Formato: LETTERA CIFRA (es: S 9)\n");

    /* 1. Lettura Input Utente */
    for (int i = 0; i < n_uniche; i++) {
        char let;
        int cif;
        /* * NOTA: " %c" con lo spazio prima serve a ignorare newline o spazi 
         * rimasti nel buffer precedentemente.
         */
        printf("Inserisci associazione per %c: ", lettere_uniche[i]);
        scanf(" %c %d", &let, &cif);

        /* Controlli base */
        if (!isupper(let)) let = toupper(let); /* Forza maiuscolo */
        if (cif < 0 || cif > 9) {
            printf("Errore: Cifra non valida.\n");
            return;
        }
        
        mappa[let - 'A'] = cif;
    }

    /* 2. Verifica Vincoli */
    /* Controllo leading zero */
    if (mappa[s1[0]-'A'] == 0 || mappa[s2[0]-'A'] == 0 || mappa[s3[0]-'A'] == 0) {
        printf("Soluzione ERRATA: La prima lettera di una parola non puo' essere 0.\n");
        return;
    }

    /* 3. Verifica Matematica */
    long n1 = stringa_a_numero(s1);
    long n2 = stringa_a_numero(s2);
    long n3 = stringa_a_numero(s3);

    printf("\nVerifica: %ld + %ld = %ld\n", n1, n2, n3);
    
    if (n1 + n2 == n3) {
        printf("CORRETTO! La soluzione fornita e' valida.\n");
    } else {
        printf("SBAGLIATO! La somma non corrisponde.\n");
    }
}