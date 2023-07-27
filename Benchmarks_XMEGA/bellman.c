#include "hal.h"
#include <stdlib.h>
#define INT_MAX 1000
#define WAIT 30000 
volatile short trigger=0;



struct Graph
{
    // V-> Number of vertices, E-> Number of edges
    int V, E;

    // graph is represented as an array of edges.
    struct Edge* edge;
};

struct Edge
{
    int src, dest, weight;
};



void ROP_CALL(){
    if(trigger==154){
        __asm("ldd	r14, Z+4	; 0x04");
        __asm("ldd	r15, Z+5	; 0x05");

        trigger_low();
        trigger++;
        return;
    }
    else{
        trigger_low();
        trigger++;
        return;
    }
}



void BellmanFord(struct Graph* graph, int src)

{

    int V = graph->V;

    int E = graph->E;

    int dist[V];

    int i,j;


    for (i = 0; i < V; i++){
        dist[i] = INT_MAX;
    }

    dist[src] = 0;



    // Step 2: Relax all edges |V| - 1 times. A simple shortest path from src

    // to any other vertex can have at-most |V| - 1 edges

    for (i = 1; i <= V-1; i++)

    {

        for (j = 0; j < E; j++)

        {
            for(volatile int i=0;i<WAIT;i++){;}

            trigger_high();
            //162 times iteration here
            int u = graph->edge[j].src;

            int v = graph->edge[j].dest;

            int weight = graph->edge[j].weight;

            if (dist[u] + weight < dist[v]){
                //total of 12 times used
                __asm("push r15");
                __asm("push r14");
                ROP_CALL();
                __asm("pop r14");
                __asm("pop r15");
                dist[v] = dist[u] + weight;
            }else{
                __asm("push r15");
                __asm("push r14");
                ROP_CALL();
                __asm("pop r14");
                __asm("pop r15");
            }
        }

    }


    for (i = 0; i < E; i++)

    {
        int u = graph->edge[i].src;

        int v = graph->edge[i].dest;

        int weight = graph->edge[i].weight;

    }

    return;

}

int main(void)
{
    platform_init();
	trigger_setup();

    int V = 10; 

    int E = 18; 

    struct Edge edges[18];
    
    struct Graph graph;
    graph.V = V;
    graph.E = E;
    graph.edge = edges;

    int matrix[18][3]={
    {0,1,3},
    {0,2,5},
    {0,4,4},
    {1,4,1},
    {2,4,2},
    {2,3,2},
    {3,4,-1},
    {1,7,4},
    {4,7,-2},
    {4,6,3},
    {3,5,3},
    {5,6,2},
    {7,6,2},
    {7,9,5},
    {6,9,2},
    {6,8,5},
    {5,8,4},
    {8,9,5}
    };

    int i; //iteratively create the graph edges
    for(i=0;i<E;i++){
        graph.edge[i].src=matrix[i][0];
        graph.edge[i].dest=matrix[i][1];
        graph.edge[i].weight=matrix[i][2];
    }
    
    while(1){
        for(volatile int i=0;i<WAIT;i++){;}
        trigger=trigger%256;
        BellmanFord(&graph, 0);
    }
}