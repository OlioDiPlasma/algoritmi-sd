#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DBG 0

#include "Graph.h"
#include "ST.h"

typedef struct {
  char name[MAXL];
  int depth;
  int treasure, gold;
} Room;

typedef struct {
  int trap;
  int door;
} Tunnel;

// The graph is undirected, so the pair of edges between two generic nodes u,v shares the same reference to Tunnel
typedef struct node_ {
  Tunnel *t;
  int dest;
  struct node_ *next;
} node;
typedef node* link;

struct G {
  int V, E;
  link *ladj;
  Room *vett;
  ST st;
};

// To collect a solution it's convenient to wrap all the pieces in a struct
typedef struct solution_ {
  int gold, treasure, loot;
  int len, *path;
} solution;

solution SOLinit(int M) {
  solution s;
  s.len = 0;
  s.gold = 0;
  s.loot = 0;
  s.treasure = 0;
  s.path = (int*) malloc (M * sizeof(int));
  return s;
}

Graph GRAPHinit(int nV) {
  Graph g = malloc(sizeof(*g));
  if (g == NULL)
    return NULL;

  g->V = nV;
  g->E = 0;

  g->ladj = (link*) calloc (nV, sizeof(link));
  if (g->ladj == NULL)
    return NULL;

  g->st = STinit(nV);
  if (g->st == NULL)
    return NULL;

  g->vett = (Room*) calloc (nV, sizeof(Room));
  if (g->vett == NULL)
    return NULL;

  return g;
}

link newNode(int dest, Tunnel *t, link next) {
  link n = (link) malloc(sizeof(node));
  n->dest = dest;
  n->t = t;
  n->next = next;
  return n;
}

void GRAPHinsertE(Graph g, int src, int dest, Tunnel *t) {
  g->ladj[src] = newNode(dest, t, g->ladj[src]);
  g->E++;
}

Graph GRAPHload(FILE *in) {
  int i, nV, id1, id2, door, trap;
  char src[MAXL], dst[MAXL];
  Graph g;

  fscanf(in, "%d", &nV);
  g = GRAPHinit(nV);
  if (g == NULL)
    return NULL;

  for (i=0; i<nV; i++) {
    fscanf(in, "%s %d %d %d\n", g->vett[i].name, &g->vett[i].depth, &g->vett[i].treasure, &g->vett[i].gold);
    STinsert(g->st, g->vett[i].name, i);
  }

  while(fscanf(in, "%s %s %d %d\n", src, dst, &trap, &door) == 4) {
    id1 = STsearch(g->st, src);
    id2 = STsearch(g->st, dst);
    if (id1 != id2 && id1 >= 0 && id2 >= 0) {
      // The pair of edges will share the same Tunnel reference
      Tunnel *t = (Tunnel*) malloc (sizeof(Tunnel));
      t->door = door;
      t->trap = trap;
      GRAPHinsertE(g, id1, id2, t);
      GRAPHinsertE(g, id2, id1, t);
    }
  }
  return g;
}

void GRAPHfree(Graph g) {
  int v;
  link iter, del;
  for (v=0; v<g->V; v++) {
    for(iter=g->ladj[v];iter;) {
      del = iter;
      iter=iter->next;
      if (v < del->dest) {
        // Tunnels are shared. Avoid double free
        free(del->t);
      }
      free(del);
    }
  }
  free(g->ladj);
  free(g->vett);
  STfree(g->st);
  free(g);
}

char *GRAPHgetName(Graph g, int index) {
  if (g == NULL) return NULL;
  if (g->vett == NULL) return NULL;
  return g->vett[index].name;
}

// Take into account the current loot (gold+treasure) and the depth of the room where we would stop to compute how much we would lose
int deduction(Graph g, int pos, int loot) {
  if (pos < 0) return -1; // Should never happen to end in an invalid room
  if (pos == 0) return 0; // The entrance is safe, we lose nothing
  int depth = g->vett[pos].depth;
  if (depth >= 3) return loot; // We lose everything (no rescue team this deep, we just die)
  if (depth == 2) return ceil(loot/2); // We lose half our loot because we are deep(er) in the dungeon
  if (depth == 1) return ceil(loot/3); // We lose a third of our loot because we are deep in the dungeon
  return 0; // We should never get here
}

void GRAPHtestPath(Graph g, int PF, int G, int M, FILE *in) {
  int i, index, steps = 0, ok = 1;
  char step[MAXL];
  link iter;

  int pos = -1, gold = 0, treasure = 0, hp = PF, lockpicks = G;

  fscanf(in, "%d\n", &steps);
  if (steps > M) ok = 0;

  for(i=0;i<steps && ok;i++) {

    if(!hp) {
      // We have run out of health points
      // We are allowed to pass out in any given room waiting for help, so this is not a straight fail
      printf("Out of health points\n");
      break;
    }

    fscanf(in, "%s", step);
    index = STsearch(g->st, step);
    if (index < 0) {
      // Room not found
      printf("Room not found\n");
      ok = 0;
      break;
    }

    if (pos == -1 && index != 0) {
      // We must start from the dungeon entrance
      printf("Wrong starting point\n");
      ok = 0;
      break;
    }

    if (pos >= 0) {
      // The tunnel from pos to index must exist
      for(iter=g->ladj[pos];iter;iter=iter->next) {
        if (iter->dest == index) break;
      }
      if (iter == NULL) {
        printf("No available path between the rooms\n");
        ok = 0;
        break;
      }

      if (iter->t->door == 1) {
        // We need at least one lockpick to walk this path
        if (!lockpicks) {
          printf("Can't open the door\n");
          ok = 0;
          break;
        }
        printf("We used a lockpick %d -> %d\n", lockpicks, lockpicks-1);
        lockpicks--;
        iter->t->door = 0;
      }

      if (iter->t->trap == 1) {
        // We suffer damage from the trap
        printf("We took damage %d -> %d\n", hp, hp-1);
        hp--;
      }
    }

    pos = index;
    printf("Enter room %s\n", g->vett[pos].name);
    if(hp) {
      gold += g->vett[index].gold;
      g->vett[index].gold = 0;
      if (treasure < g->vett[index].treasure) treasure = g->vett[index].treasure;
    }
  }

  if (i < steps) {
    // One or more condition were not meet, and the visit was not completed
    printf("Could not complete all the steps\n");
    ok = 0;
  }

  if (ok) {
    printf("HP: %d, LOCKPICKS: %d, GOLD: %d, TREASURE: %d, DEDUCTION %d\n", hp, lockpicks, gold, treasure, deduction(g, pos, gold+treasure));
  } else {
    printf("FAIL");
  }
}

void GRAPHfindPathR(Graph g, int pos, int room, int curr_hp, int curr_lp, int M, solution curr, solution *best) {

  // If we are out of moves just return
  if (pos >= M) {
    return;
  }

  link iter;
  int dest = -1, trap = 0, door = 0;
  int gold = 0;

  // To facilitate backtrack later, save this room current gold amount
  gold = g->vett[room].gold;

  // We reached a new room
  curr.path[pos] = room;
  curr.len++;

  // If we are alive, loot the room
  if(curr_hp) {
    curr.gold += gold;
    g->vett[room].gold = 0;
    if (curr.treasure < g->vett[room].treasure) curr.treasure = g->vett[room].treasure;
  }
  // Evaluate current loot even if we are passed out, as we might be saved at lower depths now
  curr.loot = curr.gold + curr.treasure - deduction(g, room, curr.gold + curr.treasure);

  // If improving...
  if (curr.loot > best->loot) {
#if DBG
    printf("Improve %d > %d [G:%d T:%d depth:%d]\n", best->loot, curr.loot, curr.gold, curr.treasure, g->vett[room].depth);
    for(int i=0;i<curr.len;i++) {
      printf("\t%s\n", g->vett[curr.path[i]].name);
    }
#endif
    best->loot = curr.loot;
    best->gold = curr.gold;
    best->treasure = curr.treasure;
    best->len = curr.len;
    memcpy(best->path, curr.path, curr.len*sizeof(int));
  }

  if ((pos && !room) || !curr_hp) {
    // If we are out of health points or we are back at the entrance
    return;
  }

  for(iter=g->ladj[room];iter;iter=iter->next) {
    // To facilitate access and backtrack later
    door = iter->t->door;
    dest = iter->dest;
    trap = iter->t->trap;

    // The door is locked and we are out of lockpicks
    if(door && !curr_lp) continue;

    // Open the door
    if(door && curr_lp) {
      iter->t->door = 0;
    }

    // Traverse the tunnel paying health and lockpicks penalties, if any
    GRAPHfindPathR(g, pos+1, dest, curr_hp - trap, curr_lp - door, M, curr, best);

    // Backtrack of traversed edge, restoring previous door status
    iter->t->door = door;
  }

  // Backtrack on visited room
  // "give back" gold amount to the room
  g->vett[room].gold = gold;
}

void GRAPHfindPath(Graph g, int PF, int G, int M) {
  solution curr = SOLinit(M);
  solution best = SOLinit(M);

  GRAPHfindPathR(g, 0, 0, PF, G, M, curr, &best);

  if (best.len > 0) {
    printf("LOOT: %d, GOLD: %d TREASURE:%d\n", best.loot, best.gold, best.treasure);
    for(int i=0;i<best.len;i++) {
      printf("\t%s [%d]\n", g->vett[best.path[i]].name, g->vett[best.path[i]].depth);
    }
  }

  free(curr.path);
  free(best.path);
}
