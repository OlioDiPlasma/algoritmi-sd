/*
 * Ritorna 1 se vero, 0 se falso
 */
int tuttiPositivi(int A[], int l, int r) {
    /*
     * Condizione di Terminazione (Caso Base)
     * Se l'intervallo ha un solo elemento, controlliamo quello.
     */
    if (l == r) {
        if (A[l] > 0) {
            return 1; // Vero
        } else {
            return 0; // Falso
        }
    }

    /*
     * 1. Fase "Divide"
     * Dividiamo il vettore a metà.
     */
    int m = (l + r) / 2;

    /*
     * 2. Fase "Impera"
     * Risolviamo ricorsivamente i due sottoproblemi (metà sx e metà dx).
     */
    int sx_ok = tuttiPositivi(A, l, m);
    int dx_ok = tuttiPositivi(A, m + 1, r);

    /*
     * 3. Fase "Combina"
     * La soluzione totale è 'vera' SOLO SE entrambe le metà sono 'vere'.
     * Usiamo l'operatore AND logico (&&).
     */
    return (sx_ok && dx_ok);
}

int main() {
    int A1[] = {5, 10, 2, 8}; // Tutti positivi
    int A2[] = {5, 10, -3, 8}; // Contiene un negativo

    printf("A1 e' tutto positivo? %d\n", tuttiPositivi(A1, 0, 3));
    printf("A2 e' tutto positivo? %d\n", tuttiPositivi(A2, 0, 3));
    return 0;
}