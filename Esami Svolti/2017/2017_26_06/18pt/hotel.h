#ifndef HOTEL_H
#define HOTEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- DEFINIZIONI COSTANTI --- */
#define MAX_STR 21 // 20 caratteri + terminatore '\0'

/* --- STRUTTURE DATI --- */

/* * Struttura per il CLIENTE.
 * Mantiene i dati letti dal file prenotazioni.
 * 'hotel_assegnato' serve per memorizzare la soluzione temporanea durante il calcolo.
 */
typedef struct {
    char id_cliente[MAX_STR];
    int arrivo;
    int pernottamenti;
    int hotel_assegnato; // Indice dell'hotel (0..N-1) o -1 se non assegnato
} Cliente;

/* * Struttura per l'HOTEL.
 * 'M' e' la capacita' totale.
 * 'camere_occupate' e' un vettore dinamico di dimensione X (giorni).
 * Ogni cella [i] dice quante camere sono piene nel giorno i.
 */
typedef struct {
    int id; // 0, 1, 2...
    int M;  // Capacita' massima
    int *camere_occupate_giornaliere; // Vettore allocato dinamicamente (dimensione X)
    int totale_camere_occupate_periodo; // Somma di tutte le occupazioni (per calcolo veloce di r_i)
} Hotel;

/* --- PROTOTIPI DI FUNZIONE --- */

/* Legge le prenotazioni dal file */
Cliente* leggiPrenotazioni(char *filename, int *num_clienti);

/* * Verifica se l'allocazione proposta nel file rispetta i vincoli.
 * Ritorna 1 se valida, 0 altrimenti.
 */
int verificaAllocazione(char *filename, Cliente *clienti, int n_clienti, Hotel *hotels, int n_hotels, int X);

/* * Funzione wrapper per l'ottimizzazione.
 * Prepara le strutture e lancia la ricorsione.
 */
void trovaSoluzioneOttima(Cliente *clienti, int n_clienti, Hotel *hotels, int n_hotels, int X, int a, int b, char *outfile);

/* Libera la memoria allocata */
void liberaMemoria(Cliente *c, Hotel *h, int n_hotels);

#endif