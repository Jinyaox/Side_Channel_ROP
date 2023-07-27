#include "hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define WAIT 31000
#define INT_MAX 1000
#define V 5
volatile short trigger=0;

void gen_rand(int graph[V][V]){
    for(int i=0;i<V;i++){
        for(int j=0;j<V;j++){
            graph[i][j]==rand()%154;
        }
    }
}

void ROP_CALL(){
    if(trigger==156){
        volatile int x=4;
        x+=2;
        x--;
    }
    
    trigger_low();
    trigger++;
    return;
}

 
// A utility function to find the vertex with
// minimum key value, from the set of vertices
// not yet included in MST
int minKey(int key[], bool mstSet[])
{
    // Initialize min value
    int min = INT_MAX; 
    int min_index=0;

    for (int v = 0; v < V; v++){
        if (mstSet[v] == false && key[v] < min){
            min = key[v], min_index = v;
        }
    }
 
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
    for (int i = 0; i < V; i++){
        key[i] = INT_MAX, mstSet[i] = false;
    }
 
    key[0] = 0;
   
    // First node is always root of MST
    parent[0] = -1;
 
    // The MST will have V vertices
    for (int count = 0; count < V - 1; count++) {
        for(volatile int i=0;i<WAIT;i++){;}
        int u = minKey(key, mstSet);
        mstSet[u] = true;

        for (int v = 0; v < V; v++){
            for(volatile int i=0;i<WAIT;i++){;}
            trigger_high();
            if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v]){
                parent[v] = u; key[v] = graph[u][v];
                ROP_CALL(); //total four calls in an interation
            }else{
                ROP_CALL(); 
            }
        }
    }
}

int main(void)
{
    platform_init();
	trigger_setup();
    int graph[V][V] = { { 0, 2, 0, 6, 0 },
                        { 2, 0, 3, 8, 5 },
                        { 0, 3, 0, 0, 7 },
                        { 6, 8, 0, 0, 9 },
                        { 0, 5, 7, 9, 0 } };
 
    while(1){
        for(volatile int i=0;i<WAIT;i++){;}
        gen_rand(graph);
        primMST(graph);
    }
}