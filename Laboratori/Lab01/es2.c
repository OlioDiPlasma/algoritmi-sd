#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CORSE 1000
#define MAX_STRING 31

// Struttura per rappresentare una data
typedef struct {
    int anno;   // Anno
    int mese;   // Mese dell'anno
    int giorno; // Giorno del mese
} Data;

// Struttura per rappresentare un orario
typedef struct {
    int ore;     // Ore (0-23)
    int minuti;  // Minuti (0-59)
    int secondi; // Secondi (0-59)
} Orario;

// Struttura per rappresentare una corsa
typedef struct {
    char codice_tratta[MAX_STRING];  // Codice identificativo tratta
    char partenza[MAX_STRING];       // Fermata di partenza
    char destinazione[MAX_STRING];   // Fermata di destinazione
    Data data;                       // Data della corsa
    Orario ora_partenza;             // Orario di partenza
    Orario ora_arrivo;               // Orario di arrivo
    int ritardo;                     // Ritardo in minuti
} Corsa;

// Tipo enumerativo per i comandi del menu
typedef enum {
    r_date,        // Elencare corse in intervallo date
    r_partenza,    // Elencare corse da una fermata
    r_capolinea,   // Elencare corse verso un capolinea
    r_ritardo,     // Elencare corse in ritardo in intervallo date
    r_ritardo_tot, // Calcolare ritardo totale per codice tratta
    r_fine,        // Terminare il programma
} comando_e;

// Funzione per leggere le corse dal file
int leggiFile(char *nomeFile, Corsa corse[]) {
    FILE *fp = fopen(nomeFile, "r"); // Apre file in lettura
    if (fp == NULL) { // Controllo apertura file
        printf("Errore apertura file %s\n", nomeFile); // Messaggio errore
        return 0; // Ritorna 0 corse lette
    }
    
    int n; // Numero di corse nel file
    fscanf(fp, "%d", &n); // Legge numero corse
    
    if (n > MAX_CORSE) { // Controllo limiti
        n = MAX_CORSE; // Limita al massimo gestibile
    }
    
    for (int i = 0; i < n; i++) { // Legge ogni corsa
        fscanf(fp, "%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d",
               corse[i].codice_tratta,           // Codice tratta
               corse[i].partenza,                // Fermata partenza
               corse[i].destinazione,            // Fermata arrivo
               &corse[i].data.anno,              // Anno (CORRETTO)
               &corse[i].data.mese,              // Mese
               &corse[i].data.giorno,            // Giorno (CORRETTO)
               &corse[i].ora_partenza.ore,       // Ora partenza
               &corse[i].ora_partenza.minuti,    // Minuti partenza
               &corse[i].ora_partenza.secondi,   // Secondi partenza (AGGIUNTO)
               &corse[i].ora_arrivo.ore,         // Ora arrivo
               &corse[i].ora_arrivo.minuti,      // Minuti arrivo
               &corse[i].ora_arrivo.secondi,     // Secondi arrivo (AGGIUNTO)
               &corse[i].ritardo);               // Ritardo in minuti
    }
    
    fclose(fp); // Chiude il file
    return n; // Ritorna numero corse lette
}

// Funzione per confrontare due date (ritorna -1 se d1<d2, 0 se uguali, 1 se d1>d2)
int confrontaDate(Data d1, Data d2) {
    if (d1.anno < d2.anno) //anni
    return -1;   
else
    return 1;
    if (d1.mese != d2.mese) return (d1.mese < d2.mese) ? -1 : 1; // Confronta mesi
    if (d1.giorno != d2.giorno) return (d1.giorno < d2.giorno) ? -1 : 1; // Confronta giorni
    return 0; // Date uguali
}

// Funzione per stampare una corsa
void stampaCorsa(Corsa c) {
    printf("%s %s->%s %04d/%02d/%02d %02d:%02d:%02d-%02d:%02d:%02d ritardo:%d min\n",
           c.codice_tratta,              // Codice tratta
           c.partenza,                   // Partenza
           c.destinazione,               // Destinazione
           c.data.anno,                  // Anno (CORRETTO)
           c.data.mese,                  // Mese
           c.data.giorno,                // Giorno (CORRETTO)
           c.ora_partenza.ore,           // Ora partenza
           c.ora_partenza.minuti,        // Minuti partenza
           c.ora_partenza.secondi,       // Secondi partenza (AGGIUNTO)
           c.ora_arrivo.ore,             // Ora arrivo
           c.ora_arrivo.minuti,          // Minuti arrivo
           c.ora_arrivo.secondi,         // Secondi arrivo (AGGIUNTO)
           c.ritardo);                   // Ritardo
}

// Funzione per elencare corse in un intervallo di date
void elencaPerDate(Corsa corse[], int n, Data data1, Data data2) {
    int trovate = 0; // Contatore corse trovate
    printf("\nCorse tra %04d/%02d/%02d e %04d/%02d/%02d:\n",
           data1.anno, data1.mese, data1.giorno,
           data2.anno, data2.mese, data2.giorno); // Stampa intestazione
    
    for (int i = 0; i < n; i++) { // Scorre tutte le corse
        // Verifica se data corsa è nell'intervallo
        if (confrontaDate(corse[i].data, data1) >= 0 && 
            confrontaDate(corse[i].data, data2) <= 0) {
            stampaCorsa(corse[i]); // Stampa corsa
            trovate++; // Incrementa contatore
        }
    }
    
    if (trovate == 0) { // Nessuna corsa trovata
        printf("Nessuna corsa trovata.\n"); // Messaggio
    }
}

// Funzione per elencare corse da una fermata di partenza
void elencaPerPartenza(Corsa corse[], int n, char *partenza) {
    int trovate = 0; // Contatore corse trovate
    printf("\nCorse partite da %s:\n", partenza); // Intestazione
    
    for (int i = 0; i < n; i++) { // Scorre tutte le corse
        if (strcmp(corse[i].partenza, partenza) == 0) { // Confronta fermata partenza
            stampaCorsa(corse[i]); // Stampa corsa
            trovate++; // Incrementa contatore
        }
    }
    
    if (trovate == 0) { // Nessuna corsa trovata
        printf("Nessuna corsa trovata.\n"); // Messaggio
    }
}

// Funzione per elencare corse verso un capolinea
void elencaPerCapolinea(Corsa corse[], int n, char *capolinea) {
    int trovate = 0; // Contatore corse trovate
    printf("\nCorse con capolinea in %s:\n", capolinea); // Intestazione
    
    for (int i = 0; i < n; i++) { // Scorre tutte le corse
        if (strcmp(corse[i].destinazione, capolinea) == 0) { // Confronta destinazione
            stampaCorsa(corse[i]); // Stampa corsa
            trovate++; // Incrementa contatore
        }
    }
    
    if (trovate == 0) { // Nessuna corsa trovata
        printf("Nessuna corsa trovata.\n"); // Messaggio
    }
}

// Funzione per elencare corse in ritardo in un intervallo di date
void elencaRitardi(Corsa corse[], int n, Data data1, Data data2) {
    int trovate = 0; // Contatore corse trovate
    printf("\nCorse in ritardo tra %04d/%02d/%02d e %04d/%02d/%02d:\n",
           data1.anno, data1.mese, data1.giorno,
           data2.anno, data2.mese, data2.giorno); // Intestazione
    
    for (int i = 0; i < n; i++) { // Scorre tutte le corse
        // Verifica intervallo date e ritardo > 0
        if (confrontaDate(corse[i].data, data1) >= 0 && 
            confrontaDate(corse[i].data, data2) <= 0 &&
            corse[i].ritardo > 0) {
            stampaCorsa(corse[i]); // Stampa corsa
            trovate++; // Incrementa contatore
        }
    }
    
    if (trovate == 0) { // Nessuna corsa trovata
        printf("Nessuna corsa in ritardo trovata.\n"); // Messaggio
    }
}

// Funzione per calcolare ritardo totale per un codice tratta
void calcolaRitardoTotale(Corsa corse[], int n, char *codice) {
    int ritardo_totale = 0; // Accumula ritardo totale
    int num_corse = 0; // Conta corse trovate
    
    for (int i = 0; i < n; i++) { // Scorre tutte le corse
        if (strcmp(corse[i].codice_tratta, codice) == 0) { // Confronta codice
            ritardo_totale += corse[i].ritardo; // Somma ritardo
            num_corse++; // Incrementa contatore corse
        }
    }
    
    printf("\nTratta %s: %d corse, ritardo totale: %d minuti\n",
           codice, num_corse, ritardo_totale); // Stampa risultato
}

// Funzione per leggere un comando da tastiera
comando_e leggiComando() {
    char comando[MAX_STRING]; // Buffer per comando
    printf("\n=== MENU COMANDI ===\n"); // Menu comandi
    printf("  date        - Elencare corse in intervallo date\n");
    printf("  partenza    - Elencare corse da una fermata\n");
    printf("  capolinea   - Elencare corse verso un capolinea\n");
    printf("  ritardo     - Elencare corse in ritardo\n");
    printf("  ritardo_tot - Calcolare ritardo totale per tratta\n");
    printf("  fine        - Terminare programma\n");
    printf("\nInserisci comando: "); // Richiesta input
    
    scanf("%s", comando); // Legge comando
    
    // Converte stringa in enumerativo
    if (strcmp(comando, "date") == 0) return r_date;
    if (strcmp(comando, "partenza") == 0) return r_partenza;
    if (strcmp(comando, "capolinea") == 0) return r_capolinea;
    if (strcmp(comando, "ritardo") == 0) return r_ritardo;
    if (strcmp(comando, "ritardo_tot") == 0) return r_ritardo_tot;
    if (strcmp(comando, "fine") == 0) return r_fine;
}

// Funzione per selezionare e visualizzare dati in base al comando
void selezionaDati(Corsa corse[], int n, comando_e cmd) {
    Data data1, data2; // Date per intervalli
    char fermata[MAX_STRING]; // Nome fermata
    char codice[MAX_STRING]; // Codice tratta
    
    switch(cmd) { // Switch sul comando
        case r_date: // Comando: corse in intervallo date
            printf("Inserisci data inizio (aaaa/mm/gg): "); // Richiesta data1 (CORRETTO)
            scanf("%d/%d/%d", &data1.anno, &data1.mese, &data1.giorno); // Legge data1
            printf("Inserisci data fine (aaaa/mm/gg): "); // Richiesta data2 (CORRETTO)
            scanf("%d/%d/%d", &data2.anno, &data2.mese, &data2.giorno); // Legge data2
            elencaPerDate(corse, n, data1, data2); // Chiama funzione
            break;
            
        case r_partenza: // Comando: corse da fermata
            printf("Inserisci fermata di partenza: "); // Richiesta fermata
            scanf("%s", fermata); // Legge fermata
            elencaPerPartenza(corse, n, fermata); // Chiama funzione
            break;
            
        case r_capolinea: // Comando: corse verso capolinea
            printf("Inserisci capolinea: "); // Richiesta capolinea
            scanf("%s", fermata); // Legge capolinea
            elencaPerCapolinea(corse, n, fermata); // Chiama funzione
            break;
            
        case r_ritardo: // Comando: corse in ritardo in intervallo
            printf("Inserisci data inizio (aaaa/mm/gg): "); // Richiesta data1 (CORRETTO)
            scanf("%d/%d/%d", &data1.anno, &data1.mese, &data1.giorno); // Legge data1
            printf("Inserisci data fine (aaaa/mm/gg): "); // Richiesta data2 (CORRETTO)
            scanf("%d/%d/%d", &data2.anno, &data2.mese, &data2.giorno); // Legge data2
            elencaRitardi(corse, n, data1, data2); // Chiama funzione
            break;
            
        case r_ritardo_tot: // Comando: ritardo totale per tratta
            printf("Inserisci codice tratta: "); // Richiesta codice
            scanf("%s", codice); // Legge codice
            calcolaRitardoTotale(corse, n, codice); // Chiama funzione
            break;
            
        case r_fine: // Comando: termina programma
            printf("Programma terminato.\n"); // Messaggio uscita
            break;
    }
}

// Funzione main
int main() {
    Corsa corse[MAX_CORSE]; // Array di corse
    int n_corse; // Numero corse lette
    comando_e comando; // Comando corrente
    
    // Legge il file delle corse
    n_corse = leggiFile("corse.txt", corse); // Carica dati da file
    
    if (n_corse == 0) { // Controllo se file letto correttamente
        printf("Nessuna corsa caricata. Uscita.\n"); // Messaggio errore
        return 1; // Termina con errore
    }
    
    printf("Caricate %d corse dal file.\n", n_corse); // Conferma caricamento
    
    // Loop principale del menu
    do {
        comando = leggiComando(); // Legge comando utente
        selezionaDati(corse, n_corse, comando); // Esegue comando
    } while (comando != r_fine); // Continua finché non si sceglie "fine"
    
    return 0; // Termina con successo
}