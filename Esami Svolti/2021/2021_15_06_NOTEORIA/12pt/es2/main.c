#include <stdio.h>
#include "list.h" /* Includiamo la nostra definizione */

int main() {
    /* --- TEST CASE 1 --- */
    printf("--- Test 1: abracadabra, sep='r' ---\n");
    char s1[] = "abracadabra";
    char sep1 = 'r';
    
    LIST l1 = list_create();
    split(l1, s1, sep1);
    
    printf("Input: %s\nOutput atteso: ab -> racadab -> ra\nOutput reale:  ", s1);
    list_print(l1);
    
    /* Pulizia memoria Test 1 */
    list_destroy(l1);

    printf("\n");

    /* --- TEST CASE 2 --- */
    printf("--- Test 2: ramarro, sep='r' ---\n");
    char s2[] = "ramarro";
    char sep2 = 'r';
    
    LIST l2 = list_create();
    split(l2, s2, sep2);
    
    printf("Input: %s\nOutput atteso: rama -> r -> ro\nOutput reale:  ", s2);
    list_print(l2);

    /* Pulizia memoria Test 2 */
    list_destroy(l2);

    return 0;
}