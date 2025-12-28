#include "modello.h"

/**
 * Funzione: carica_dati
 * Scopo: Inizializzare la struttura principale del gioco leggendo da due file diversi.
 * Parametri:
 * - L: dimensione del buffer (probabilmente usata altrove, qui viene solo salvata).
 * - file_grid: nome del file contenente la matrice.
 * - file_bonus: nome del file contenente le sequenze bonus.
 * Ritorna: Un puntatore alla struttura GameData allocata e riempita.
 */
GameData* carica_dati(int L, const char* file_grid, const char* file_bonus) {
    
    // 1. ALLOCAZIONE STRUTTURA PRINCIPALE
    // Chiediamo al sistema operativo spazio sufficiente per una variabile di tipo GameData.
    // Usiamo malloc perché vogliamo che questo oggetto sopravviva anche dopo la fine della funzione.
    GameData *game = (GameData*)malloc(sizeof(GameData));
    
    // Controllo fondamentale: se la RAM è piena, malloc restituisce NULL.
    // In quel caso usciamo dal programma per evitare crash (exit(1)).
    if (!game) exit(1);
    
    // Assegniamo il valore L passato come parametro al campo L della struct.
    game->L = L;

    // --- 2. CARICAMENTO GRIGLIA (FILE 1) ---
    
    // Apriamo il file della griglia in modalità lettura ("r" = read).
    FILE *fp_g = fopen(file_grid, "r");
    
    // Se il file non esiste o non si può aprire, fopen restituisce NULL.
    // perror stampa l'errore specifico (es. "File not found") e poi usciamo.
    if (!fp_g) { perror("Errore file griglia"); exit(1); }
    
    // Leggiamo un intero dal file (la dimensione N della matrice)
    // e lo salviamo all'indirizzo di game->N (&game->N).
    fscanf(fp_g, "%d", &game->N);
    
    // Doppio ciclo for per scorrere righe (i) e colonne (j).
    for(int i=0; i<game->N; i++) {
        for(int j=0; j<game->N; j++) {
            // Leggiamo una stringa (%s) dal file.
            // NOTA IMPORTANTE: Qui non c'è malloc. Questo implica che nella struct GameData
            // 'grid' è definita come matrice statica (es. char grid[MAX][MAX][3]).
            // fscanf legge fino al primo spazio bianco e scrive direttamente nella memoria già riservata.
            fscanf(fp_g, "%s", game->grid[i][j]);
        }
    }
    // Chiudiamo il file per liberare le risorse del sistema operativo.
    fclose(fp_g);

    // --- 3. CARICAMENTO BONUS (FILE 2) ---
    
    // Apriamo il secondo file.
    FILE *fp_b = fopen(file_bonus, "r");
    if (!fp_b) { perror("Errore file bonus"); exit(1); }

    // Leggiamo quanti bonus ci sono in totale.
    fscanf(fp_b, "%d", &game->num_bonus);
    
    // ALLOCAZIONE ARRAY DINAMICO DI BONUS
    // Dobbiamo creare un array di 'num_bonus' elementi di tipo Bonus.
    // Questo array andrà nel campo 'lista_bonus' della struct principale.
    game->lista_bonus = (Bonus*)malloc(game->num_bonus * sizeof(Bonus));

    // Iteriamo su ogni singolo bonus per riempirlo.
    for(int i=0; i<game->num_bonus; i++) {
        
        // Assegniamo un ID progressivo (utile per debugging o logica di gioco).
        game->lista_bonus[i].id = i;
        
        // Leggiamo due interi: lunghezza della sequenza e valore in punti.
        // Li salviamo nei rispettivi campi dell'i-esimo bonus.
        fscanf(fp_b, "%d %d", &game->lista_bonus[i].len_seq, &game->lista_bonus[i].valore);
        
        // --- ALLOCAZIONE DELLA SEQUENZA (La parte complessa) ---
        // Ogni bonus ha una sequenza di stringhe. Dobbiamo allocare un array di PUNTATORI a char.
        // Esempio: se len_seq è 3, allochiamo spazio per 3 puntatori (char*).
        game->lista_bonus[i].sequenza = (char**)malloc(game->lista_bonus[i].len_seq * sizeof(char*));
        
        // Ora, per OGNI elemento della sequenza, dobbiamo allocare lo spazio per la stringa vera e propria.
        for(int k=0; k<game->lista_bonus[i].len_seq; k++) {
            
            // Allocazione della stringa k-esima.
            // TOKEN_LEN è una costante (definita nel .h) che dice quanto è lunga al massimo una stringa (es. "55" o "E9").
            // sizeof(char) è sempre 1, ma lo mettiamo per chiarezza.
            game->lista_bonus[i].sequenza[k] = (char*)malloc(TOKEN_LEN * sizeof(char));
            
            // Finalmente leggiamo la stringa dal file e la mettiamo nella memoria appena allocata.
            fscanf(fp_b, "%s", game->lista_bonus[i].sequenza[k]);
        }
    }
    // Chiudiamo il secondo file.
    fclose(fp_b);
    
    // Ritorniamo l'indirizzo della struttura completa al main.
    return game;
}

/**
 * Funzione: libera_dati
 * Scopo: Liberare (free) tutta la memoria allocata con malloc per evitare Memory Leaks.
 * Regola d'oro: Si libera in ordine INVERSO rispetto all'allocazione (dall'interno verso l'esterno).
 */
void libera_dati(GameData *game) {
    // Controllo di sicurezza: se il puntatore è NULL, non c'è nulla da liberare.
    if(!game) return;

    // 1. LIBERIAMO I BONUS (Parte più interna)
    for(int i=0; i<game->num_bonus; i++) {
        
        // Per ogni bonus, dobbiamo prima liberare le singole stringhe della sequenza.
        for(int k=0; k<game->lista_bonus[i].len_seq; k++) {
            // Libera la stringa k-esima (es. libera la memoria che contiene "55").
            free(game->lista_bonus[i].sequenza[k]);
        }
        
        // Dopo aver liberato tutte le stringhe, liberiamo l'array di puntatori che le teneva insieme.
        // (Libera il vettore 'sequenza').
        free(game->lista_bonus[i].sequenza);
    }
    
    // 2. LIBERIAMO L'ARRAY DI BONUS
    // Ora che il contenuto di ogni bonus è pulito, possiamo liberare l'array che contiene le struct Bonus.
    free(game->lista_bonus);
    
    // NOTA: Non facciamo free(game->grid) perché, come visto sopra, non era allocata con malloc
    // ma era una matrice statica dentro la struct.

    // 3. LIBERIAMO LA STRUTTURA PRINCIPALE
    // Infine, distruggiamo il contenitore generale.
    free(game);
}