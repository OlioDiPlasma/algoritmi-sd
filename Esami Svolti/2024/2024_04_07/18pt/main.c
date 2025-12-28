#include "pronto_soccorso.h"

int main() {
    printf("--- SIMULAZIONE ESAME 04/07/2024 ---\n");

    /* --- 1. CREAZIONE DATI DI TEST (Mock) --- */
    // Simulo la lettura che farebbe caricaDati per l'esempio pagina 3-4
    int N = 6;
    ELENCO elenco;
    elenco.n = N;
    elenco.nomi = malloc(N * sizeof(char*));
    char *names[] = {"A", "B", "C", "D", "E", "F"};
    for(int i=0; i<N; i++) elenco.nomi[i] = strdup(names[i]);

    DISTMATR dist;
    dist.n = N;
    dist.mat = malloc(N * sizeof(int*));
    // Matrice esatta del PDF [cite: 181-186]
    int m_val[6][6] = {
        {0, 2, 5, 4, 2, 6}, // A
        {2, 0, 6, 6, 2, 5}, // B
        {5, 6, 0, 7, 4, 3}, // C
        {4, 6, 7, 0, 6, 3}, // D
        {2, 2, 4, 6, 0, 6}, // E
        {6, 5, 3, 3, 6, 0}  // F
    };
    for(int i=0; i<N; i++) {
        dist.mat[i] = malloc(N * sizeof(int));
        memcpy(dist.mat[i], m_val[i], N * sizeof(int));
    }

    /* --- 2. CONFIGURAZIONE PARAMETRI --- */
    // Testiamo la configurazione specificata nel testo
    SEDI sedi;
    sedi.m = 2;
    sedi.elencoSedi = malloc(2 * sizeof(int));
    sedi.elencoSedi[0] = 0; // Città A (indice 0)
    sedi.elencoSedi[1] = 2; // Città C (indice 2)

    int MAXD = 4;
    int MINS = 2;

    printf("Parametri: MAXD=%d, MINS=%d, Sedi={%s, %s}\n", 
           MAXD, MINS, elenco.nomi[0], elenco.nomi[2]);

    /* --- 3. TEST CHECKSEDI --- */
    printf("\n[TEST 1] Verifica validità sedi (checkSedi)...\n");
    if (checkSedi(dist, sedi, MAXD, MINS)) {
        printf(" -> ESITO: Sedi VALIDE (Corretto).\n");
    } else {
        printf(" -> ESITO: Sedi NON VALIDE (Errore).\n");
    }

    /* --- 4. TEST BESTPART --- */
    printf("\n[TEST 2] Ottimizzazione partizionamento (bestPart)...\n");
    SERVIZI res = bestPart(elenco, dist, sedi, MAXD, MINS);

    if (res.mediaDist == FLT_MAX) {
        printf(" -> Nessuna soluzione trovata.\n");
    } else {
        printf(" -> Soluzione trovata!\n");
        printf(" -> Distanza Media Calcolata: %.2f\n", res.mediaDist);
        printf(" -> Distanza Media Attesa (da testo): 3.25\n"); // 13/4
        
        printf("\nDETTAGLIO ASSEGNAZIONI:\n");
        printf("Città\tAssegnata A\tDistanza\n");
        printf("--------------------------------\n");
        for(int i=0; i<N; i++) {
            // Stampiamo solo se non è sede (per chiarezza output)
            // o marchiamo le sedi
            char *nomeCitta = elenco.nomi[i];
            char *nomeSede = elenco.nomi[res.assegnazioni[i]];
            int d = dist.mat[i][res.assegnazioni[i]];
            
            if(i == res.assegnazioni[i])
                printf("%s\t[SEDE]\t\t-\n", nomeCitta);
            else
                printf("%s\t%s\t\t%d\n", nomeCitta, nomeSede, d);
        }
    }

    /* --- 5. PULIZIA --- */
    liberaTutto(&elenco, &dist, &sedi, &res);
    printf("\nMemoria liberata con successo.\n");

    return 0;
}