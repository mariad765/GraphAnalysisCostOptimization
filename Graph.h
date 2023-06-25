/*DOBRE Maria-Adina - 312CD*/
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
/*...................Define structures.............................*/
/*...................Declare functions.............................*/

#ifndef GRAPH_H_
#define GRAPH_H_
typedef float TCost;

typedef struct node {
  int v;             // index of the node
  char *string;      // node name
  TCost c;           // cost of node
  struct node *next; // pointer to the next node in  adjacency list
} TNode, *ATNode;

typedef struct {
  int N; // number of nodes
  int M; // number of bidirectional vertexes
  ATNode *adl;
} TGraphL;

// read function for task 1
extern void read_graph(const char *filename, TGraphL *G);

// allocate memory for the graph in both tasks
extern void alloc_graph(TGraphL *G, int n, int m);

// dfs function for task 1
extern int dfsRecursive(TGraphL *graph, int **totalCost);

// free memory in task 1 and 2
extern void free_graph(TGraphL *G);

// create adjacency list for task 1 - graph in not oriented
extern void insert_edge_list(TGraphL *G, int v1, int v2, float c);

// function performing prim algorithm
extern int Prim(TGraphL *G, int start_node, int size);

// read graph for task 2
extern void read_graph2(const char *filename, TGraphL *G, int *goldWeight);

// perform modified disktra for task 2
extern void dijkstra(TGraphL *G, int s, int destination, int *depths,
                     int goldWeight, int hasPathToSource);

// create adjacency list for task 2 - graph oriented
extern void insert_edge_list2(TGraphL *G, int v1, int v2, float c);

// function that checks if there is any way from "Corabie " to " Insula"
int hasPathToSourceDFS(TGraphL *G, int currentVertex, int sourceIndex,
                       int *visited);

#endif /* GRAPH_H_ */
