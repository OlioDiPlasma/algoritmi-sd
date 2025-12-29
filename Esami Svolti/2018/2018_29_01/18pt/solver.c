#include "defs.h"

/* --- 1. FUNZIONI DI SUPPORTO --- */

/*
 * Scopo: Convertire una stringa (es. "a3") nel suo indice numerico (es. 3).
 * Parametri:
 * - nomi: array di stringhe dove cercare.
 * - N: dimensione array.
 * - target: stringa da cercare.
 * Concetti Chiave:
 * - strcmp restituisce 0 se le stringhe sono identiche.
 */
int trova_indice(char **nomi, int N, char *target) {
    for (int i = 0; i < N; i++) {
        if (strcmp(nomi[i], target) == 0) return i;
    }
    return -1; // Errore: nome non trovato
}

/* --- 2. ALLOCAZIONE MEMORIA --- */

/*
 * Scopo: Allocare dinamicamente le strutture dati.
 * Concetti Chiave:
 * - Malloc di matrici (int**): Prima alloco il vettore delle righe, poi per ogni riga alloco le colonne.
 * - sizeof(int*): Dimensione di un puntatore (su 64bit è 8 byte).
 * - sizeof(int): Dimensione di un intero (4 byte).
 */
ProblemData* inizializza_dati(int N) {
    ProblemData *pd = (ProblemData*)malloc(sizeof(ProblemData));
    pd->N = N;

    // Allocazione Nomi
    pd->nomi_persone = (char**)malloc(N * sizeof(char*));
    pd->nomi_aziende = (char**)malloc(N * sizeof(char*));
    for(int i=0; i<N; i++) {
        pd->nomi_persone[i] = (char*)malloc(MAX_STR * sizeof(char));
        pd->nomi_aziende[i] = (char*)malloc(MAX_STR * sizeof(char));
    }

    // Allocazione Matrici Preferenze (N righe)
    pd->pref_persone = (int**)malloc(N * sizeof(int*));
    pd->pref_aziende = (int**)malloc(N * sizeof(int*));
    pd->rank_aziende = (int**)malloc(N * sizeof(int*));

    for (int i = 0; i < N; i++) {
        // Ogni riga contiene N interi (le colonne)
        pd->pref_persone[i] = (int*)malloc(N * sizeof(int));
        pd->pref_aziende[i] = (int*)malloc(N * sizeof(int));
        pd->rank_aziende[i] = (int*)malloc(N * sizeof(int));
    }

    // Allocazione vettore matching input
    pd->match_p_input = (int*)malloc(N * sizeof(int));
    // Inizializzo a -1 (nessuno assegnato)
    for(int i=0; i<N; i++) pd->match_p_input[i] = -1;

    return pd;
}

/* --- 3. LETTURA DATI --- */

/*
 * Scopo: Leggere i due file di preferenze e popolare le matrici.
 * Logica:
 * 1. Leggo nome persona (riga 1).
 * 2. Leggo N nomi di aziende (riga 2).
 * 3. Converto i nomi in indici e salvo nella matrice.
 * 4. POPOLO RANK_AZIENDE: Questa è un'ottimizzazione cruciale.
 * Invece di scorrere la lista ogni volta per vedere "chi preferisce chi",
 * salviamo direttamente il rango. Se rank_aziende[a][p] = 0, p è il preferito assoluto di a.
 */
void leggi_preferenze(ProblemData *pd, char *file_persone, char *file_aziende) {
    FILE *fp_p = fopen(file_persone, "r");
    FILE *fp_a = fopen(file_aziende, "r");
    if (!fp_p || !fp_a) { fprintf(stderr, "Errore apertura file preferenze\n"); exit(1); }

    char buffer[MAX_STR];
    int N = pd->N;

    // --- A. Leggiamo Persone ---
    for (int i = 0; i < N; i++) {
        // Leggo il nome della persona i-esima (es "p0")
        fscanf(fp_p, "%s", pd->nomi_persone[i]);
        
        // Leggo le N aziende in ordine di preferenza
        for (int j = 0; j < N; j++) {
            fscanf(fp_p, "%s", buffer);
            // Non posso salvare "a1", devo trovare l'indice temporaneo o gestirlo dopo.
            // NOTA: Qui c'è un problema "uovo-gallina". Non conosciamo ancora i nomi delle aziende
            // perché sono nell'altro file. 
            // SOLUZIONE ROBUSTA: Salviamo tutto come stringhe o facciamo due passaggi. 
            // PER SEMPLICITÀ DIDATTICA: Assumiamo di conoscere i nomi o facciamo un pre-caricamento.
            // Dato che l'esercizio chiede di caricare, caricheremo prima i nomi aziende in un ciclo separato?
            // No, facciamo così: Salviamo i nomi delle aziende man mano che le incontriamo? 
            // No, il testo dice che il secondo file contiene i nomi delle aziende.
            
            // TRUCCO: Leggiamo prima TUTTI i nomi dal file aziende per popolare pd->nomi_aziende, 
            // poi torniamo a leggere le preferenze. Ma facciamo la cosa standard:
            // Leggiamo tutto il file aziende PRIMA per avere i nomi, poi il file persone.
        }
    }
    // Riavvolgo per rileggere correttamente (didatticamente non efficiente ma chiaro)
    rewind(fp_p);
    
    // --- B. Carico Nomi Aziende e loro preferenze ---
    for (int i = 0; i < N; i++) {
        fscanf(fp_a, "%s", pd->nomi_aziende[i]); // Salvo nome azienda "a0", "a1"... all'indice i
        for (int j = 0; j < N; j++) {
            fscanf(fp_a, "%s", buffer); 
            // Qui sto leggendo preferenze aziende verso persone. 
            // Ma non ho ancora caricato i nomi delle persone! 
            // Quindi: dobbiamo caricare PRIMA tutti i nomi (intestazioni) e POI le preferenze.
        }
    }
    rewind(fp_a); 
    // Rileggiamo ancora persone per avere i nomi persone corretti.
    for(int i=0; i<N; i++) {
        fscanf(fp_p, "%s", pd->nomi_persone[i]);
        for(int j=0; j<N; j++) fscanf(fp_p, "%s", buffer); // Salto preferenze per ora
    }
    rewind(fp_p);

    // --- C. ORA HO TUTTI I NOMI, POSSO CARICARE LE PREFERENZE (GLI INDICI) ---
    
    // 1. Preferenze Persone
    for (int i = 0; i < N; i++) {
        fscanf(fp_p, "%s", buffer); // Salto nome persona
        for (int j = 0; j < N; j++) {
            fscanf(fp_p, "%s", buffer);
            int id_az = trova_indice(pd->nomi_aziende, N, buffer);
            pd->pref_persone[i][j] = id_az;
        }
    }

    // 2. Preferenze Aziende
    for (int i = 0; i < N; i++) {
        fscanf(fp_a, "%s", buffer); // Salto nome azienda
        for (int j = 0; j < N; j++) {
            fscanf(fp_a, "%s", buffer);
            int id_pers = trova_indice(pd->nomi_persone, N, buffer);
            pd->pref_aziende[i][j] = id_pers;
            
            // OTTIMIZZAZIONE RANKING
            // Se l'azienda 'i' ha la persona 'id_pers' alla posizione 'j' (0=top),
            // allora rank_aziende[i][id_pers] = j.
            // Minore è j, più l'azienda vuole quella persona.
            pd->rank_aziende[i][id_pers] = j;
        }
    }

    fclose(fp_p);
    fclose(fp_a);
}

void leggi_matching_input(ProblemData *pd, char *file_match) {
    FILE *fp = fopen(file_match, "r");
    if (!fp) { fprintf(stderr, "Errore file matching\n"); exit(1); }
    
    char nome_p[MAX_STR], nome_a[MAX_STR];
    for (int i = 0; i < pd->N; i++) {
        if (fscanf(fp, "%s %s", nome_p, nome_a) != 2) break;
        int id_p = trova_indice(pd->nomi_persone, pd->N, nome_p);
        int id_a = trova_indice(pd->nomi_aziende, pd->N, nome_a);
        
        // Assegno nel vettore del matching
        if(id_p != -1 && id_a != -1)
            pd->match_p_input[id_p] = id_a;
    }
    fclose(fp);
}

/* --- 4. VERIFICA STABILITÀ (Richiesta dell'esercizio) --- */

/*
 * Scopo: Verificare se il matching in input è perfetto.
 * Algoritmo:
 * Iteriamo su TUTTE le possibili coppie (p, a) che NON sono nel matching (coppie alternative).
 * Se esiste una coppia (p, a) tale che:
 * 1. p preferisce a rispetto al suo attuale partner
 * AND
 * 2. a preferisce p rispetto al suo attuale partner
 * ALLORA è instabile.
 * * Complessità: O(N^2) perché iteriamo su tutte le coppie possibili.
 */
int verifica_stabilita(ProblemData *pd) {
    int N = pd->N;
    printf("\n--- VERIFICA STABILITA' DEL MATCHING FORNITO ---\n");
    
    int instabile = 0;

    // Itero su tutte le persone p
    for (int p = 0; p < N; p++) {
        int partner_corrente_di_p = pd->match_p_input[p];
        
        // Itero su tutte le aziende a
        for (int a = 0; a < N; a++) {
            
            // Se (p, a) sono già insieme, non sono una coppia "alternativa", salto.
            if (partner_corrente_di_p == a) continue;

            // --- CONTROLLO CONDIZIONE 1: p preferisce a? ---
            // Devo vedere se 'a' appare PRIMA di 'partner_corrente_di_p' nella lista di p.
            int preferisce_a = 0;
            // Scorro la lista delle preferenze di p
            for (int k = 0; k < N; k++) {
                int azienda_k = pd->pref_persone[p][k];
                if (azienda_k == a) {
                    preferisce_a = 1; // Ho trovato 'a' prima del partner corrente
                    break;
                }
                if (azienda_k == partner_corrente_di_p) {
                    preferisce_a = 0; // Ho trovato il partner corrente prima di 'a'
                    break;
                }
            }

            // --- CONTROLLO CONDIZIONE 2: a preferisce p? ---
            // Qui uso la matrice rank_aziende per farlo in O(1)!
            // Devo trovare chi è il partner attuale dell'azienda a.
            // Nota: pd->match_p_input è indicizzato per persona. Devo cercare chi ha 'a'.
            // Questo renderebbe il tutto O(N^3). Per evitarlo, costruiamo l'inverso al volo o cerchiamo.
            // Per didattica, facciamo la ricerca lineare del partner di 'a'.
            int partner_corrente_di_a = -1;
            for(int x=0; x<N; x++) {
                if (pd->match_p_input[x] == a) {
                    partner_corrente_di_a = x;
                    break;
                }
            }

            // Uso i rank: valore più basso = preferenza più alta
            int rank_p = pd->rank_aziende[a][p];
            int rank_curr = pd->rank_aziende[a][partner_corrente_di_a];

            if (preferisce_a && (rank_p < rank_curr)) {
                printf("Instabilita' trovata: Persona %s vorrebbe %s (e viceversa!)\n", 
                       pd->nomi_persone[p], pd->nomi_aziende[a]);
                instabile = 1;
                // Non faccio 'return 0' subito perché magari voglio vedere tutte le instabilità
            }
        }
    }

    if (instabile) return 0; // Non perfetto
    return 1; // Perfetto
}

/* --- 5. ALGORITMO COMPLETO (Gale-Shapley) --- */

/*
 * Scopo: Trovare un matching stabile garantito.
 * Algoritmo: "Proposte delle persone"
 * Finché c'è una persona libera che non ha proposto a tutti:
 * 1. La persona P propone alla sua azienda preferita A non ancora contattata.
 * 2. Se A è libera -> Si fidanzano (temporaneamente).
 * 3. Se A è occupata con P_old:
 * - A confronta P con P_old.
 * - Se A preferisce P -> A scarica P_old (che torna libero) e si mette con P.
 * - Se A preferisce P_old -> A rifiuta P (P proverà con la prossima azienda).
 */
void algoritmo_gale_shapley(ProblemData *pd) {
    int N = pd->N;
    printf("\n--- ESECUZIONE ALGORITMO GALE-SHAPLEY ---\n");

    int *partner_di_azienda = (int*)malloc(N * sizeof(int)); // Chi sta con l'azienda i?
    int *partner_di_persona = (int*)malloc(N * sizeof(int)); // Chi sta con la persona i?
    int *prossima_proposta = (int*)malloc(N * sizeof(int)); // Indice della prossima azienda da provare per persona i
    int *persone_libere = (int*)malloc(N * sizeof(int)); // Coda/Stack di persone libere
    int num_liberi = 0;

    // Inizializzazione
    for (int i = 0; i < N; i++) {
        partner_di_azienda[i] = -1; // Nessuno
        partner_di_persona[i] = -1;
        prossima_proposta[i] = 0;   // Inizia dalla prima preferenza (indice 0)
        persone_libere[num_liberi++] = i; // Tutti liberi all'inizio
    }

    // Ciclo principale
    while (num_liberi > 0) {
        // Prendo una persona libera
        int p = persone_libere[--num_liberi];

        // A chi propone? Alla prossima nella sua lista
        int rank_proposta = prossima_proposta[p]++;
        int a = pd->pref_persone[p][rank_proposta]; // ID dell'azienda

        // Se l'azienda è libera
        if (partner_di_azienda[a] == -1) {
            partner_di_azienda[a] = p;
            partner_di_persona[p] = a;
            // p non è più libero (è stato tolto dallo stack)
        } 
        else {
            // L'azienda 'a' è già occupata con 'p_old'
            int p_old = partner_di_azienda[a];
            
            // Verifichiamo chi preferisce l'azienda usando la matrice rank (O(1))
            if (pd->rank_aziende[a][p] < pd->rank_aziende[a][p_old]) {
                // A preferisce il NUOVO proponente p
                partner_di_azienda[a] = p;
                partner_di_persona[p] = a;
                
                // p_old torna libero
                partner_di_persona[p_old] = -1;
                persone_libere[num_liberi++] = p_old;
            } else {
                // A preferisce il VECCHIO, rifiuta p
                // p torna libero e proverà la prossima azienda al prossimo giro
                persone_libere[num_liberi++] = p;
            }
        }
    }

    // Stampa Risultati
    printf("Matching Perfetto Calcolato:\n");
    for (int i = 0; i < N; i++) {
        int a = partner_di_persona[i];
        printf("(%s, %s)\n", pd->nomi_persone[i], pd->nomi_aziende[a]);
    }

    // Pulizia interna
    free(partner_di_azienda);
    free(partner_di_persona);
    free(prossima_proposta);
    free(persone_libere);
}

/* --- 6. PULIZIA --- */
void libera_tutto(ProblemData *pd) {
    int N = pd->N;
    for(int i=0; i<N; i++) {
        free(pd->nomi_persone[i]);
        free(pd->nomi_aziende[i]);
        free(pd->pref_persone[i]);
        free(pd->pref_aziende[i]);
        free(pd->rank_aziende[i]);
    }
    free(pd->nomi_persone);
    free(pd->nomi_aziende);
    free(pd->pref_persone);
    free(pd->pref_aziende);
    free(pd->rank_aziende);
    free(pd->match_p_input);
    free(pd);
}