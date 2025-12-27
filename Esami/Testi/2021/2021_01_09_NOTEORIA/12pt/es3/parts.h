#ifndef PARTS_H
#define PARTS_H

/* =========================================================================
 * DEFINIZIONE TIPI DATI
 * =========================================================================
 * Rappresentiamo il "pezzo" come richiesto dalla traccia.
 * char *s: la stringa del pezzo (es. "ers")
 * int pos: la posizione OBBLIGATORIA dove questo pezzo deve iniziare nel target.
 * int cost: il costo di utilizzo di questo pezzo.
 */
typedef struct {
    char *s;
    int pos;
    int cost;
} part;

/* =========================================================================
 * PROTOTIPO FUNZIONE WRAPPER
 * =========================================================================
 * SCOPO:
 * Funzione pubblica che prepara le strutture dati ausiliarie (soluzione corrente,
 * soluzione ottima, best_cost, ecc.) e lancia la ricorsione.
 *
 * PARAMETRI:
 * - target: La stringa obiettivo da formare.
 * - P: Il vettore delle parti disponibili.
 * - nParts: La dimensione del vettore P.
 */
void solve(char *target, part *P, int nParts);

#endif