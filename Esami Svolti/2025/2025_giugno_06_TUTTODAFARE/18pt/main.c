#include "esame.h"

int main() {
    Oggetti oggetti;
    Vincoli vincoli;
    
    // Parametri del mezzo (Modificali qui per testare casi diversi)
    float Pmax = 100.0; 
    float Vmax = 80.0;

    printf("--- INIZIO PROGRAMMA ---\n");

    // 1. Lettura
    leggi_file("oggetti.txt", "vincoli.txt", &oggetti, &vincoli);
    printf("Lettura completata: %d oggetti caricati.\n", oggetti.n_oggetti);

    // 2. Verifica (Domanda 6)
    printf("\n--- VERIFICA VALIDITA' (Domanda 6) ---\n");
    if (verifica_validita(oggetti, vincoli, Pmax, Vmax)) {
        printf("OK: Configurazione valida. Procedo con ottimizzazione.\n");
    } else {
        printf("FAIL: Impossibile trasportare alcuni gruppi (superano Pmax/Vmax).\n");
    }

    // 3. Ottimizzazione (Domanda 7)
    printf("\n--- CALCOLO OTTIMO (Domanda 7) ---\n");
    Soluzione s = calcola_ottimo(oggetti, vincoli, Pmax, Vmax);

    if (s.valida) {
        printf("Soluzione Ottima trovata!\n");
        printf("Giorni totali: %d\n", s.n_giorni_usati);
        printf("Delta Costo: %.2f\n", s.delta_costo);
        
        printf("\nDETTAGLIO PIANO DI CARICO:\n");
        for (int d = 0; d < s.n_giorni_usati; d++) {
            printf(">> Giorno %d:\n", d+1);
            float p_d=0, v_d=0, c_d=0;
            for(int i=0; i<oggetti.n_oggetti; i++) {
                if(s.assegnazioni[i] == d) {
                    printf("   - %s (Peso:%.1f Vol:%.1f Costo:%.1f)\n", 
                           oggetti.vettore[i].nome, 
                           oggetti.vettore[i].peso, 
                           oggetti.vettore[i].volume,
                           oggetti.vettore[i].costo);
                    p_d += oggetti.vettore[i].peso;
                    v_d += oggetti.vettore[i].volume;
                    c_d += oggetti.vettore[i].costo;
                }
            }
            printf("   [TOTALE GIORNO: Peso=%.1f/%.1f, Vol=%.1f/%.1f, Costo=%.1f]\n", p_d, Pmax, v_d, Vmax, c_d);
        }
    } else {
        printf("Nessuna soluzione valida trovata (o vincoli troppo stretti).\n");
    }

    free_tutto(&oggetti, &vincoli, &s);
    return 0;
}