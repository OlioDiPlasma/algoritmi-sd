#include <stdio.h>
#include <stdlib.h>

/* ============================================================================
 * FUNZIONE PRIVATA (RECURSIVE KERNEL): combinazioni_r
 * ============================================================================
 * SCOPO:
 * Questa è la funzione "motore" che esegue il lavoro sporco ricorsivo.
 * Costruisce le soluzioni passo dopo passo (Backtracking).
 *
 * PARAMETRI:
 * - vet:        Il vettore originale dei generi dei libri.
 * - n:          Numero totale di libri disponibili.
 * - sol:        Vettore temporaneo che contiene gli INDICI dei libri scelti finora.
 * - k:          Numero di libri da scegliere (profondità della ricorsione).
 * - pos:        Livello attuale della ricorsione (quanti libri ho già scelto).
 * - start:      Indice da cui iniziare a cercare nel vettore 'vet' (per evitare duplicati e permutazioni).
 * - used_genres: Vettore "flag" per tenere traccia dei generi già usati nella soluzione corrente.
 *
 * CONCETTI CHIAVE:
 * 1. Combinazioni Semplici: Per evitare di generare sia (0,1) che (1,0) (che sono lo stesso set di libri),
 * usiamo la variabile 'start'. Ogni livello ricorsivo inizia a cercare DOPO l'ultimo elemento preso.
 * 2. Pruning (Potatura): Prima di prendere un libro, controlliamo se il suo genere è già stato usato.
 * Se sì, saltiamo quel ramo dell'albero di ricerca. Questo rende l'algoritmo efficiente.
 * ============================================================================
 */
void combinazioni_r(int *vet, int n, int *sol, int k, int pos, int start, int *used_genres) {

    /* --- 1. CASO BASE (Terminazione) --- */
    /* Se pos == k, significa che abbiamo riempito il vettore 'sol' con k indici validi.
       Possiamo stampare la soluzione trovata. */
    if (pos == k) {
        printf("(");
        for (int i = 0; i < k; i++) {
            /* Stampiamo l'indice del libro.
               Se non è l'ultimo, aggiungiamo una virgola estetica. */
            printf("%d", sol[i]);
            if (i < k - 1) printf(",");
        }
        printf(") ");
        /* Facciamo un a capo ogni tanto o semplicemente stampiamo in sequenza.
           Per l'esercizio, stampiamo di seguito come nell'esempio. */
        return; 
    }

    /* --- 2. CICLO DI SCANSIONE (Iterazione sui candidati) --- */
    /* Scorriamo i libri disponibili partendo dall'indice 'start' fino alla fine (n). */
    for (int i = start; i < n; i++) {
        
        /* Recuperiamo il genere del libro corrente 'i'.
           SINTASSI: vet[i] accede all'i-esimo intero dell'array. */
        int genere_corrente = vet[i];

        /* --- 3. CONTROLLO VINCOLI (Pruning) --- */
        /* Verifichiamo se questo genere è già stato preso nella soluzione che stiamo costruendo.
           used_genres è un array dove l'indice corrisponde al genere e il valore è 0 (libero) o 1 (preso). */
        if (used_genres[genere_corrente] == 0) {
            
            /* SE IL GENERE È LIBERO: LO PRENDIAMO */

            /* A. Registriamo la mossa */
            sol[pos] = i;                  // Salviamo l'indice del libro nel vettore soluzione
            used_genres[genere_corrente] = 1; // "Marchiamo" il genere come occupato

            /* B. Passo Ricorsivo */
            /* Chiamiamo la funzione per scegliere il prossimo libro (pos + 1).
               IMPORTANTE: passiamo 'i + 1' come nuovo start. Questo assicura che
               non riprenderemo lo stesso libro e andremo sempre avanti (combinazioni, non disposizioni). */
            combinazioni_r(vet, n, sol, k, pos + 1, i + 1, used_genres);

            /* C. Backtracking (Annullamento mossa) */
            /* Quando la chiamata ricorsiva torna indietro, dobbiamo "liberare" il genere
               perché nel prossimo giro del ciclo 'for' (prossima i), quel genere potrebbe
               essere riutilizzato validamente in una combinazione diversa. */
            used_genres[genere_corrente] = 0;
        }
        /* Se il genere era già usato, non facciamo nulla e il ciclo 'for' passa al libro successivo. */
    }
}

/* ============================================================================
 * FUNZIONE PUBBLICA: birthday
 * ============================================================================
 * SCOPO:
 * Funzione wrapper (interfaccia) richiesta dal testo dell'esercizio.
 * Prepara le strutture dati ausiliarie e lancia la ricorsione.
 *
 * PARAMETRI:
 * - vet, n, m, k: Come da specifica del problema.
 *
 * GESTIONE MEMORIA:
 * Qui allochiamo due vettori fondamentali nello Heap:
 * 1. sol: Per contenere gli indici scelti.
 * 2. used_genres: Per marcare i generi usati (funziona come una Hash Map semplice).
 * ============================================================================
 */
void birthday(int *vet, int n, int m, int k) {
    
    /* --- 1. CONTROLLI DI SICUREZZA --- */
    if (k > n || k > m) {
        /* Se voglio più libri di quelli che ho, o più libri di quanti generi esistono,
           è impossibile soddisfare la richiesta. */
        return; 
    }

    /* --- 2. ALLOCAZIONE VETTORE SOLUZIONE --- */
    /* Allochiamo spazio per k interi. */
    int *sol = (int *)malloc(k * sizeof(int));
    if (sol == NULL) {
        printf("Errore malloc sol\n");
        return;
    }

    /* --- 3. ALLOCAZIONE VETTORE MARCATORE GENERI --- */
    /* Dobbiamo tracciare i generi da 1 a m.
       Allochiamo (m + 1) spazi per poter usare direttamente l'indice del genere
       (es. genere 4 va in used_genres[4]). Ignoreremo l'indice 0.
       
       USIAMO calloc INVECE DI malloc:
       Perché? calloc inizializza tutto a 0. È fondamentale che i generi
       risultino inizialmente "non usati" (0). */
    int *used_genres = (int *)calloc((m + 1), sizeof(int));
    if (used_genres == NULL) {
        printf("Errore malloc used_genres\n");
        free(sol); // Ricordarsi di liberare ciò che abbiamo allocato prima!
        return;
    }

    /* --- 4. AVVIO RICORSIONE --- */
    printf("Soluzioni per k=%d: \n", k);
    combinazioni_r(vet, n, sol, k, 0, 0, used_genres);
    printf("\n");

    /* --- 5. PULIZIA MEMORIA --- */
    free(sol);
    free(used_genres);
}

/* ============================================================================
 * MAIN DI TEST
 * ============================================================================
 * SCOPO: Creare i dati dell'Esempio 1 e testare la funzione.
 * ============================================================================
 */
int main() {
    /* DATI ESEMPIO 1 */
    /* vet = (2 1 1 4 3) */
    int n1 = 5;
    int m1 = 4;
    int k1 = 3;
    
    /* Allocazione vettore dati input */
    int *vet1 = (int *)malloc(n1 * sizeof(int));
    vet1[0] = 2; 
    vet1[1] = 1; 
    vet1[2] = 1; 
    vet1[3] = 4; 
    vet1[4] = 3;

    printf("--- TEST ESEMPIO 1 ---\n");
    /* Output atteso: (0,1,3), (0,1,4), (0,2,3), (0,2,4), (0,3,4), (1,3,4), (2,3,4) */
    birthday(vet1, n1, m1, k1);

    /* DATI ESEMPIO 2 */
    /* vet = (1 2 3 1 2 3) */
    int n2 = 6;
    int m2 = 3;
    int k2 = 2;

    int *vet2 = (int *)malloc(n2 * sizeof(int));
    vet2[0] = 1; vet2[1] = 2; vet2[2] = 3;
    vet2[3] = 1; vet2[4] = 2; vet2[5] = 3;

    printf("\n--- TEST ESEMPIO 2 ---\n");
    birthday(vet2, n2, m2, k2);


    /* --- PULIZIA FINALE --- */
    free(vet1);
    free(vet2);

    return 0;
}