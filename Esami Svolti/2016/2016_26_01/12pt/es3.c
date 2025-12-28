#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * FUNZIONE AUSILIARIA: stampaSoluzione
 * ============================================================================
 * SCOPO:
 * Stampa a video la decomposizione trovata nel formato richiesto: "str1" "str2" ...
 *
 * PARAMETRI:
 * - str: La stringa originale completa.
 * - soluzione: Array contenente le lunghezze dei pezzi scelti (es: {2, 2, 5}).
 * - k: Numero di pezzi trovati (dimensione logica di 'soluzione').
 * ============================================================================
 */
void stampaSoluzione(char *str, int *soluzione, int k) {
    int current_idx = 0;
    for (int i = 0; i < k; i++) {
        int len = soluzione[i];
        
        printf("\"");
        /* Stampiamo carattere per carattere il pezzo corrente */
        for (int j = 0; j < len; j++) {
            printf("%c", str[current_idx + j]);
        }
        printf("\" ");
        
        /* Avanziamo l'indice nella stringa originale */
        current_idx += len;
    }
    printf("\n");
}

/* ============================================================================
 * FUNZIONE RICORSIVA (KERNEL): decomponi_rec
 * ============================================================================
 * SCOPO:
 * Motore ricorsivo che tenta di costruire la soluzione passo dopo passo.
 *
 * PARAMETRI:
 * - str:       Puntatore alla stringa (o sottostringa corrente).
 * - num:       Numero di lunghezze disponibili nel vettore 'lungh'.
 * - lungh:     Vettore delle lunghezze permesse.
 * - soluzione: Vettore temporaneo dove salviamo le scelte fatte finora (lunghezze).
 * - k:         Contatore di quanti pezzi abbiamo scelto finora (livello ricorsione).
 *
 * RITORNA:
 * - 1 (true) se è stata trovata una decomposizione valida fino alla fine.
 * - 0 (false) se questo ramo non porta a una soluzione (backtracking).
 *
 * CONCETTI CHIAVE (BACKTRACKING):
 * 1. Scelta: Iteriamo su tutte le lunghezze possibili in 'lungh'.
 * 2. Vincolo: Possiamo prendere un pezzo di lunghezza L solo se la stringa
 * rimanente è lunga almeno L.
 * 3. Ricorsione: Se prendiamo un pezzo, chiamiamo la funzione sulla parte
 * restante della stringa (str + L).
 * 4. Backtrack: Se la chiamata ricorsiva fallisce, annulliamo la scelta (non serve
 * codice esplicito qui, basta sovrascrivere 'soluzione[k]' al prossimo giro)
 * e proviamo la prossima lunghezza.
 * ============================================================================
 */
int decomponi_rec(char *str, int num, int *lungh, int *soluzione, int k) {
    
    /* --- 1. CASO BASE (Successo) --- */
    /* Se la stringa corrente è vuota (raggiunto il terminatore '\0'),
       significa che abbiamo decomposto tutto perfettamente. */
    if (*str == '\0') {
        return 1; 
    }

    /* Calcoliamo la lunghezza rimanente della stringa per i controlli */
    int len_rimanente = strlen(str);

    /* --- 2. CICLO DEI TENTATIVI --- */
    /* Proviamo ogni lunghezza disponibile nel vettore 'lungh' */
    for (int i = 0; i < num; i++) {
        int len_tentativo = lungh[i];

        /* --- 3. CONTROLLO DI FATTIBILITÀ (Pruning) --- */
        /* Posso tagliare un pezzo di lunghezza 'len_tentativo' solo se
           la stringa ha abbastanza caratteri. */
        if (len_rimanente >= len_tentativo) {
            
            /* DO: Registro la mossa */
            soluzione[k] = len_tentativo;

            /* RECURSE: Scendo nel prossimo livello.
               Passo 'str + len_tentativo' per avanzare il puntatore all'inizio
               della prossima sottostringa. */
            if (decomponi_rec(str + len_tentativo, num, lungh, soluzione, k + 1)) {
                return 1; // Trovato! Risaliamo velocemente (non cerchiamo altre soluzioni)
            }

            /* UNDO: Se arrivo qui, la ricorsione ha restituito 0.
               Non devo fare nulla di speciale per "cancellare" soluzione[k],
               verrà sovrascritta alla prossima iterazione del for. */
        }
    }

    return 0; // Nessuna delle lunghezze ha funzionato qui. Backtrack.
}

/* ============================================================================
 * FUNZIONE WRAPPER: decomponi
 * ============================================================================
 * SCOPO:
 * Prepara le strutture dati necessarie (il buffer 'soluzione') e avvia la ricorsione.
 *
 * PARAMETRI:
 * - str, num, lungh: Come da specifica.
 *
 * GESTIONE MEMORIA:
 * Allochiamo un array 'soluzione'. Quanto grande?
 * Nel caso peggiore, decomponiamo la stringa in pezzi da 1 carattere.
 * Quindi la dimensione massima necessaria è pari alla lunghezza di str.
 * ============================================================================
 */
void decomponi(char *str, int num, int *lungh) {
    
    int n = strlen(str);

    /* --- 1. ALLOCAZIONE VETTORE SOLUZIONI --- */
    /* Allochiamo spazio sufficiente per contenere la sequenza di lunghezze.
       sizeof(int) perché memorizziamo interi (es. 2, 5, 7). */
    int *soluzione = (int *)malloc(n * sizeof(int));
    if (soluzione == NULL) {
        printf("Errore malloc\n");
        return;
    }

    printf("Cerco decomposizione per: \"%s\"\n", str);

    /* --- 2. AVVIO RICORSIONE --- */
    /* Partiamo col livello k=0 */
    if (decomponi_rec(str, num, lungh, soluzione, 0)) {
        
        /* Se ritorna 1, dobbiamo stampare la soluzione.
           Ma aspetta: decomponi_rec ritorna solo vero/falso, non "quanto" è profonda.
           
           TRUCCO DIDATTICO:
           Come facciamo a sapere quanti elementi stampare da 'soluzione'?
           Possiamo ricalcolarlo sommando le lunghezze in 'soluzione' finché non coprono 'str',
           oppure (più pulito) modificare decomponi_rec per ritornare la profondità,
           o ancora più semplice: usiamo un ciclo while per stampare finché la somma
           delle lunghezze non eguaglia strlen(str). */
        
        /* Calcoliamo k (numero di pezzi) a posteriori per la stampa */
        int k = 0;
        int somma = 0;
        while (somma < n) {
            somma += soluzione[k];
            k++;
        }
        
        printf("Soluzione trovata: ");
        stampaSoluzione(str, soluzione, k);
    } else {
        printf("Nessuna decomposizione possibile.\n");
    }

    /* --- 3. PULIZIA MEMORIA --- */
    free(soluzione);
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * SCOPO:
 * Esegue i due casi di test presentati nell'immagine.
 * ============================================================================
 */
int main() {
    /* ESEMPIO 1 */
    /* str = "tentativo", num = 3, lungh = {2, 5, 7} */
    char *str1 = "tentativo"; // String literal (in read-only memory, ma va bene, non la modifichiamo)
    int lungh1[] = {2, 5, 7};
    int num1 = 3;

    printf("--- TEST 1 ---\n");
    decomponi(str1, num1, lungh1);
    /* Output Atteso: "te" "nt" "ativo" (perché 2+2+5=9 e matchano le lunghezze) */

    printf("\n");

    /* ESEMPIO 2 */
    /* str = "tentativo", num = 2, lungh = {2, 4} */
    char *str2 = "tentativo";
    int lungh2[] = {2, 4};
    int num2 = 2;

    printf("--- TEST 2 ---\n");
    decomponi(str2, num2, lungh2);
    /* Output Atteso: Nessuna decomposizione (somma lunghezze pari non può fare 9 dispari) */

    return 0;
}