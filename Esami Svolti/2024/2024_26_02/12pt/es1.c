#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/*
 * ======================================================================================
 * DEFINIZIONE STRUTTURE DATI
 * ======================================================================================
 * Per risolvere questo problema in modo pulito, dobbiamo definire come sono fatti i nostri dati.
 * Non possiamo usare semplici array, perché ogni parola trovata ha un numero variabile di posizioni.
 */

// 1. Rappresenta una singola coordinata (riga, colonna)
typedef struct {
    int r;
    int c;
} Posizione;

// 2. Rappresenta una parola trovata e tutte le sue occorrenze
typedef struct {
    char *parola;           // La stringa stessa (es: "casa")
    Posizione *posizioni;   // Vettore dinamico delle coordinate dove l'abbiamo trovata
    int num_posizioni;      // Quante volte l'abbiamo trovata (dimensione del vettore sopra)
} ParolaTrovata;

/*
 * ======================================================================================
 * FUNZIONE: paroleTrovate
 * ======================================================================================
 * Scopo:
 * Analizzare una matrice di caratteri, estrarre le parole (sequenze di sole lettere),
 * confrontarle con un elenco di parole cercate e, se presenti, memorizzare le loro coordinate.
 *
 * Parametri:
 * - int NR, int NC: Dimensioni della matrice testo.
 * - char testo[NR][NC]: La matrice contenente il testo da analizzare.
 * - char **elenco: Vettore di stringhe (le parole da cercare).
 * - int np: Numero di parole nell'elenco.
 * - ParolaTrovata **risultato: (OUTPUT) Puntatore al vettore dinamico che creeremo.
 * Usiamo un doppio puntatore perché dobbiamo modificare l'indirizzo del vettore nel main.
 * - int *num_trovate: (OUTPUT) Puntatore a intero per dire al main quante parole uniche abbiamo trovato.
 *
 * Concetti Chiave:
 * - Parsing: Dobbiamo leggere carattere per carattere. Una parola inizia quando troviamo una lettera
 * e finisce quando troviamo un non-alfabetico (o fine riga).
 * - Realloc: Non sappiamo a priori quante parole troveremo né quante volte compariranno.
 * Useremo realloc per allargare la memoria "on demand".
 */
void paroleTrovate(int NR, int NC, char testo[NR][NC], 
                   char **elenco, int np, 
                   ParolaTrovata **risultato, int *num_trovate) {

    // Inizializziamo gli output
    *risultato = NULL; // Il vettore risultante è inizialmente vuoto
    *num_trovate = 0;

    /* --- 1. SCANSIONE DELLA MATRICE --- */
    // Scorriamo la matrice cella per cella.
    // Assumiamo che le parole non vadano a capo (finiscono alla fine della riga).
    
    for (int i = 0; i < NR; i++) {
        for (int j = 0; j < NC; j++) {
            
            // Se troviamo un carattere alfabetico, potrebbe essere l'inizio di una parola
            if (isalpha(testo[i][j])) {
                
                // CONTROLLO INIZIO PAROLA:
                // Una parola inizia se siamo alla colonna 0 OPPURE se il carattere precedente NON è lettera.
                // Esempio: "a casa" -> la 'c' è inizio parola perché prima c'è uno spazio.
                // Esempio: "casa" -> la 'a' (seconda lettera) non è inizio parola.
                bool inizio_parola = (j == 0) || !isalpha(testo[i][j-1]);

                if (inizio_parola) {
                    /* --- 2. ESTRAZIONE PAROLA TEMPORANEA --- */
                    // Abbiamo trovato l'inizio. Ora leggiamo finché non finisce.
                    char buffer[100]; // Buffer temporaneo per la parola (assumiamo max 100 char)
                    int k = 0;     // Indice nel buffer
                    int c_curr = j; // Indice temporaneo colonna

                    // Copiamo finché è lettera e siamo nella riga
                    while (c_curr < NC && isalpha(testo[i][c_curr])) {
                        buffer[k] = testo[i][c_curr];
                        k++;
                        c_curr++;
                    }
                    buffer[k] = '\0'; // Chiudiamo la stringa C

                    /* --- 3. VERIFICA NELL'ELENCO --- */
                    // Controlliamo se la parola estratta (buffer) è presente in 'elenco'
                    bool da_cercare = false;
                    for (int w = 0; w < np; w++) {
                        if (strcmp(elenco[w], buffer) == 0) {
                            da_cercare = true;
                            break;
                        }
                    }

                    if (da_cercare) {
                        /* --- 4. AGGIORNAMENTO RISULTATI (LOGICA COMPLESSA) --- */
                        // Dobbiamo capire: abbiamo già trovato questa parola in precedenza o è la prima volta?
                        
                        int indice_trovato = -1;
                        for (int z = 0; z < *num_trovate; z++) {
                            if (strcmp((*risultato)[z].parola, buffer) == 0) {
                                indice_trovato = z;
                                break;
                            }
                        }

                        if (indice_trovato == -1) {
                            // CASO A: PRIMA VOLTA CHE TROVIAMO QUESTA PAROLA
                            // 1. Allarghiamo il vettore principale dei risultati di 1 slot
                            (*num_trovate)++;
                            *risultato = (ParolaTrovata *)realloc(*risultato, (*num_trovate) * sizeof(ParolaTrovata));
                            
                            // 2. Inizializziamo la nuova struttura nell'ultima posizione
                            indice_trovato = (*num_trovate) - 1;
                            (*risultato)[indice_trovato].parola = strdup(buffer); // Copia dinamica della stringa
                            (*risultato)[indice_trovato].num_posizioni = 0;
                            (*risultato)[indice_trovato].posizioni = NULL;
                        }

                        // CASO B: PAROLA ESISTENTE (o appena creata) -> AGGIUNGIAMO LA POSIZIONE
                        // Ora 'indice_trovato' punta alla struttura corretta. Aggiungiamo la coordinata.
                        ParolaTrovata *pt = &(*risultato)[indice_trovato]; // Puntatore di comodo
                        
                        pt->num_posizioni++;
                        pt->posizioni = (Posizione *)realloc(pt->posizioni, pt->num_posizioni * sizeof(Posizione));
                        
                        // Salviamo le coordinate di inizio parola
                        pt->posizioni[pt->num_posizioni - 1].r = i;
                        pt->posizioni[pt->num_posizioni - 1].c = j;
                    }
                    
                    // IMPORTANTE:
                    // Saltiamo l'indice j in avanti per non rileggere le lettere interne alla parola
                    // j verrà incrementato anche dal for loop, quindi lo portiamo all'ultimo carattere della parola
                    j = c_curr - 1; 
                }
            }
        }
    }
}

/*
 * ======================================================================================
 * FUNZIONE: main (TEST HARNESS)
 * ======================================================================================
 */
int main() {
    /* --- 1. SETUP DATI DI PROVA --- */
    #define R 3
    #define C 10
    
    // Matrice di caratteri (simuliamo testo con punteggiatura)
    // Riga 0: "ciao, miao"
    // Riga 1: "cane..gatto"
    // Riga 2: " miao,cane"
    char testo[R][C] = {
        {'c','i','a','o',',',' ','m','i','a','o'},
        {'c','a','n','e','.','.','g','a','t','t'},
        {' ','m','i','a','o',',','c','a','n','e'}
    };

    // Elenco parole da cercare
    char *elenco[] = {"miao", "cane", "tavolo"};
    int np = 3;

    /* --- 2. CHIAMATA FUNZIONE --- */
    ParolaTrovata *res = NULL;
    int dim_res = 0;

    printf("Inizio ricerca...\n");
    paroleTrovate(R, C, testo, elenco, np, &res, &dim_res);

    /* --- 3. STAMPA RISULTATI --- */
    printf("Trovate %d parole uniche dall'elenco.\n", dim_res);
    for (int i = 0; i < dim_res; i++) {
        printf("Parola: '%s' trovata %d volte in:\n", res[i].parola, res[i].num_posizioni);
        for (int k = 0; k < res[i].num_posizioni; k++) {
            printf("  -> (%d, %d)\n", res[i].posizioni[k].r, res[i].posizioni[k].c);
        }
    }

    /* --- 4. GESTIONE MEMORIA (CLEANUP) --- */
    // La pulizia qui è complessa: è una struttura dentro una struttura.
    for (int i = 0; i < dim_res; i++) {
        free(res[i].parola);     // Liberiamo la stringa (creata con strdup)
        free(res[i].posizioni);  // Liberiamo il vettore delle coordinate
    }
    free(res); // Infine liberiamo il vettore principale

    return 0;
}