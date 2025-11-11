#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>

#define TEST 0
#define EMPTY NULL
#define MAX 50

typedef struct paper_ {
  char *title, *speaker, *topic;
  int slot;
} paper;
typedef paper* PAPER;

typedef struct papers_ {
  int np;
  PAPER *vp;
} papers;
typedef papers* PAPERS;

typedef struct rs_ {
  int room, slot;
} rs;

typedef struct schedule_ {
  int nr, ns;
  // Table version NS x NR (rooms as columns make for an easier check of speakers overlap)
  PAPER **table;
  // Compact versione, a room-initial_slot pair for each paper
  rs *paper_slot;
  // Params to evaluate goodness of this schedule
  int holes; // Empty slot not at the end of the day
  int disp; // Topic dispersion among rooms
} schedule;
typedef schedule* SCHEDULE;

PAPER PAPERinit() {
  PAPER p = calloc(1, sizeof(*p));
  return p;
}

PAPER PAPERread(FILE *fp) {
  PAPER p = PAPERinit();
  char t[MAX], s[MAX], to[MAX];
  fscanf(fp, "%s %s %d %s\n", t, s, &p->slot, to);
  p->title = strdup(t);
  p->speaker = strdup(s);
  p->topic = strdup(to);
  return p;
}

void PAPERstore(FILE *fp, PAPER p) {
  if (!fp || !p) return;
  fprintf(fp, "%s %s %d %s\n", p->title, p->speaker, p->slot, p->topic);
}

PAPERS PAPERSinit(int np) {
  PAPERS pp = calloc(1, sizeof(papers));
  pp->np = np;
  pp->vp = calloc(pp->np, sizeof(PAPER));
  return pp;
}

PAPERS PAPERSread(char *filename) {
  int i, np;
  FILE *fp = fopen(filename, "r");
  if(!fp) return NULL;
  fscanf(fp, "%d\n", &np);
  PAPERS pp = PAPERSinit(np);
  if(!pp) return NULL;
  for(i=0;i<pp->np;i++) {
    pp->vp[i] = PAPERread(fp);
  }
  return pp;
}

void PAPERSstore(FILE *fp, PAPERS pp) {
  if (!fp || !pp) return;
  int i;
  for(i=0;i<pp->np;i++) PAPERstore(fp, pp->vp[i]);
}

SCHEDULE SCHEDULEinit(int R, int S, int np) {
  int i;
  SCHEDULE s = calloc(1, sizeof(*s));
  s->nr = R;
  s->ns = S;
  s->paper_slot = calloc(np, sizeof(rs));
  s->table = calloc(S, sizeof(PAPER*));
  for(i=0;i<S;i++) s->table[i] = calloc(R, sizeof(PAPER));
  for(i=0;i<np;i++) s->paper_slot[i].room = s->paper_slot[i].slot = -1;
  return s;
}

int SCHEDULEcheckPaper(SCHEDULE s, PAPER p, int index, int room, int slot) {
  int i, j;
  // Make sure we can place this paper starting from this slot
  for(i=slot;i<p->slot+slot;i++) {
    if (s->table[i][room] != EMPTY)
      return 0;
    // Make sure the speaker is not in another room already
    for(j=0;j<s->nr;j++) {
      if(s->table[i][j] != EMPTY && s->table[i][j]->speaker == p->speaker)
        return 0;
    }
  }
  return 1;
}

void SCHEDULEplacePaper(SCHEDULE s, PAPER p, int index, int room, int slot) {
  int i;
  // Place the paper
  for(i=slot;i<p->slot+slot;i++) s->table[i][room] = p;
  s->paper_slot[index].room = room;
  s->paper_slot[index].slot = slot;
}

void SCHEDULEremovePaper(SCHEDULE s, PAPER p, int index, int room, int slot) {
  // Remove the paper
  int i;
  for(i=slot;i<p->slot+slot;i++) s->table[i][room] = EMPTY;
  s->paper_slot[index].room = -1;
  s->paper_slot[index].slot = -1;
}

void SCHEDULEprint(SCHEDULE s) {
  int i, j;
  for(i=0;i<s->ns;i++) {
    for(j=0;j<s->nr;j++) {
      if (s->table[i][j]) fprintf(stdout, "%s\t", s->table[i][j]->title);
      else fprintf(stdout, "---\t");
    }
    printf("\n");
  }
  printf("H = %d, D = %d\n", s->holes, s->disp);
}

void SCHEDULEeval(SCHEDULE s) {
  // Empty slots
  // Start from the end of the day, ignore all the empty slots until the last paper and then count
  int i, j, k;
  int h = 0, d = 0;
  // For each room
  for(i=0;i<s->nr;i++) {
    // For each slot backward
    PAPER last = NULL;
    for(j=s->ns-1;j>=0;j--) {
      if (s->table[j][i]) last = s->table[j][i];
      if (!last) continue;
      if (last && !s->table[j][i]) h++;
    }
  }
  s->holes = h;

  // Topic dispersion
  int *tt = calloc(s->nr, sizeof(int));
  // For each room
  for(i=0;i<s->nr;i++) {
    // For each occupied starting slot
    for(j=0;j<s->ns;) {
      if (s->table[j][i]) {
        // For each previous paper
        for(k=0;k<j;) {
          if(s->table[k][i]) {
            tt[i] = tt[i] + (strcmp(s->table[j][i]->topic, s->table[k][i]->topic) != 0);
            k = k + s->table[k][i]->slot;
          } else {
            k = k + 1;
          }
        }
        j = j + s->table[j][i]->slot;
      } else {
        j = j + 1;
      }
    }
    d += tt[i];
  }
  s->disp = d;

  free(tt);
}

void solve_r(PAPERS pp, SCHEDULE s, int pos) {
  if (pos >= pp->np) {
    SCHEDULEeval(s);
    // Copy if better...
    SCHEDULEprint(s);
    return;
  }

  int i, j;
  // For each room
  for(i=0;i<s->nr;i++) {
    // For each slot, taking into account the slots requirements for the paper (can't start "too late" in case of multi-slots papers)
    for(j=0;j<=s->ns - pp->vp[pos]->slot;j++) {
      if(SCHEDULEcheckPaper(s, pp->vp[pos], pos, i, j)) {
        SCHEDULEplacePaper(s, pp->vp[pos], pos, i, j);
        solve_r(pp, s, pos+1);
        SCHEDULEremovePaper(s, pp->vp[pos], pos, i, j);
      }
    }
  }
}

void solve(PAPERS pp, int R, int S) {
  SCHEDULE s = SCHEDULEinit(R, S, pp->np);
  solve_r(pp, s, 0);
}

void check_file(PAPERS pp, int R, int S, char *fn) {
  FILE *fp = fopen(fn, "r");
  if (!fp) return;
  int room, slot, i=0, ok = 1;
  SCHEDULE s = SCHEDULEinit(R, S, pp->np);
  // Read a file in which we have a line for each paper, with a room-slot pair
  while(i<pp->np && fscanf(fp, "%d %d", &room, &slot) == 2) {
    if (!SCHEDULEcheckPaper(s, pp->vp[i], i, room, slot)) {
      ok = 0;
      break;
    }
    SCHEDULEplacePaper(s, pp->vp[i], i, room, slot);
  }
  if (i < pp->np) ok = 0;
  if (!ok) printf("Schedule is %svalid\n", ok ? "" : "not ");
}

int main(int argc, char **argv) {

  FILE *in;
  if (argc != 5) {
    printf("Use: %s <file_papers> <file_schedule> <R> <S>\n", argv[0]);
    exit(-1);
  }

  in = fopen(argv[1], "r");
  if (in == NULL)
    exit(-1);

  int R = atoi(argv[3]);
  int S = atoi(argv[4]);

  PAPERS pp = PAPERSread(argv[1]);
  if (!pp) return -1;
  PAPERSstore(stdout, pp);

  // Verifica
  check_file(pp, R, S, argv[2]);

  // Generazione
  solve(pp, R, S);

  return 0;
}
