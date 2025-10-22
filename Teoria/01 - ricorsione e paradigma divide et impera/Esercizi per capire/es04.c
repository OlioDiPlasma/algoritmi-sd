/*
 * 1. Funzione NON Tail-Recursive
 */
int sum(int n) {
    // Caso Base
    if (n <= 0) {
        return 0;
    }

    /*
     * L'ultima operazione è l'ADDIZIONE (+).
     * Il computer deve aspettare il ritorno di sum(n-1)
     * e POI sommarci n.
     */
    return n + sum(n - 1);
}

/*
 * 3. Funzione Tail-Recursive (con helper)
 */
int sum_tail_helper(int n, int acc) {
    // Caso Base: quando n=0, il risultato è nell'accumulatore.
    if (n <= 0) {
        return acc;
    }

    /*
     * L'ultima operazione è la CHIAMATA RICORSIVA stessa.
     * Il calcolo (n + acc) viene fatto PRIMA di passare
     * il parametro alla funzione successiva.
     */
    return sum_tail_helper(n - 1, n + acc);
}

// Funzione "wrapper" per la tail-recursion
int sum_tail(int n) {
    return sum_tail_helper(n, 0); // L'accumulatore parte da 0
}