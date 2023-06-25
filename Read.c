/*DOBRE Maria-Adina - 312CD*/
#include "Graph.h"
#include "Util.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*.................................................................*/
/*.................................................................*/
/*.................................................................*/

#define MAX_NODES 100
#define MAX_LENGTH 100

// allocate memory for the graph
// strings for adj list not initialized here , especially since they are not
// needed in the foirst task
void alloc_graph(TGraphL *G, int n, int m) {
  int i;                                // counter
  G->N = n;                             // number of nodes
  G->M = m;                             //  number of vertexes
  G->adl = calloc((n), sizeof(ATNode)); // use calloc for safe
  // initialization;
  //  avoid invalid sadresses
  for (i = 0; i < n; i++)
    G->adl[i] = NULL; // each adj list head NULL
}

// function that reads from gile and forms the graph
void read_graph(const char *filename, TGraphL *G) {
  // open file
  FILE *file = fopen(filename, "r");
  if (file == NULL) {  // check if the file poiner is valid
    printf("ERROR\n"); // print error message if the file fails to open
    return;
  }

  char v1[MAX_LENGTH]; // read the two strings
  char v2[MAX_LENGTH];
  int c;    // cost
  int n, m; // nodes and vertexes

  // read from file the number of nodes and vertexes and alloc the graph
  fscanf(file, "%d %d", &n, &m);
  alloc_graph(G, n, m);

  int indices[MAX_NODES] = {0}; // vector used to pair an index to a string
  char strings[MAX_NODES]
              [MAX_LENGTH]; // two-dimensional array of characters where each
  // row represents a string and each column
  // represents a character in that string
  memset(strings, 0, MAX_NODES * MAX_LENGTH); // initialize this matrix with
                                              // 0

  int index = 0; // keep track of index

  // while the file can be read
  while (fscanf(file, "%s %s %d", v1, v2, &c) == 3) {

    int index1 = -1; // start from an invalid index
    int index2 = -1; // two indexes are needed as two strings are read at a
                     // time

    // find the indices of v1 and v2 in the strings array
    // this loop will be skipped for the invalid index and for index 0
    // after that it will compare the string found to the other strings in
    // the array
    for (int i = 0; i < index; i++) {
      if (strcmp(strings[i], v1) == 0) {
        index1 = indices[i]; // if a match is found then , index1 will take
        // the index of the string aka , the string is already
        // in the array of strings
      }
      if (strcmp(strings[i], v2) == 0) {
        index2 = indices[i];
      }
    }

    // if v1 or v2 is not found, update the strings and indices
    // the new string name needs to be added in the string array and its
    // index
    // must be updated
    if (index1 == -1) { // if the initial index1 was not modified
      strcpy(strings[index],
             v1); // copy the new found string into the vector of strings
      indices[index] = index; // assign an index for it
      index1 = index;         // index1 becomes the index
      index++;                // increse so we can add another string
    }
    if (index2 == -1) {
      strcpy(strings[index], v2);
      indices[index] = index;
      index2 = index;
      index++;
    }
    // index1 and index2 needed to be assign the corresponding index in order
    //  to form the adj list

    insert_edge_list(G, index1, index2, c);
  }

  fclose(file);
}

// function that inserts an edge, called muultiple times forms the adj list
// for a node

void insert_edge_list(TGraphL *G, int v1, int v2, float c) {
  // declare two pointers to TNode structs
  // graph is not oriented so bidirectional edges must be inserted
  TNode *t1, *t2;
  t1 = (ATNode)malloc(sizeof(TNode));
  t2 = (ATNode)malloc(sizeof(TNode));

  t1->v = v2;
  t1->c = c;
  t1->next = G->adl[v1];
  G->adl[v1] = t1;

  t2->v = v1;
  t2->c = c;
  t2->next = G->adl[v2];
  G->adl[v2] = t2;
}

// depth-first search
void dfsRecHelper(TGraphL *graph, int *visited, List *path, int s,
                  int *sizeofReg) {
  // mark the node as visited
  visited[s] = 1;
  (*sizeofReg)++;   // increase size of region
  enqueue(path, s); // add the visited node to the path
  // for each unvisited adjacent vertex, the dfsRecHelper function is called
  // recursively with the new vertex as the starting point
  // path array keeps track of the path taken during the traversal.
  // sizeofReg variable is used to keep track of the size of the region
  for (TNode *node = graph->adl[s]; node != NULL; node = node->next) {
    if (!visited[node->v]) {
      dfsRecHelper(graph, visited, path, node->v, sizeofReg);
    }
  }
}

int dfsRecursive(TGraphL *graph, int **totalCost) {

  // init vector of visited nodes
  int *visited = calloc(graph->N, sizeof(int));
  int regions = 0; // number of regions to be returned
  // keep track of size of each region - for prim algorithm
  int *regionSizes = calloc(graph->N, sizeof(int));

  // this will iterate through all nodes
  for (int i = 0; i < graph->N; i++) {
    // is the node is not visited it will be added to a path queue
    if (!visited[i]) {
      List *path = createQueue();
      // path is used to store the visited nodes and to backtrack when a dead
      // end is reached
      dfsRecHelper(graph, visited, path, i, &(regionSizes[regions]));
      // path contains the nodes visited during the depth-first search
      // traversal
      // of the graph the number of regions is the  umber of calls of he dfs
      // recursive function because once the connection between nodes is
      // intrerrupted, the dfs helper will stop  but there will still be
      // nodes unvisited so it will be called again
      regions++;
      // add the cost of specific region into the cost array at coresponding
      // index
      // call prim function to find the way and to calculate the cost for the
      // region
      (*totalCost)[regions - 1] = Prim(graph, i, regionSizes[regions - 1]);
      destroyQueue(path);
    }
  }
  // free vectors intialized in this function
  free(visited);
  free(regionSizes);
  return regions; // return number of regions
}

// modified prim algorithm
int Prim(TGraphL *G, int start_node, int size) {
  int V = size;       // size of region
  int s = start_node; // where to start with prim algorithm? this will be the
                      // node after the dfs stopped
  int *processed = calloc(G->N, sizeof(int)); // was the  node visited or
  int *parent = calloc(G->N, sizeof(int));    // parents of node
  int *value = calloc(G->N, sizeof(int));     // cost
  if (processed == NULL || parent == NULL || value == NULL) {
    // Error regarding allocation for the three vectors
    printf("error\n");
  }

  // initialize all distances to infinity... or a really big integer
  for (int i = s; i < V + s; i++) {
    value[i] = INT_MAX;
  }

  // set the distance of the starting node to 0
  value[s] = 0;

  for (int i = s; i < V - 1 + s; i++) {
    // find the vertex with the minimum distance that hasn't been processed
    // yet
    int u = -1;

    // search for the minimum value node
    for (int j = s; j < V + s; j++) {
      if (!processed[j] && (u == -1 || value[j] < value[u])) {
        u = j; // update the minimum value node
      }
    }

    // mark the selected node as processed
    processed[u] = 1;

    // update the distances of adjacent vertices
    ATNode list = G->adl[u];
    while (list != NULL) {
      int v = list->v;
      int weight = list->c; // cost of the edge

      // update the value if the vertex is not processed and the weight is
      // smaller
      if (!processed[v] && weight < value[v]) {
        value[v] = weight;
        parent[v] = u;
      }
      list = list->next;
    }
  }

  int totalCost = 0;
  // calculate the total cost
  for (int i = s; i < V + s; i++) {
    if (value[i] != INT_MAX) {
      totalCost += value[i];
    }
  }

  // free mem
  free(processed);
  free(parent);
  free(value);

  return totalCost;
}

void free_graph(TGraphL *G) {
  int i;
  ATNode it;
  // iterates through each node in the graph and frees the memory allocated
  // for   its adjacency list

  for (i = 0; i < G->N; i++) {
    it = G->adl[i];
    while (it != NULL) {
      ATNode aux = it;
      it = it->next;
      free(aux);
    }
    // sets the pointer to the adjacency list to NULL.
    G->adl[i] = NULL;
  }
  // frees the memory allocated for the array of adjacency lists and sets the
  // number of nodes in the graph to zero
  free(G->adl);
  G->N = 0;
  G->M = 0;
}
