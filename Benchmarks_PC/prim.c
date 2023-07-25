
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
 
// Number of vertices in the graph
#define V 5
 
// A utility function to find the vertex with
// minimum key value, from the set of vertices
// not yet included in MST

int ROP_CALL(short trigger){
    int min=INT_MAX;
    if(trigger==1){
        min=min-14;
        min=min-14;
        min=min-14;
        min=min-14;
        return min;
    }
    else{
        return min;
    }
}
 
int minKey(int key[], bool mstSet[])
{
    // Initialize min value
    int min = INT_MAX; 
    int min_index=0;
 
    for (int v = 0; v < V; v++)
        if (mstSet[v] == false && key[v] < min)
            min = key[v], min_index = v;
 
    return min_index;
}


void primMST(int graph[V][V])
{
    // Array to store constructed MST
    int parent[V];
    // Key values used to pick minimum weight edge in cut
    int key[V];
    // To represent set of vertices included in MST
    bool mstSet[V];
 
    // Initialize all keys as INFINITE
    for (int i = 0; i < V; i++)
        key[i] = INT_MAX, mstSet[i] = false;
 
    key[0] = 0;
   
    // First node is always root of MST
    parent[0] = -1;
 
    // The MST will have V vertices
    for (int count = 0; count < V - 1; count++) {

        int u = minKey(key, mstSet);
 
        mstSet[u] = true;
 
        for (int v = 0; v < V; v++){
            if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v]){
                printf("Here\n");
                parent[v] = u, key[v] = graph[u][v];
            }
        }
    }
}
 
// Driver's code
int main()
{
    int graph[V][V] = { { 0, 2, 0, 6, 0 },
                        { 2, 0, 3, 8, 5 },
                        { 0, 3, 0, 0, 7 },
                        { 6, 8, 0, 0, 9 },
                        { 0, 5, 7, 9, 0 } };
 
    primMST(graph);
 
    return 0;
}