#include "strutture.h"

int main() {
    // 1. Apertura File
    FILE *fp = fopen("data.txt", "r");
    if (!fp) { printf("Errore file data.txt\n"); return 1; }

    // 2. Caricamento Dati
    ELENCO elenco = ELENCOload(fp);
    DISTMATR dist = DISTMATRload(fp, elenco->N);
    fclose(fp);

    // 3. Configurazione Parametri (Esempio PDF)
    int MAXD = 4;
    int MINS = 2;
    
    // Scelgo le sedi manualmente come da esempio: A (0) e C (2)
    SEDI sedi = SEDIinit(elenco->N);
    SEDIadd(sedi, 0); 
    SEDIadd(sedi, 2);

    printf("Verifica Sedi {A, C} con MAXD=%d, MINS=%d...\n", MAXD, MINS);

    // 4. Verifica Sedi (Domanda 6)
    if (checkSedi(dist, MAXD, MINS, sedi)) {
        printf("Sedi valide! Avvio ottimizzazione...\n");

        // 5. Algoritmo (Domanda 7)
        SERVIZI sol = bestPart(elenco, dist, sedi, MAXD, MINS);

        // 6. Stampa Risultati
        if (sol->media != -1) {
            printf("\n--- SOLUZIONE OTTIMA ---\n");
            printf("Distanza Media: %.2f\n", sol->media);
            for (int i = 0; i < elenco->N; i++) {
                if (!sedi->is_sede[i]) {
                    printf("Citta %s -> Sede %s (dist: %d)\n", 
                        elenco->nomi[i],
                        elenco->nomi[sol->assegnazioni[i]],
                        dist->mat[i][sol->assegnazioni[i]]);
                }
            }
        } else {
            printf("Nessuna soluzione trovata che rispetti i vincoli.\n");
        }
        
        freeAll(NULL, NULL, NULL, sol); // Free parziale
    } else {
        printf("Le sedi scelte non soddisfano i requisiti iniziali.\n");
    }

    // Cleanup finale
    freeAll(elenco, dist, sedi, NULL);
    return 0;
}