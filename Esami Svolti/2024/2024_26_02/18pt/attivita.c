#include "attivita.h"

/* ========================================================================
   FUNZIONE: activityRead
   SCOPO: Leggere dal file il numero di attività, i dettagli e le precedenze.
   PARAMETRI: 
     - FILE *f: Puntatore al file già aperto in modalità lettura.
   RITORNA: Un puntatore alla struttura ACT inizializzata.
   ========================================================================
*/
ACT activityRead(FILE *f) {
    /* --- 1. ALLOCAZIONE STRUTTURA PRINCIPALE --- */
    ACT a = (ACT)malloc(sizeof(struct activities));
    // Controllo sempre se malloc ha avuto successo
    if (a == NULL) {
        printf("Errore allocazione ACT\n");
        return NULL;
    }

    int nP; // Numero di righe di precedenza
    /* Leggo NA (Numero Attività) e NP (Numero Precedenze) [cite: 73] */
    if (fscanf(f, "%d %d", &a->nAtt, &nP) != 2) return NULL;

    /* --- 2. ALLOCAZIONE VETTORE ATTIVITÀ --- */
    /* Alloco dinamicamente l'array interno di attività */
    a->vettAtt = (Attivita *)malloc(a->nAtt * sizeof(Attivita));
    
    /* --- 3. LETTURA DELLE ATTIVITÀ --- */
    /* Ciclo per leggere le quaterne: nome inizio durata valore [cite: 74] */
    for (int i = 0; i < a->nAtt; i++) {
        fscanf(f, "%s %d %d %d", 
               a->vettAtt[i].nome, 
               &a->vettAtt[i].inizio, 
               &a->vettAtt[i].durata, 
               &a->vettAtt[i].valore);
        
        // Calcolo la fine per comodità nei controlli successivi
        a->vettAtt[i].fine = a->vettAtt[i].inizio + a->vettAtt[i].durata;
        a->vettAtt[i].nVin = 0; // Inizializzo a 0 vincoli
    }

    /* --- 4. LETTURA DELLE PRECEDENZE --- */
    /* Questa parte è delicata. Il formato è: "ActVincolata Prec1 [Prec2]"
       Usiamo fgets per leggere l'intera riga e poi sscanf per parsare i token.
       Questo gestisce il fatto che la seconda precedenza è opzionale.
    */
    char buffer[100];
    char nomeAt[21], vincolo1[21], vincolo2[21];
    
    // Consumo il 'newline' rimasto dopo l'ultima fscanf
    fgets(buffer, 100, f); 

    for (int i = 0; i < nP; i++) {
        if (fgets(buffer, 100, f) != NULL) {
            // Provo a leggere 3 stringhe. sscanf ritorna il numero di elementi letti correttamente.
            int letti = sscanf(buffer, "%s %s %s", nomeAt, vincolo1, vincolo2);
            
            // Cerco l'indice dell'attività vincolata nel mio vettore
            int idx = -1;
            for (int k = 0; k < a->nAtt; k++) {
                if (strcmp(a->vettAtt[k].nome, nomeAt) == 0) {
                    idx = k;
                    break;
                }
            }

            // Se ho trovato l'attività, aggiorno i suoi vincoli
            if (idx != -1) {
                if (letti >= 2) {
                    strcpy(a->vettAtt[idx].vincoli[0], vincolo1);
                    a->vettAtt[idx].nVin++;
                }
                if (letti == 3) {
                    strcpy(a->vettAtt[idx].vincoli[1], vincolo2);
                    a->vettAtt[idx].nVin++;
                }
            }
        }
    }

    return a;
}

/* ========================================================================
   FUNZIONE: checkSelection
   SCOPO: Verificare se un insieme di nomi di attività è valido.
   PARAMETRI:
     - ACT a: La struttura dati completa.
     - char **selected: Array di stringhe (i nomi delle attività scelte).
     - int nsel: Quante attività sono state scelte.
   CONCETTI CHIAVE:
     - Doppi puntatori (char **): `selected` è un vettore di stringhe.
     - Complessità: Questo approccio non è ottimizzato (fa molte ricerche lineari), 
       ma è corretto e didattico.
   ========================================================================
*/
int checkSelection(ACT a, char **selected, int nsel) {
    /* Mappa i nomi selezionati ai loro indici reali nella struct ACT per accesso veloce ai dati */
    int *indices = (int *)malloc(nsel * sizeof(int));
    for (int i = 0; i < nsel; i++) {
        indices[i] = -1;
        for (int k = 0; k < a->nAtt; k++) {
            if (strcmp(selected[i], a->vettAtt[k].nome) == 0) {
                indices[i] = k; // Trovato indice reale
                break;
            }
        }
    }

    /* --- CONTROLLO 1: SOVRAPPOSIZIONE TEMPORALE --- */
    /* Confrontiamo ogni attività scelta con tutte le altre scelte */
    for (int i = 0; i < nsel; i++) {
        for (int j = i + 1; j < nsel; j++) {
            Attivita act1 = a->vettAtt[indices[i]];
            Attivita act2 = a->vettAtt[indices[j]];

            // Due intervalli [start1, end1) e [start2, end2) NON si sovrappongono se:
            // end1 <= start2 OR end2 <= start1.
            // Quindi SI sovrappongono se: !(end1 <= start2 || end2 <= start1)
            // Ovvero: end1 > start2 && end2 > start1
            if (act1.fine > act2.inizio && act2.fine > act1.inizio) {
                free(indices);
                return 0; // Invalid: Sovrapposizione
            }
        }
    }

    /* --- CONTROLLO 2: VINCOLI DI PRECEDENZA --- */
    /* Se ho selezionato l'attività A, e A dipende da B, allora ANCHE B deve essere 
       presente nell'elenco `selected`. Inoltre B deve finire prima che A inizi.
       Nota: Il controllo temporale sopra garantisce già che non si sovrappongano.
       Se A dipende da B, e non si sovrappongono, B deve venire prima di A logicamente.
       Qui verifichiamo solo la *presenza* dei prerequisiti.
    */
    for (int i = 0; i < nsel; i++) {
        Attivita current = a->vettAtt[indices[i]];
        
        // Per ogni vincolo dell'attività corrente
        for (int v = 0; v < current.nVin; v++) {
            char *prereqName = current.vincoli[v];
            int foundPrereq = 0;
            
            // Cerco se il prerequisito è presente nella lista `selected`
            for (int k = 0; k < nsel; k++) {
                if (strcmp(selected[k], prereqName) == 0) {
                    foundPrereq = 1;
                    // Opzionale: verificare che il prerequisito finisca prima dell'inizio
                    // (Ma l'overlap check + logica del problema di solito lo implicano)
                    if (a->vettAtt[indices[k]].fine > current.inizio) {
                         free(indices); return 0; // Prerequisito finisce dopo l'inizio!
                    }
                    break;
                }
            }
            if (!foundPrereq) {
                free(indices);
                return 0; // Invalid: Prerequisito mancante
            }
        }
    }

    free(indices);
    return 1; // Tutto ok
}

/* ========================================================================
   FUNZIONE: bestSelectionR (Ricorsiva) e bestSelection (Wrapper)
   SCOPO: Generare il Power Set (tutti i sottoinsiemi) o usare backtracking
          per trovare la combinazione con profitto massimo.
   CONCETTI CHIAVE:
     - Backtracking: Esploriamo l'albero delle decisioni. Per ogni attività
       possiamo decidere se prenderla (1) o non prenderla (0).
     - Pruning (Potatura): Controlliamo la validità.
   ========================================================================
*/

// Funzione di supporto per ordinare la soluzione finale per tempo di inizio (Bubble Sort semplice)
void sortSolution(ACT a, char **list, int n) {
    for(int i=0; i<n-1; i++){
        for(int j=0; j<n-i-1; j++){
            int idx1=-1, idx2=-1;
            // Recupero gli indici reali per confrontare i tempi
            for(int k=0; k<a->nAtt; k++) {
                if(strcmp(list[j], a->vettAtt[k].nome)==0) idx1=k;
                if(strcmp(list[j+1], a->vettAtt[k].nome)==0) idx2=k;
            }
            if (a->vettAtt[idx1].inizio > a->vettAtt[idx2].inizio) {
                char temp[21];
                strcpy(temp, list[j]);
                strcpy(list[j], list[j+1]);
                strcpy(list[j+1], temp);
            }
        }
    }
}

void bestSelectionR(ACT a, int level, char **currentSol, int currentSize, int currentProfit, 
                    char ***bestSol, int *bestSize, int *maxProfit) {
    
    // CASO BASE: Abbiamo deciso per tutte le attività (level == nAtt)
    if (level == a->nAtt) {
        // Verifichiamo se questa soluzione completa è valida
        if (checkSelection(a, currentSol, currentSize)) {
            // Se è valida e il profitto è migliore del massimo trovato finora
            if (currentProfit > *maxProfit) {
                *maxProfit = currentProfit;
                *bestSize = currentSize;
                
                // Copiamo la soluzione corrente nella soluzione best
                // NOTA: Dobbiamo riallocare le stringhe per bestSol
                for (int i = 0; i < currentSize; i++) {
                    strcpy((*bestSol)[i], currentSol[i]);
                }
            }
        }
        return;
    }

    /* --- RAMO 1: PROVO A INCLUDERE L'ATTIVITÀ CORRENTE (level) --- */
    // Aggiungo il nome alla soluzione temporanea
    strcpy(currentSol[currentSize], a->vettAtt[level].nome);
    
    // Passo ricorsivo: vado al livello successivo, size aumenta, profitto aumenta
    bestSelectionR(a, level + 1, currentSol, currentSize + 1, 
                   currentProfit + a->vettAtt[level].valore, 
                   bestSol, bestSize, maxProfit);

    /* --- RAMO 2: PROVO A ESCLUDERE L'ATTIVITÀ CORRENTE --- */
    // Non faccio nulla su currentSol (sovrascriverò al prossimo giro o ignoro)
    // Il profitto non cambia, la size non cambia
    bestSelectionR(a, level + 1, currentSol, currentSize, 
                   currentProfit, 
                   bestSol, bestSize, maxProfit);
}

void bestSelection(ACT a, char ***bestNomi, int *pNsel, int *maxProfitto) {
    // Allocazione strutture ausiliarie per la ricorsione
    char **currentSol = (char **)malloc(a->nAtt * sizeof(char *));
    *bestNomi = (char **)malloc(a->nAtt * sizeof(char *));
    
    for (int i = 0; i < a->nAtt; i++) {
        currentSol[i] = (char *)malloc(21 * sizeof(char));
        (*bestNomi)[i] = (char *)malloc(21 * sizeof(char));
    }

    *maxProfitto = 0;
    *pNsel = 0;

    // Avvio ricorsione
    bestSelectionR(a, 0, currentSol, 0, 0, bestNomi, pNsel, maxProfitto);

    // Ordino il risultato finale per tempo di inizio come richiesto [cite: 93]
    sortSolution(a, *bestNomi, *pNsel);

    // Pulizia memoria ausiliaria (currentSol)
    for (int i = 0; i < a->nAtt; i++) {
        free(currentSol[i]);
    }
    free(currentSol);
}

void activityFree(ACT a) {
    if (a != NULL) {
        if (a->vettAtt != NULL) free(a->vettAtt);
        free(a);
    }
}