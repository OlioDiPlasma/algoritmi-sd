#include "dipendenti.h"
#include <string.h>

/* --- LETTURA DIPENDENTE --- */
int dipendente_leggi(FILE *fp, dipendente_t *d) {
    /* Restituiamo il risultato della fscanf per capire se la lettura è andata a buon fine.
       La sintassi %s legge fino al primo spazio bianco.
    */
    return fscanf(fp, "%d %s %s %d %d %d %d", 
                  &d->matricola, d->nome, d->cognome, 
                  &d->competenze[0], &d->competenze[1], 
                  &d->competenze[2], &d->competenze[3]);
}

void dipendente_stampa(dipendente_t d) {
    printf("%d %s %s [Op:%d Am:%d Te:%d In:%d]\n", 
           d.matricola, d.nome, d.cognome,
           d.competenze[0], d.competenze[1], d.competenze[2], d.competenze[3]);
}

/* Helper per convertire char in enum/indice */
int char_to_ruolo(char c) {
    switch(c) {
        case 'o': return OPERAIO;
        case 'a': return AMMINISTRATIVO;
        case 't': return TECNICO;
        case 'i': return INFORMATICO;
        default: return -1;
    }
}

char ruolo_to_char(int r) {
    switch(r) {
        case OPERAIO: return 'o';
        case AMMINISTRATIVO: return 'a';
        case TECNICO: return 't';
        case INFORMATICO: return 'i';
        default: return '?';
    }
}