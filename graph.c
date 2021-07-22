/*
graph.c

Set of vertices and edges implementation.

Implementations for helper functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2021
*/
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "graph.h"
#include "utils.h"

#define INITIALEDGES 32
#define INITIALVERTEX 0

struct edge;

/* Definition of a graph. */
struct graph {
  int numVertices;
  int numEdges;
  int allocedEdges;
  struct edge **edgeList;
};

/* Definition of an edge. */
struct edge {
  int start;
  int end;
  int cost;
};

struct graph *newGraph(int numVertices){
  struct graph *g = (struct graph *) malloc(sizeof(struct graph));
  assert(g);
  /* Initialise edges. */
  g->numVertices = numVertices;
  g->numEdges = 0;
  g->allocedEdges = 0;
  g->edgeList = NULL;
  return g;
}

/* Adds an edge to the given graph. */
void addEdge(struct graph *g, int start, int end, int cost){
  assert(g);
  struct edge *newEdge = NULL;
  /* Check we have enough space for the new edge. */
  if((g->numEdges + 1) > g->allocedEdges){
    if(g->allocedEdges == 0){
      g->allocedEdges = INITIALEDGES;
    } else {
      (g->allocedEdges) *= 2;
    }
    g->edgeList = (struct edge **) realloc(g->edgeList,
      sizeof(struct edge *) * g->allocedEdges);
    assert(g->edgeList);
  }

  /* Create the edge */
  newEdge = (struct edge *) malloc(sizeof(struct edge));
  assert(newEdge);
  newEdge->start = start;
  newEdge->end = end;
  newEdge->cost = cost;

  /* Add the edge to the list of edges. */
  g->edgeList[g->numEdges] = newEdge;
  (g->numEdges)++;
}

/* Frees all memory used by graph. */
void freeGraph(struct graph *g){
  int i;
  for(i = 0; i < g->numEdges; i++){
    free((g->edgeList)[i]);
  }
  if(g->edgeList){
    free(g->edgeList);
  }
  free(g);
}

struct solution *graphSolve(struct graph *g, enum problemPart part,
  int antennaCost, int numHouses){
  struct solution *solution = (struct solution *)
    malloc(sizeof(struct solution));
  assert(solution);

  int vertices = g->numVertices, i;
  int cost[vertices], prev[vertices];
  prims(g, cost, prev, vertices);

  if(part == PART_A){
    /* IMPLEMENT 2A SOLUTION HERE */

    solution->antennaTotal = 0;
    solution->cableTotal = 0;

    solution->antennaTotal = antennaCost * (numHouses);
    
    for (i = 0; i < vertices; i++) {
      solution->cableTotal += cost[i];
    }

  } else {
    /* IMPLEMENT 2C SOLUTION HERE */
    solution->mixedTotal = 0;
    
    int optimum_cost[vertices];
    memcpy(optimum_cost, cost, vertices * sizeof(int));

    for (i = 0; i < vertices; i++) {
      if (optimum_cost[i] > antennaCost) {
        optimum_cost[i] = antennaCost;
      }
    }

    for (i = 0; i < vertices; i++) {
      solution->mixedTotal += optimum_cost[i];
    }
  }
  return solution;
}


void prims(struct graph *g, int cost[], int prev [], int vertices) {
  int i, u;

  for (i = 0; i < vertices; i++) {
    cost[i] = INT_MAX;
    prev[i] = -1;
  }

  cost[INITIALVERTEX] = 0;

  struct pq *priq = newPQ();

  for (i = 0; i < vertices; i++) {
    enqueue(priq, i, cost[i]);
  }
  
  while(!empty(priq)) {
    u = deletemin(priq);
    updatecosts(g, priq, u, cost, prev);
  }
}

void updatecosts(struct graph *g, struct pq *priq, int u, 
int cost[], int prev []) {
  int i, w = -1;

  for (i = 0; i < g->numEdges; i++) {
      w = -1;
      if (g->edgeList[i]->start == u) {
        w = g->edgeList[i]->end;
      } else if (g->edgeList[i]->end == u) {
        w = g->edgeList[i]->start;
      } else {
        continue;
      }
      
      if (pqhasnode(priq, w) && g->edgeList[i]->cost < cost[w]) {
        cost[w] = g->edgeList[i]->cost;
        prev[w] = u;
        // update cost for w; shorter length found
        updatecost(priq, w, cost[w]);
      }
    }
}