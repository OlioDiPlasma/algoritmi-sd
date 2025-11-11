//Graham Scan
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/* =============================================================
 * Struttura dati per i punti
 * ============================================================= */
typedef struct point *Point;
struct point {
    int x, y;   // coordinate
    int flag;   // flag per indicare se il punto è utile nella scansione
};

/* =============================================================
 * PROTOTIPI
 * ============================================================= */
Point NEWpoint(int, int);
void ReadInputFile(char *);
void LookForP0(void);
void MergeSort(int, int);
void Merge(int, int, int);
int ComparePoints(Point, Point);
int MoreDistant(Point, Point);
int CrossProduct(Point, Point, Point);
void GrahanScan(void);
void WriteOutputFile(char *);

/* =============================================================
 * VARIABILI GLOBALI
 * ============================================================= */
Point *P; // array di punti
int N;    // numero totale di punti

/* =============================================================
 * MAIN
 * ============================================================= */
int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        exit(1);
    }

    // Legge i punti da file
    ReadInputFile(argv[1]);

    // Calcola il convex hull con Graham Scan
    GrahanScan();

    // Scrive il risultato su file
    WriteOutputFile(argv[2]);

    return 0;
}

/* =============================================================
 * NEWpoint
 * -------------------------------------------------------------
 * Crea un nuovo punto con coordinate (x,y) e flag iniziale = 1
 * ============================================================= */
Point NEWpoint(int x, int y)
{
    Point p = malloc(sizeof(*p));
    p->x = x;
    p->y = y;
    p->flag = 1;
    return p;
}

/* =============================================================
 * ReadInputFile
 * -------------------------------------------------------------
 * Legge il numero dei punti e le loro coordinate da file
 * ============================================================= */
void ReadInputFile(char* filein)
{
    FILE *fp;
    int i, x, y;

    fp = fopen(filein, "r");
    if(fp == NULL) {
        printf("Error opening input file\n");
        exit(0);
    }

    fscanf(fp, "%d", &N);
    P = malloc(N * sizeof(Point));

    for(i = 0; i < N; i++) {
        fscanf(fp, "%d%d", &x, &y);
        P[i] = NEWpoint(x, y);
    }

    fclose(fp);
}

/* =============================================================
 * LookForP0
 * -------------------------------------------------------------
 * Trova il punto P0 con coordinata y minima (e x minima in caso
 * di pareggio) e lo mette in P[0]
 * ============================================================= */
void LookForP0(void)
{
    int i, min = 0;
    Point tmp;

    for(i = 1; i < N; i++) {
        if(P[i]->y < P[min]->y)
            min = i;
        else if(P[i]->y == P[min]->y)
            if(P[i]->x < P[min]->x)
                min = i;
    }

    tmp = P[0];
    P[0] = P[min];
    P[min] = tmp;
}

/* =============================================================
 * MergeSort / Merge
 * -------------------------------------------------------------
 * Ordina i punti (eccetto P0) per angolo polare rispetto a P0
 * ============================================================= */
void MergeSort(int l, int r)
{
    if(l < r) {
        int q = (l + r)/2;
        MergeSort(l, q);
        MergeSort(q+1, r);
        Merge(l, q, r);
    }
}

void Merge(int l, int q, int r)
{
    int i = l, j = q+1, k;
    Point Q[N];

    for(k = l; k <= r; k++) {
        if(i > q)
            Q[k] = P[j++];
        else if(j > r)
            Q[k] = P[i++];
        else if(ComparePoints(P[i], P[j]) < 0)
            Q[k] = P[i++];
        else
            Q[k] = P[j++];
    }

    for(k = l; k <= r; k++)
        P[k] = Q[k];
}

/* =============================================================
 * ComparePoints
 * -------------------------------------------------------------
 * Confronta due punti rispetto a P0 usando il cross product
 * - se collineari, disattiva il punto più vicino
 * ============================================================= */
int ComparePoints(Point p1, Point p2)
{
    int cp = CrossProduct(P[0], p1, p2);

    if(cp == 0) {
        if(MoreDistant(p1, p2))
            p2->flag = 0;
        else
            p1->flag = 0;
        return -cp; // garantisce ordinamento corretto
    }

    return -cp; // ordinamento in senso antiorario
}

/* =============================================================
 * MoreDistant
 * -------------------------------------------------------------
 * Restituisce 1 se p1 è più lontano da P0 di p2
 * ============================================================= */
int MoreDistant(Point p1, Point p2)
{
    int dx1 = p1->x - P[0]->x;
    int dx2 = p2->x - P[0]->x;
    int dy1 = p1->y - P[0]->y;
    int dy2 = p2->y - P[0]->y;

    return (dx1*dx1 + dy1*dy1) > (dx2*dx2 + dy2*dy2);
}

/* =============================================================
 * CrossProduct
 * -------------------------------------------------------------
 * Restituisce:
 *  1 se p0->p1->p2 forma angolo a sinistra (CCW)
 * -1 se p0->p1->p2 forma angolo a destra (CW)
 *  0 se collineari
 * ============================================================= */
int CrossProduct(Point p0, Point p1, Point p2)
{
    int x1 = p1->x - p0->x;
    int y1 = p1->y - p0->y;
    int x2 = p2->x - p0->x;
    int y2 = p2->y - p0->y;

    int prod = x1 * y2 - x2 * y1;
    if(prod > 0) return 1;
    else if(prod < 0) return -1;
    else return 0;
}

/* =============================================================
 * GrahanScan
 * -------------------------------------------------------------
 * Costruisce il convex hull usando stack
 * ============================================================= */
void GrahanScan(void)
{
    int i, j;

    // Trova P0 e ordina i punti per angolo polare
    LookForP0();
    MergeSort(1, N-1);

    // Inizializza stack
    STACKinit(N);
    STACKpush(P[0]);

    // Inserisce i primi 2 punti utili
    for(i = 1, j = 1; i < 3;) {
        if(P[j]->flag) {
            STACKpush(P[j++]);
            i++;
        } else {
            j++;
        }
    }

    // Inserimento punti rimanenti con test cross product
    for(; j < N; j++) {
        if(P[j]->flag) {
            while(CrossProduct(STACKnext_to_top(), STACKtop(), P[j]) != 1)
                STACKpop();
            STACKpush(P[j]);
        }
    }
}

/* =============================================================
 * WriteOutputFile
 * -------------------------------------------------------------
 * Scrive il numero di punti nel convex hull e le coordinate
 * ============================================================= */
void WriteOutputFile(char *fileout)
{
    FILE *fp;
    int i, M = STACKsize();
    Point q;

    fp = fopen(fileout, "w");
    if(fp == NULL) {
        printf("Error opening output file\n");
        exit(1);
    }

    fprintf(fp, "%d\n", M);
    for(i = 0; i < M && !STACKempty(); i++) {
        q = STACKpop();
        fprintf(fp, "%d %d\n", q->x, q->y);
    }

    fclose(fp);
}
