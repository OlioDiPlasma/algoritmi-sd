#include <stdio.h>
#include <string.h>

/*
 * Funzione: strContains
 * Verifica se s2 può essere ottenuta rimuovendo caratteri da s1.
 * In altre parole: verifica se s2 è una "sottosequenza" di s1.
 *
 * Approccio: Iterativo Lineare (Greedy).
 * Scorriamo s1 e cerchiamo i caratteri di s2 nell'ordine in cui appaiono.
 */
int strContains(char *s1, char *s2) {
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    int i1, i2;

    // Inizializziamo entrambi gli indici a 0
    // i1 scorre la stringa sorgente (s1)
    // i2 tiene traccia di quanti caratteri di s2 abbiamo trovato finora
    i1 = 0;
    i2 = 0;

    // Ciclo che scorre tutta la stringa s1
    for (i1 = 0; i1 < l1; i1++) {
        
        // Se abbiamo già trovato tutta s2, potremmo fermarci qui (ottimizzazione).
        // Tuttavia, per fedeltà al codice nell'immagine, lasciamo scorrere il loop
        // o aggiungiamo un controllo di sicurezza per non leggere oltre s2.
        if (i2 < l2) {
            // Se il carattere corrente di s1 coincide con il carattere
            // che stiamo cercando in s2 (s2[i2])
            if (s1[i1] == s2[i2]) {
                // Abbiamo trovato il carattere! Passiamo al prossimo carattere di s2
                i2++;
            }
        }
    }

    // Se l'indice i2 ha raggiunto la lunghezza di s2, significa
    // che abbiamo trovato tutti i caratteri di s2 dentro s1 nell'ordine corretto.
    return (i2 == l2);
}

// --- MAIN DI TEST ---
int main() {
    // Caso 1: s2 è contenuta in s1 (True)
    // Rimuovendo 'u', 't', 'o' da "auto", resta "a". (Esempio banale)
    // Esempio migliore: "tavolo" -> "tvo"
    char *s1_a = "tavolo";
    char *s2_a = "tvo"; // t... v... o... presenti in ordine

    // Caso 2: s2 non è contenuta (False - caratteri mancanti)
    char *s1_b = "tavolo";
    char *s2_b = "tva"; // t... v... a (la 'a' è prima della 'v', quella dopo non c'è)

    // Caso 3: s2 non è contenuta (False - ordine errato)
    char *s1_c = "albero";
    char *s2_c = "rba"; // ci sono tutti, ma l'ordine in s1 è b...r...

    printf("--- Test strContains ---\n");
    
    printf("1. Cerco '%s' in '%s': %s (Atteso: SI)\n", 
           s2_a, s1_a, strContains(s1_a, s2_a) ? "SI" : "NO");

    printf("2. Cerco '%s' in '%s': %s (Atteso: NO)\n", 
           s2_b, s1_b, strContains(s1_b, s2_b) ? "SI" : "NO");

    printf("3. Cerco '%s' in '%s': %s (Atteso: NO)\n", 
           s2_c, s1_c, strContains(s1_c, s2_c) ? "SI" : "NO");

    // Caso limite: stringhe uguali
    printf("4. Stringhe identiche: %s (Atteso: SI)\n", 
           strContains("ciao", "ciao") ? "SI" : "NO");

    return 0;
}