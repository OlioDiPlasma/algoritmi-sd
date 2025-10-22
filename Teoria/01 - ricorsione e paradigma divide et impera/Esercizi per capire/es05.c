/*
 * Funzione Ricorsiva (per confronto)
 */
int sum_recursive(int n) {
    if (n <= 0) return 0;
    return n + sum_recursive(n - 1);
}

/*
 * Paragrafo 9: Soluzione Iterativa
 * Sostituiamo la ricorsione con un ciclo.
 */
int sum_iterative(int n) {
    int accumulatore = 0;
    int i;

    /*
     * Questo ciclo fa lo stesso lavoro della ricorsione
     * "in discesa" della versione tail-recursive.
     */
    for (i = 1; i <= n; i++) {
        accumulatore = accumulatore + i;
    }

    return accumulatore;
}