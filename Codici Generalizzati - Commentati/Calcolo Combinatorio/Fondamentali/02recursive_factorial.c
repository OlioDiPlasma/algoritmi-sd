#include <stdio.h>

// Dichiarazione della funzione fattoriale
unsigned long fact(int n);

int main() {
  int n;
  
  // Chiede all'utente di inserire un numero
  // Nota: il limite è 12 perché 13! = 6,227,020,800 supera il range
  // di unsigned long su molti sistemi (max ~4 miliardi per 32 bit)
  printf("Input n (<=12):  ");
  scanf("%d", &n);
  
  // Chiama la funzione fattoriale e stampa il risultato
  // %lu è lo specificatore di formato per unsigned long
  printf("factorial of %d is: %lu \n", n, fact(n));
  
  return 0;
}

/* FUNZIONE FATTORIALE RICORSIVA
 * Calcola n! = n × (n-1) × (n-2) × ... × 2 × 1
 * 
 * Esempi:
 * - 0! = 1 (per definizione)
 * - 1! = 1
 * - 2! = 2 × 1 = 2
 * - 3! = 3 × 2 × 1 = 6
 * - 4! = 4 × 3 × 2 × 1 = 24
 * - 5! = 5 × 4 × 3 × 2 × 1 = 120
 * 
 * La funzione usa la ricorsione:
 * n! = n × (n-1)!
 */
unsigned long fact(int n) {
  
  // CASO BASE: il fattoriale di 0 è 1 per definizione
  // Questo ferma la ricorsione
  if(n == 0)
    return 1;
  
  // CASO RICORSIVO: n! = n × (n-1)!
  // La funzione chiama sé stessa con un valore più piccolo
  return n * fact(n-1);
}

/* ESEMPIO DI ESECUZIONE CON n=4:
 * 
 * fact(4) 
 *   → calcola: 4 * fact(3)
 *                    ↓
 *              fact(3)
 *                → calcola: 3 * fact(2)
 *                               ↓
 *                         fact(2)
 *                           → calcola: 2 * fact(1)
 *                                          ↓
 *                                    fact(1)
 *                                      → calcola: 1 * fact(0)
 *                                                     ↓
 *                                               fact(0)
 *                                                 → ritorna: 1 (CASO BASE)
 *                                    
 * Poi la ricorsione "risale" calcolando i risultati:
 * 
 * fact(0) = 1
 * fact(1) = 1 * 1 = 1
 * fact(2) = 2 * 1 = 2
 * fact(3) = 3 * 2 = 6
 * fact(4) = 4 * 6 = 24  ← RISULTATO FINALE
 * 
 * 
 * STACK DELLE CHIAMATE (cosa succede in memoria):
 * 
 * 1. main() chiama fact(4)
 * 2. fact(4) chiama fact(3)   [fact(4) aspetta in memoria]
 * 3. fact(3) chiama fact(2)   [fact(4) e fact(3) aspettano]
 * 4. fact(2) chiama fact(1)   [fact(4), fact(3), fact(2) aspettano]
 * 5. fact(1) chiama fact(0)   [tutte le chiamate precedenti aspettano]
 * 6. fact(0) ritorna 1        [inizia a svuotare lo stack]
 * 7. fact(1) calcola 1*1=1 e ritorna
 * 8. fact(2) calcola 2*1=2 e ritorna
 * 9. fact(3) calcola 3*2=6 e ritorna
 * 10. fact(4) calcola 4*6=24 e ritorna a main()
 * 
 * 
 * PERCHÉ IL LIMITE A 12?
 * 
 * Su sistemi a 32 bit, unsigned long può contenere valori fino a 4,294,967,295
 * 
 * 12! = 479,001,600        ✓ OK
 * 13! = 6,227,020,800      ✗ OVERFLOW (supera il limite!)
 * 
 * Su sistemi a 64 bit il limite è più alto, ma 20! è già troppo grande
 */