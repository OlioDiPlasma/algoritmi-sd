#include "ST.h"

/* METODO MOLTIPLICATIVO */ - float
// k = chiave (float), M = dimensione tabella
// s = estremo inferiore dell'intervallo (es. 0.0)
// t = estremo superiore dell'intervallo (es. 1000.0)
int hash(float k, int M, float s, float t) {
    // (k-s) -> Calcola quanto 'k' è distante dall'inizio dell'intervallo
    // (t-s) -> Calcola la larghezza totale dell'intervallo
    // (k-s)/(t-s) -> Calcola la posizione "percentuale" di k (un float tra 0.0 e 1.0)
    // ... * M -> Scala questa percentuale alla dimensione della tabella (es. 0.75 * 100 = 75)
    // (int)... -> Converte il risultato in un indice intero
    return ((k-s)/(t-s))*M;
}

/* METODO MODULARE */ - interi
// k = chiave (int), M = dimensione tabella
int hash(int k, int M) {
    // Ritorna il RESTO della divisione tra la chiave 'k' e la dimensione 'M'.
    // Il risultato è garantito essere un indice valido (tra 0 e M-1).
    return (k%M);
}

/* METODO DI HORNER */ - stringhe
// v = puntatore alla stringa (char *), M = dimensione tabella
int hash(char *v, int M) { 
    int h = 0, base = 128; // h = accumulatore hash, base = 128 (set di caratteri ASCII)

    // Cicla finché non raggiunge la fine della stringa ('\0')
    for (; *v != '\0'; v++)
        // h = (base * hash_precedente + valore_carattere_corrente) % M
        // Il modulo (%) viene applicato ad ogni passo per evitare
        // che 'h' diventi un numero enorme (overflow).
        h = (base * h + *v) % M;
    
    return h; // Ritorna l'hash finale
}



/* CHIAVI STRINGA CON BASE PRIMA */ - stringhe con base prima
int hash(char *v, int M) { 
    // Identico a prima
    int h = 0, base = 127; // UNICA DIFFERENZA: usa 127 (un numero primo)

    // L'uso di una 'base' prima aiuta a distribuire meglio le chiavi
    // e a ridurre le collisioni, specialmente se 'M' non è primo.
    for (; *v != '\0'; v++)
        h = (base * h + *v) % M;

    return h;
}

/* CHIAVI STRINGA CON HASH UNIVERSALE */ - stringhe
int hashU(char *v, int M) { 
    int h, a = 31415, b = 27183; // h = accumulatore, a/b = costanti "magiche"

    // Cicla su ogni carattere della stringa 'v'
    for (h = 0; *v != '\0'; v++, a = a*b % (M-1))
        // 'h = (a*h + *v) % M;'
        // Simile a Horner, ma il moltiplicatore 'a' non è fisso.
        // 'a' cambia ad ogni iterazione in modo pseudo-casuale
        // (a = a*b % (M-1)). Questo rende l'hash meno prevedibile.
        h = (a*h + *v) % M;

    return h; // Ritorna l'hash finale
}
