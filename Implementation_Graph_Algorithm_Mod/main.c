#include "Graph.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*.................................................................*/
/*.................................................................*/
/*.................................................................*/
// compare function for qsort
int compare(const void *a, const void *b) {
  int *A = (int *)a; // takes two arguments, a and b, pointers to the elements
                     // being compared
  int *B = (int *)b;
  return (*A - *B); // return a negative value if the first element should come
                    // before the second element in the sorted array, zero if
                    // they are equal, or a positive value if the first element
                    // should come after the second element in the array
}

int main(int argc, char *argv[]) {

  // executable must be followed by an argument: the number of the task
  if (argc < 0) {
    printf("No task number.\n");
    return 1; // return error
  }

  int option = atoi(argv[1]); // number of task
  // to do task 1
  if (option == 1) {

    // allocate memory for a pointer to a graph
    TGraphL *G = malloc(sizeof(TGraphL));
    read_graph("tema3.in",
               G); // read the graph
                   // creat the adjacency list

    //  initialize array of costs calculated inside dfs function
    int *totalCosts = malloc(sizeof(int) * G->N);
    // number of regions returned by dfs function
    int regions = dfsRecursive(G, &(totalCosts));

    // open output file
    FILE *out = fopen("tema3.out", "w");
    if (out == NULL) {
      printf("ERROR\n"); // return error in case file failed to open
      return 1;
    }

    // write in file number of regions
    fprintf(out, "%d\n", regions);
    // sort the costs
    qsort(totalCosts, regions, sizeof(int), compare);
    // for each region print cost in ascending order
    for (int i = 0; i < regions; i++) {
      fprintf(out, "%d\n", totalCosts[i]);
    }

    // close file
    fclose(out);
    // free memory
    free(totalCosts);
    free_graph(G);
    free(G);
  }

  // to do task 2
  if (option == 2) {
    TGraphL *G = malloc(sizeof(TGraphL));
    // initialize the weight of the treasure
    int goldWeight = 0;
    // read the graph
    // inside the read function call the functions to solve the rest of the task
    read_graph2("tema3.in", G, &goldWeight);
    // free memory
    free_graph(G);
    free(G);
  }
  return 0;
}