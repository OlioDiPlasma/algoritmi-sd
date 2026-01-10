#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ======================================================================================
 * FUNZIONE: checkConcat
 * ======================================================================================
 * Scopo:
 * Verificare se due stringhe s1 ed s2 possono essere concatenate.
 * La condizione è che esista un suffisso PROPRIO di s1 che coincide con un prefisso
 * PROPRIO di s2.
 *
 * Parametri:
 * - char *s1: La prima stringa (quella che viene prima).
 * - char *s2: La seconda stringa (quella che viene dopo).
 *
 * Concetti Chiave:
 * - "Proprio": Significa che la sottostringa non può essere uguale all'intera stringa.
 * Quindi la lunghezza dell'overlap (l) deve essere: 1 <= l < min(len1, len2).
 * - Aritmetica dei Puntatori: Per accedere al suffisso di lunghezza 'l' di s1,
 * partiamo dall'indice [len1 - l].
 */
int checkConcat(char *s1, char *s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    
    // Il suffisso/prefisso deve essere minore della stringa stessa.
    // Quindi la lunghezza massima testabile è il minimo tra le due lunghezze meno 1.
    // Se una stringa ha lunghezza 1, min_len è 1, il loop non parte -> return 0 (corretto, no parti proprie).
    int min_len = (len1 < len2) ? len1 : len2;

    // Proviamo tutte le possibili lunghezze di sovrapposizione 'l'
    for (int l = 1; l < min_len; l++) {
        
        // Confrontiamo il suffisso di s1 con il prefisso di s2.
        // Suffisso s1 di lunghezza l inizia a: s1 + (len1 - l)
        // Prefisso s2 di lunghezza l inizia a: s2
        
        // Usiamo strncmp per confrontare esattamente 'l' caratteri.
        // È più pulito che scrivere un ciclo for interno manuale.
        if (strncmp(s1 + (len1 - l), s2, l) == 0) {
            return 1; // Trovato un match valido!
        }
    }

    return 0; // Nessuna sovrapposizione valida trovata
}

/*
 * ======================================================================================
 * FUNZIONE RICORSIVA: perm_sempl (Backtracking)
 * ======================================================================================
 * Scopo:
 * Esplorare ricorsivamente tutte le sequenze possibili di stringhe.
 * È una variante delle permutazioni semplici, ma con un vincolo (Pruning):
 * scendiamo in ricorsione solo se le stringhe si concatenano.
 *
 * Parametri:
 * - char **S: L'insieme delle stringhe disponibili.
 * - int n: Numero totale di stringhe.
 * - int *mark: Array per segnare le stringhe già usate nella catena corrente.
 * - int current_len: Lunghezza della catena che stiamo costruendo.
 * - char *last_str: Puntatore all'ultima stringa aggiunta (per verificare la connessione).
 * - int *max_len: Puntatore alla variabile che tiene traccia del record globale.
 */
void perm_sempl(char **S, int n, int *mark, int current_len, char *last_str, int *max_len) {
    
    // Aggiornamento del massimo (se la catena corrente è la più lunga vista finora)
    if (current_len > *max_len) {
        *max_len = current_len;
    }

    // Proviamo ad aggiungere ogni stringa 'i' non ancora usata
    for (int i = 0; i < n; i++) {
        
        if (mark[i] == 0) { // Se la stringa i non è stata usata
            
            // CONTROLLO DI VALIDITÀ (PRUNING)
            // Possiamo aggiungere S[i] se:
            // 1. È la prima stringa della catena (last_str == NULL)
            // 2. OPPURE se si concatena correttamente con l'ultima inserita
            if (last_str == NULL || checkConcat(last_str, S[i])) {
                
                // DO: Marchiamo e scendiamo
                mark[i] = 1;
                
                perm_sempl(S, n, mark, current_len + 1, S[i], max_len);
                
                // UNDO: Backtracking (togliamo il marchio per i prossimi tentativi)
                mark[i] = 0;
            }
        }
    }
}

/*
 * ======================================================================================
 * FUNZIONE WRAPPER: maxConcatSeq
 * ======================================================================================
 * Scopo:
 * Preparare le strutture dati e lanciare la ricorsione.
 */
int maxConcatSeq(char **S, int n) {
    // Allocazione array di marcatori (inizializzato a 0 con calloc)
    int *mark = (int *)calloc(n, sizeof(int));
    if (mark == NULL) return -1; // Gestione errore

    int max_len = 0;

    // Lanciamo la ricorsione.
    // current_len = 0
    // last_str = NULL (nessuna stringa precedente)
    perm_sempl(S, n, mark, 0, NULL, &max_len);

    free(mark);
    return max_len;
}

/*
 * ======================================================================================
 * MAIN DI TEST
 * ======================================================================================
 */
int main() {
    // Caso di test:
    // "alfa" termina con "fa" (o "a")
    // "faro" inizia con "fa" -> OK (alfa -> faro)
    // "roma" inizia con "ro" -> OK (faro -> roma)
    // "mela" inizia con "me" -> NO
    
    char *dizionario[] = {
        "mela",
        "alfa",
        "roma",
        "faro",
        "anodo" // potrebbe collegarsi a "mela" (no, "la" != "an") o "romano" (roma->ano)
    };
    int n = 5;

    printf("Calcolo massima sequenza concatenabile...\n");
    int ris = maxConcatSeq(dizionario, n);
    
    // Risultato atteso: mela alfa faro roma anodo (Lunghezza 5).
    printf("Lunghezza massima trovata: %d\n", ris);

    return 0;
}