#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>
#define LEN 100

int regexpCmp(char *s, char *regexp);
int regexpLen(char *regexp);
char *cercaRegexp(char *s, char *regexp, int lenS, int lenR);
int regexpCmpChar(char c, char *regexp, int *np);

int main(int argc, char **argv) {
  char *src=argv[1], *regexp=argv[2], *exp;
  int lenS, lenR, match=0;

  lenS = strlen(src);
  printf("src = %s src len = \%d \n", src, lenS);
  lenR = regexpLen(regexp);
  printf("regexp len (%s) = %d\n", regexp, lenR);

  do {
    exp = cercaRegexp(src, regexp, lenS, lenR);
    if (exp) {
      printf("%.*s\n", lenR, exp);
      /* salta parola trovata. Cercane un'altra (con eventuali sovrapposizioni)*/
      src = exp + 1;
      match++;
    }
  } while (exp!=NULL);

  printf("Trovati %d match\n", match);

  return 0;
}

int regexpLen(char *regexp) {
  int i, len, salta;

  for (i=len=0; regexp[i]!='\0'; len++,i+=salta) {
    /* non confronta ma salta un metacarattere in regexp */
    regexpCmpChar(' ',&regexp[i],&salta);
  }
  return len;
}

int regexpCmpChar(char c, char *regexp, int *np) {
  int res, n;
  switch(regexp[0]) {
  case '.':
    res = 1; /* qualunque carattere va bene */
    n = 1;
    break;
  case '\\':
    /* si assume regexp corretta, quindi se non regexp[1] non e' a si assume sia A */
    res = regexp[1]=='a' ? islower(c) : isupper(c);
    n = 2;
    break;
  case '[':
    /* quantificazione esistenziale: res = 1 se e solo se il carattere corrente c e' o non e' uno dei caratteri fra [].
       Assumi falso, scandisci regesxp, se regexp[n} e' ^ assumi vero e passa al prossimo carattere, altrimenti  se il carattere
       corrente c e' tra quelli ammessi/vietati, cambia verdetto */
    res = 0;
    for(n=1; regexp[n]!=']'; n++) {
      if (regexp[n] == '^') {
        res = 1; /* cambia verdetto */
      }
      else if (regexp[n]==c)
        res = !res;
    }
    n++; /* per contare la ']' */
  break;
  default:
    n = 1;
    res = c==regexp[0];
  }

  *np = n;    /* ritorna caratteri usati in regexp */
  return res; /* ritorna risultato vero/falso */
}

int regexpCmp(char *s, char *regexp) {
  int i, ir, salta;

  for (i=ir=0; regexp[ir]!='\0'; i++, ir+=salta) {
    /* non confronta ma salta un metacarattere in regexp */
    if (!regexpCmpChar(s[i],&regexp[ir],&salta))
      return 0; /* trovata una differenza */
  }
  return 1;
}


char *cercaRegexp(char *src, char *regexp, int lenS, int lenR) {
  int i;

  for (i=0; i<=lenS-lenR; i++) {
    if (regexpCmp(&src[i],regexp))
      return (&src[i]);
  }
  return NULL;
}


