#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "timer.h"
#include <stdint.h>

#define DEBUG 0
#define INFINITY (1.0/0.0)

typedef uint32_t u32;
typedef uint64_t u64;

typedef struct {
  u32 i;
  u32 j;
  float w;
} edge;

typedef struct {
  u64 num_e;
  u32 num_v;
  u32 *n;
  edge *e;
} graph;

/* for sorting edges accordding to their indices */
int edge_cmp(const void *a, const void *b) {

  edge *ea = (edge*)a;
  edge *eb = (edge*)b;

  if (ea->i < eb->i) return -1;
  if (ea->i > eb->i) return 1;
  if (ea->j < eb->j) return -1;
  if (ea->j > eb->j) return 1;
  if (ea->w < eb->w) return -1;
  if (ea->w > eb->w) return 1;

  return 0; /* unlikely */
}

void graph_read(graph *g, char *fname) {

  FILE *f = fopen(fname, "r");
  if (NULL == f) {
    fprintf(stderr, "couldn't open %s\n",
    fname); return;
  }

  printf("reading %s...\n", fname);

  edge *e;

  int ok=0;

  if (1 == fscanf(f, "%u\n", &(g->num_v))) {
    if (1 == fscanf(f, "%llu\n", &(g->num_e))) {
      e = calloc(g->num_e, sizeof(edge));
      g->n = (u32*) calloc( g->num_v, sizeof(u32));
      for (u64 x=0; x<g->num_e; x++) {
        if (3 != fscanf(f, "%u %u %f\n",
          &(e[x].i), &(e[x].j), &(e[x].w))) {
            fprintf(stderr,
            "uh-oh, expected an edge "
            "(line %llu)\n", x+2);
            free(e);
            fclose(f);
            return;
        } else { }
        ok=1;
      }
    }
  }

  fclose(f);

  if (0==ok) {
    fprintf(stderr,
      "something wrong with the file\n");
    if (NULL != g->n) free(g->n);
    return;
  } else {
    printf("OK: num_v=%u, num_e=%llu\n",
           g->num_v, g->num_e);
  }

  qsort(e, g->num_e, sizeof(edge), edge_cmp);
  g->e = e;

  u64 x = 0, idx = 0, src = e[0].i;

  for (u32 x=0; x<g->num_v; x++) {
    g->n[x] = idx;
    while (src == e[idx].i) idx++;
    src = e[idx].i;
  }
}

void print_edges(int n, edge *e) {
  for (u64 x=0; x<n; x++) {
    printf("%d %d %f\n", e[x].i, e[x].j, e[x].w);
  }
}

void dump_edges(int n, edge *e, char *fname) {
  FILE *f = fopen(fname, "w");
  if (!f) return;
  for (u64 x=0; x<n; x++) {
    fprintf(f, "%d %d %f\n", e[x].i, e[x].j, e[x].w);
  }
  fclose(f);
}

void graph_show(graph *g) {

  if (NULL == g->e) {
    puts("graph is empty");
  }
  else
  for (u32 v=0; v<g->num_v; v++) {
    printf("%d [%2d]: ", v, g->n[v]);
    u32 s = g->n[v];
    u32 end = v == (g->num_v-1) ?
      g->num_e : g->n[v+1];
    edge *e = &(g->e[s]);
    u32 n = end-s;
    for (u32 x=0; x<n; x++) {
      printf("%s (#%d; %2d, %2d, %4.2f)",
        x==0 ? "" : ", ", x, e[x].i, e[x].j, e[x].w);
    }
    puts("");
  }
}

void graph_free(graph *g) {

  if (NULL != g->e) {
    free(g->e); g->e = NULL;
  }

  if (NULL != g->n) {
    free(g->n); g->n = NULL;
  }

}

void graph_sssp(graph *g, char *oname) {

  heap h;
  heap_init(&h, 1);

  u32 src = g->e[0].i;
  u32 *visited =
    (u32*) calloc(g->num_v, sizeof(u32));
  edge *d =
    (edge*) malloc(g->num_v * sizeof (edge));
  for (u32 j=0; j<g->num_v; j++)
    d[j] = (edge){src, j, INFINITY};
  d[src].w = 0;

  //
  int done=0, iter=0;

  while (0 == done || iter == 0)
  {
    // mark as final
    visited[src] = 1;
    // distance so far to src
    float current_d = d[src].w;
    u32 s = g->n[src];
    u32 end = src == (g->num_v-1) ?
      g->num_e : g->n[src+1];
    u32 n = end-s;
    edge *e = &(g->e[s]);
    for (u32 x=0; x<n; x++) {
      if (0 == visited[e[x].j]) {
        float new_d = current_d + e[x].w;
        if (d[e[x].j].w > new_d) {
          d[e[x].j].w = new_d;
          kv tmp = {e[x].j, new_d};
          heap_insert(&h, &tmp);
        }
      }
    } // for i

    u32 new_src;
    do {
      // check the front/top of the queue/heap
      kv *top = heap_peek(&h);
      if (NULL == top) {
        // the queue is empty
        done=1; break;
      }
      new_src = top->key;
      heap_pop(&h);
      // ignore all vertices visited
    } while (1 == visited[new_src]);
    src = new_src;
    iter++;
  }

  if (NULL != oname) dump_edges(g->num_v, d, oname);

  free(visited);
  free(d);
  heap_release(&h);
}

int main(int argc, char **argv) {

  if (argc < 2) {
    printf("usage: %s file [out]\n",
      argv[0]);
    return -1;
  }

  char *fname = argv[1];
  char *oname = NULL;

  if (argc > 2) oname = argv[2];

  graph g;
  timeit(graph_read(&g, fname));
  //graph_show(&g);
  timeit(graph_sssp(&g, oname));
  graph_free(&g);

  return 0;
}
