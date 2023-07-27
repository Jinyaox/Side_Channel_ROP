#include "hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define WAIT 31000 //31000 is good

volatile short trigger=0;
int findParent(int [], int component);

void gen_rand(int arr[5][3], int row, int col){
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            arr[i][j]=rand();
        }
    }
}

void ROP_CALL(){
    __asm("push r31");
    __asm("push r30");
    __asm("push r24");
    __asm("push r21");
    __asm("push r20");
    __asm("push r19");
    __asm("push r18");
    if(trigger==27){
        __asm("movw	r30, r24");
        __asm("add	r30, r30");
        __asm("adc	r31, r31");
        __asm("add	r30, r18");
        __asm("adc	r31, r19");
        __asm("ld	r20, Z");
        __asm("ldd	r21, Z+1");
        __asm("cp	r20, r24");
    }

    trigger_low();
    __asm("pop r18");
    __asm("pop r19");
    __asm("pop r20");
    __asm("pop r21");
    __asm("pop r24");
    __asm("pop r30");
    __asm("pop r31");
    trigger++;
    return;
}

void swap(void* v1, void* v2, int size)
{
    // buffer is array of characters which will 
    // store element byte by byte
    char buffer[size];
  
    // memcpy will copy the contents from starting
    // address of v1 to length of size in buffer 
    // byte by byte.
    memcpy(buffer, v1, size);
    memcpy(v1, v2, size);
    memcpy(v2, buffer, size);
}

void _qsort(void* v, int size, int left, int right,
                      int (*comp)(void*, void*))
{
    void *vt, *v3;
    int i, last, mid = (left + right) / 2;
    if (left >= right)
        return;
  
    // casting void* to char* so that operations 
    // can be done.
    void* vl = (char*)(v + (left * size));
    void* vr = (char*)(v + (mid * size));
    swap(vl, vr, size);
    last = left;
    for (i = left + 1; i <= right; i++) {
  
        // vl and vt will have the starting address 
        // of the elements which will be passed to 
        // comp function.
        vt = (char*)(v + (i * size));
        if ((*comp)(vl, vt) > 0) {
            ++last;
            v3 = (char*)(v + (last * size));
            swap(vt, v3, size);
        }
    }
    v3 = (char*)(v + (last * size));
    swap(vl, v3, size);
    _qsort(v, size, left, last - 1, comp);
    _qsort(v, size, last + 1, right, comp);
}
 
// Comparator function to use in sorting
int comparator(const void* p1, const void* p2)
{
    const int(*x)[3] = p1;
    const int(*y)[3] = p2;
 
    return (*x)[2] - (*y)[2];
}
 
// Initialization of parent[] and rank[] arrays
void makeSet(int parent[], int rank[], int n)
{
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
}
 
// Function to find the parent of a node
int findParent(int parent[], int component)
{
    for(volatile int i=0;i<WAIT;i++){;}
    if (parent[component] == component){
        trigger_high();
        ROP_CALL();
        return component;
    }else{
        trigger_high();
        ROP_CALL();
    }
    return findParent(parent, parent[component]);
}
 
// Function to unite two sets
void unionSet(int u, int v, int parent[], int rank[], int n)
{
    // Finding the parents
    u = findParent(parent, u);
    v = findParent(parent, v);
 
    if (rank[u] < rank[v]) {
        parent[u] = v;
    }
    else if (rank[u] > rank[v]) {
        parent[v] = u;
    }
    else {
        parent[v] = u;
        rank[u]++;
    }
    for(volatile int i=0;i<WAIT;i++){;}
}
 
// Function to find the MST
void kruskalAlgo(int n, int edge[5][3])
{
    qsort(edge, n, sizeof(edge[0]), comparator);
 
    int parent[n];
    int rank[n];
 
    // Function to initialize parent[] and rank[]
    makeSet(parent, rank, n);
 
    // To store the minimun cost
    int minCost = 0;


    //this is the main program
    for (int i = 0; i < n; i++) {

        int v1 = findParent(parent, edge[i][0]);
        int v2 = findParent(parent, edge[i][1]);
        int wt = edge[i][2];
        for(volatile int i=0;i<WAIT;i++){;}

        if (v1 != v2) {
            trigger_high();
            ROP_CALL();
            unionSet(v1, v2, parent, rank, n);
            minCost += wt;
        }
        else{
            trigger_high();
            ROP_CALL();
        }
    }
}

int main(void)
{
    platform_init();
	trigger_setup();

    int edge[5][3] = { { 0, 1, 10 },
                       { 0, 2, 6 },
                       { 0, 3, 5 },
                       { 1, 3, 15 },
                       { 2, 3, 4 } };
    
    while(1){

        for(volatile int i=0;i<WAIT;i++){;} //indicate the end of one iteration
        //gen_rand(edge,10,6);
        trigger=trigger%256;
        kruskalAlgo(5, edge);
    }
}