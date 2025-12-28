#ifndef HUFFMAN_H
#define HUFFMAN_H

/* --- DEFINIZIONE TIPO OPACO (ADT I CLASSE) --- */
/*
 * H è un puntatore a una struttura incompleta definita nel .c.
 * Questo nasconde i dettagli implementativi al main.
 */
typedef struct h_wrapper *H;

/* --- PROTOTIPI --- */

/*
 * Funzione: decode
 * Scopo: Decodificare una stringa di '0' e '1' usando l'albero H.
 * Input: L'albero h, la stringa codificata str.
 * Output: Nuova stringa allocata dinamicamente con il testo in chiaro.
 */
char *decode(H h, char *str);

/* * Funzioni di utilità per il test (per costruire e distruggere l'albero) 
 */
H crea_albero_test_hardcoded(); 
void libera_albero(H h);

#endif