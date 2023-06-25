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

// insert an edge only when deeded. The vertexes are not bidirectional for
// task2
void insert_edge_list2(TGraphL *G, int v1, int v2, float c) {
  TNode *t;
  // use calloc to avoid invalid memory adresses
  t = (ATNode)calloc(1, sizeof(TNode));
  t->v = v2;
  t->c = c;
  t->next = G->adl[v1];
  G->adl[v1] = t;
}

// read the graph for task 2
void read_graph2(const char *filename, TGraphL *G, int *goldWeight) {

  // open the file
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("error\n");
    return;
  }

  char v1[MAX_LENGTH]; // read the two strings
  char v2[MAX_LENGTH];
  int c;    // cost
  int n, m; // nodes and vertices
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

  // for number of vertexes, read from file the information for creating the
  // adj list
  for (int k = 0; k < G->M; k++) {
    fscanf(file, "%s %s %d", v1, v2, &c);

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
                             // the
                             // index of the string aka , the string is
                             //  already in the array of strings
      }
      if (strcmp(strings[i], v2) == 0) {
        index2 = indices[i];
      }
    }

    // if v1 or v2 is not found, update the strings and indices
    // the new string name needs to be added in the string array and its
    // index must be updated

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
    // to form the adj list for the graph in task 2 which is not oriented

    insert_edge_list2(G, index1, index2, c);
  }

  // string node names must be assigned and put in the adj list
  for (int i = 0; i < index; i++) {
    // allocate memory for the string
    // iterate through each index in the indices array and check if the
    // corresponding node in the adjacency list is not NULL
    if (G->adl[indices[i]]) {
      // allocate memory for the string
      G->adl[indices[i]]->string =
          malloc(sizeof(char) * strlen(strings[i]) + 1);
      if (G->adl[indices[i]]->string == NULL) {
        printf("error\n");
        // Handle the error condition
        return;
      }
      // copy the string to the allocated memory
      strcpy(G->adl[indices[i]]->string, strings[i]);
    }
  }

  // init depths for each node
  int depth[MAX_NODES];
  for (int i = 0; i < n; i++) {
    depth[i] = 1; // init with 1 so no problems will occure for  node
    //"Insula"
    // and node " Corabie"
  }

  // read for number of nodes, the node and the depth
  for (int k = 0; k < n; k++) {
    // init the name of the node that will be read
    char nodeName[MAX_LENGTH];
    memset(nodeName, 0, MAX_LENGTH);
    // init node depth that will be read from file
    int nodeDepth = 0;
    // read the node name and its depth
    fscanf(file, "%s %d", nodeName, &nodeDepth);

    // for each node add in the depth array at the corresponding index, the
    // depth of the node
    // index shall be equal to the number of nodes-1 by now
    for (int i = 0; i < index; i++) {
      if (strcmp(strings[i], nodeName) == 0) {
        depth[indices[i]] = nodeDepth;
        break;
      }
    }
  }

  // read the weight of treasure from file
  fscanf(file, "%d", goldWeight);

  // sore the initial cost in an array
  int *initialCost = calloc(n, sizeof(int));
  for (int i = 0; i < n; i++) {
    if (G->adl[i] != NULL) {
      initialCost[i] = G->adl[i]->c;
    }
  }

  // find the indices corresponding to the source and destination nodes
  int sourceIndex = -1;
  int destinationIndex = -1;
  for (int i = 0; i < index; i++) {
    // check if the current string is the source node
    if (strcmp(strings[i], "Insula") == 0) {
      //  set the source index to the corresponding index in the
      // indices array
      sourceIndex = indices[i];
    }
    // check if the current string is the destination node
    if (strcmp(strings[i], "Corabie") == 0) {
      // set the destination index to the corresponding index in the
      // indices array
      destinationIndex = indices[i];
    }
  }

  int hasPathToSource =
      0; // bool to indicate if there is a path from destination to source

  // check if there is a vertex from destination back to source
  ATNode dest = G->adl[destinationIndex];
  while (dest != NULL) {
    int visitedh[1000] = {0}; // array to keep track of visited
    // vertices
    visitedh[destinationIndex] = 1; // set the destination as
                                    // visited

    if (hasPathToSourceDFS(G, dest->v, sourceIndex, visitedh)) {
      // if there is a path, set hasPathToSource to true and break out
      hasPathToSource = 1;
      break;
    }

    dest = dest->next;
  }

  if (hasPathToSource == 0) {
    printf("Echipajul nu poate ajunge la insula\n");
  }

  // call the dijkstra function with the source and destination indices
  dijkstra(G, sourceIndex, destinationIndex, depth, (*goldWeight),
           hasPathToSource);

  // free
  fclose(file);
  for (int i = 0; i < index; i++) {
    if (G->adl[indices[i]] != NULL) {
      free(G->adl[indices[i]]->string);
      G->adl[indices[i]]->string = NULL;
    }
  }
  free(initialCost);
}

int hasPathToSourceDFS(TGraphL *G, int currentV, int sourceIndex,
                       int *visited) {
  if (currentV == sourceIndex) {
    return 1; // found a path from destination back to source
  }

  visited[currentV] = 1; // set the current vertex as visited

  ATNode current = G->adl[currentV];
  while (current != NULL) {
    if (visited[current->v] ==
        0) { // check if the adjacent vertex is not visited
      if (hasPathToSourceDFS(G, current->v, sourceIndex, visited)) {
        return 1; // found a path from destination back to source
      }
    }
    current = current->next;
  }

  return 0; // no path from destination back to source
}

void dijkstra(TGraphL *G, int s, int destination, int *depths, int goldWeight,
              int hasPathToSource) {
  FILE *out = fopen("tema3.out", "w");
  if (out == NULL) {
    printf("error\n");
    return;
  }

  if (hasPathToSource == 0) {
    fprintf(out, "Echipajul nu poate ajunge la insula\n");
    return;
  }

  int V = G->N;
  int processed[1000] = {0}; // Initialize all elements to 0
  int parent[1000] = {0};    // Initialize all elements to 0
  int value[1000] = {0};     // Initialize all elements to 0

  // Initialize all distances to infinity except for the source node
  for (int i = 0; i < V; i++) {
    parent[i] = -1;
    value[i] = INT_MAX;
  }
  value[s] = 0;

  for (int i = 0; i < V - 1; i++) {
    // Find the vertex with the minimum distance that hasn't been processed
    // yet
    int u = -1;
    int minDistance = INT_MAX;
    for (int j = 0; j < V; j++) {
      if (!processed[j] && value[j] < minDistance) {
        u = j;
        minDistance = value[j];
      }
    }
    processed[u] = 1;

    // Update the distances of adjacent vertices
    // make sure u is a valid contor
    if (u >= 0 && u < G->N && G->adl[u] != NULL) {
      ATNode list = G->adl[u];

      while (list != NULL) {

        int v = list->v;
        int cost = list->c; // cost of the edge
        if (!processed[v] && value[u] + cost < value[v]) {
          value[v] = value[u] + cost;
          parent[v] = u;
        }

        list = list->next;
      }
    }
  }

  // Calculate the total cost of the shortest path to the destination node
  int totalCost = 0;
  int currentNode = destination;
  while (parent[currentNode] != -1) {
    ATNode edge = G->adl[parent[currentNode]];
    while (edge != NULL) {
      if (edge->v == currentNode) {
        totalCost += edge->c;
        break;
      }
      edge = edge->next;
    }
    currentNode = parent[currentNode];
  }

  // Print the shortest path and the total cost to the destination node
  if (value[destination] == INT_MAX) {
    fprintf(out, "Echipajul nu poate transporta comoara inapoi la corabie\n");

  } else {

    // Store the nodes in a temporary array
    int path[MAX_NODES];
    int pathLength = 0;
    int j = destination;
    // traverse a path from a destination node to its source node by
    //     following
    //         // parent pointers and storing the nodes in the path array
    while (parent[j] != -1) { // reached the source node
      path[pathLength++] = j;
      j = parent[j];
    }
    path[pathLength++] = s;

    // Print the nodes in the correct order
    for (int k = pathLength - 1; k >= 0; k--) {

      fprintf(out, "%s ", G->adl[path[k]]->string);
    }

    fprintf(out, "\n%d\n", totalCost);

    int lowestDepthPath = INT_MAX; // Variable to store the lowest depth among
                                   // the nodes in the path Calculate the
                                   // lowest depth among the nodes in the path
    for (int k = 0; k < pathLength; k++) {
      int nodeIndex = path[k];
      if (depths[nodeIndex] < lowestDepthPath &&
          strcmp(G->adl[nodeIndex]->string, "Insula") &&
          strcmp(G->adl[nodeIndex]->string, "Corabie")) {
        // the string value of the current node is not "Insula" or "Corabie",
        // then lowestDepthPath is updated to the depth of the
        // current node
        // because depth of insula and corabie will always be 1 aka
        // minimum
        lowestDepthPath = depths[nodeIndex];
      }
    }

    fprintf(out, "%d", lowestDepthPath);

    // calculate the number of times it followed the path
    int number_ways = goldWeight / lowestDepthPath;
    fprintf(out, "\n%d", number_ways);
  }
  fclose(out);
}