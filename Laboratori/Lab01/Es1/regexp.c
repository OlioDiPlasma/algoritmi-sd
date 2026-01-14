#include <stdio.h>
#include <ctype.h>  // Per islower, isupper
#include <string.h> // Per strlen
#include "regexp.h"

/* --- 1. FUNZIONI AUSILIARIE (Interne) --- */

/*
 * Scopo: Verifica se la regex 'r' matcha l'inizio della stringa 's'.
 * Nota: Questa funzione controlla solo se il match avviene ESATTAMENTE
 * a partire dal puntatore 's'.
 * Ritorno: 1 (Vero) se matcha, 0 (Falso) altrimenti.
 */
int controllaMatch(char *s, char *r) {
    
    // Continuiamo finche' la regex non e' terminata
    while (*r != '\0') {
        
        // Se la stringa finisce prima della regex, non puo' esserci match
        if (*s == '\0') {
            return 0;
        }

        /* --- CASO 1: Metacarattere punto '.' --- */
        if (*r == '.') {
            // Il punto matcha qualsiasi carattere.
            // Avanziamo entrambi i puntatori.
            s++;
            r++;
        }
        
        /* --- CASO 2: Metacarattere parentesi quadra '[...]' --- */
        else if (*r == '[') {
            int trovato = 0;
            int negazione = 0;
            
            r++; // Entriamo nella parentesi (saltiamo '[')

            // Verifichiamo se c'e' il simbolo di negazione '^'
            if (*r == '^') {
                negazione = 1;
                r++; // Saltiamo '^'
            }

            // Scansioniamo il contenuto della parentesi fino a ']'
            while (*r != ']' && *r != '\0') {
                if (*s == *r) {
                    trovato = 1; // Trovata corrispondenza nel set
                }
                r++; // Passiamo al prossimo carattere dentro [...]
            }

            // Ora r punta a ']'. Dobbiamo assicurarci che la parentesi sia chiusa.
            if (*r == ']') {
                r++; // Usciamo dalla parentesi, pronti per il prossimo pezzo di regex
            }

            // Logica finale parentesi:
            // Se negazione e' attiva: 'trovato' deve essere 0 per avere successo.
            // Se negazione non e' attiva: 'trovato' deve essere 1.
            if (negazione) {
                if (trovato) return 0; // Fallito perche' il char era nel set proibito
            } else {
                if (!trovato) return 0; // Fallito perche' il char non era nel set richiesto
            }

            s++; // Abbiamo consumato un carattere della sorgente
        }

        /* --- CASO 3: Metacarattere slash '\' (minuscole/maiuscole) --- */
        else if (*r == '\\') {
            r++; // Saltiamo lo slash per vedere cosa c'e' dopo ('a' o 'A')
            
            if (*r == 'a') {
                // Richiede minuscola
                if (!islower(*s)) return 0;
            } 
            else if (*r == 'A') {
                // Richiede maiuscola
                if (!isupper(*s)) return 0;
            } 
            else {
                // Caso non specificato nell'esercizio, ma per robustezza:
                // se c'e' uno slash seguito da altro, trattiamo come letterale o errore.
                // Qui assumiamo match fallito per sicurezza.
                return 0;
            }
            
            r++; // Saltiamo 'a' o 'A' nella regex
            s++; // Consumiamo il carattere nella sorgente
        }

        /* --- CASO 4: Carattere letterale semplice --- */
        else {
            if (*s != *r) {
                return 0; // Non corrispondono
            }
            s++;
            r++;
        }
    }

    // Se il ciclo while finisce perche' *r == '\0', abbiamo matchato tutto
    return 1;
}

/* --- 2. FUNZIONE PRINCIPALE --- */

char *cercaRegexp(char *src, char *regexp) {
    /* * Concetto Chiave: Scorrimento a finestra.
     * Non sappiamo dove inizia il match. Quindi proviamo a chiamare
     * controllaMatch() partendo dal primo carattere di src, poi dal secondo,
     * poi dal terzo, ecc... finche' non troviamo un match o finisce la stringa.
     */

    if (src == NULL || regexp == NULL) return NULL;

    char *p = src;

    // Iteriamo finche' non arriviamo alla fine della stringa sorgente
    while (*p != '\0') {
        
        // Proviamo a vedere se la regex matcha iniziando ESATTAMENTE da qui (p)
        if (controllaMatch(p, regexp)) {
            return p; // Trovato! Ritorniamo l'indirizzo corrente
        }

        // Aritmetica dei puntatori: sposto il puntatore avanti di 1 char
        p++; 
    }

    return NULL; // Nessuna occorrenza trovata
}