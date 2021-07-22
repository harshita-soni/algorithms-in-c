/*
graph.h

Visible structs and functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2021
*/

/* Definition of a graph. */
#include "pq.h"

struct graph;

enum problemPart;

struct solution;

/* A particular solution to a graph problem. */
#ifndef SOLUTION_STRUCT
#define SOLUTION_STRUCT
struct solution {
  int antennaTotal;
  int cableTotal;
  int mixedTotal;
};
#endif

/* Which part the program should find a solution for. */
#ifndef PART_ENUM
#define PART_ENUM
enum problemPart {
  PART_A=0,
  PART_C=1
};
#endif

/* Creates an undirected graph with the given numVertices and no edges and
returns a pointer to it. NumEdges is the number of expected edges. */
struct graph *newGraph(int numVertices);

/* Adds an edge to the given graph. */
void addEdge(struct graph *g, int start, int end, int cost);

/* Find the total radio-based cost, total cabled cost if the part is PART_A, and
  the mixed total cost if the part is PART_C. */
struct solution *graphSolve(struct graph *g, enum problemPart part,
  int antennaCost, int numHouses);

/* Frees all memory used by graph. */
void freeGraph(struct graph *g);

/* Frees all data used by solution. */
void freeSolution(struct solution *solution);

/* runs rims algorithm on the graph to find minimum costs for cable installation */
void prims(struct graph *g, int cost[], int prev [], int vertices);

/* helper function for prims, updates costs for adjacent nodes of the vertex recently added to the mst */
void updatecosts(struct graph *g, struct pq *priq, int u, int cost[], int prev []);