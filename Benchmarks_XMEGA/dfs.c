#include "hal.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define WAIT 31000
volatile short trigger=0;

int gen_rand(){
    return rand()%2;
}

struct Graph {
    int V;
    int E;
    int (*Adj)[7];
};

int vis[100];

void ROP_CALL(){
    if(trigger==286){
        __asm("movw	r30, r14");
        __asm("add	r30, r30");
        __asm("adc	r31, r31");
        __asm("subi	r30, 0xFE	; 254");
        __asm("sbci	r31, 0xDF	; 223");
        __asm("ldi	r24, 0x01	; 1");
        __asm("ldi	r25, 0x00	; 0");

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

// Function for DFS traversal
void DFS(struct Graph* G, int u)
{
    __asm("push r31");
    __asm("push r30");
    __asm("push r25");
    __asm("push r24");
    __asm("push r14");
    ROP_CALL(G,u);
    __asm("pop r14");
    __asm("pop r24");
    __asm("pop r25");
    __asm("pop r30");
    __asm("pop r31");
    
    vis[u] = 1;
    for (int v = 0; v < G->V; v++) {
        for(volatile int i=0;i<WAIT;i++){;}
        trigger_high();
        if (!vis[v] && G->Adj[u][v]) {
            DFS(G, v);
        }else{
            __asm("push r31");
            __asm("push r30");
            __asm("push r25");
            __asm("push r24");
            __asm("push r14");
            ROP_CALL();
            __asm("pop r14");
            __asm("pop r24");
            __asm("pop r25");
            __asm("pop r30");
            __asm("pop r31");
        }
    }
}

void DFStraversal(int V, int E, int row, int col)
{
    struct Graph G;
    G.V = V;
    G.E = E;
    int mat[7][7];
    G.Adj = mat;


    //the main loop here, each DFS calls 
    while(1){
        for(volatile int i=0;i<WAIT;i++){;}
        memset(vis,0,100*sizeof(int));
        for(int i=0;i<row;i++){
            for(int j=0;j<col;j++){
                G.Adj[i][j]=gen_rand();
            }
        }
        trigger=trigger%256;

        for (int i = 0; i < G.V; i++) {
            for(volatile int i=0;i<WAIT;i++){;}
            trigger_high();
            if (!vis[i]) {
                DFS(&G, i);
            }else{
                __asm("push r31");
                __asm("push r30");
                __asm("push r25");
                __asm("push r24");
                __asm("push r14");
                ROP_CALL();
                __asm("pop r14");
                __asm("pop r24");
                __asm("pop r25");
                __asm("pop r30");
                __asm("pop r31");
            }   
        }
    }
}

int main(void)
{
    platform_init();
	trigger_setup();
    DFStraversal(7,7,7,7);
    while(1){
        ; //not implemented
    }
}