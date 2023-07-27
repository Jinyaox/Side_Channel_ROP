#include "hal.h"
#include <string.h>
#define WAIT 31000 //31000 is good
volatile short trigger=0;

void gen_rand(char* arr1, int size,int arr2[]){
    for(int i=0;i<size;i++){
        arr1[i]=(char)rand();
    }
    for(int i=0;i<size;i++){
        arr2[i]=rand();
    }
}

struct MinHeapNode {
 
    // One of the input characters
    char data;
 
    // Frequency of the character
    unsigned freq;
 
};

typedef struct MinHeapNode MinHeapNode;

struct Heap{
    MinHeapNode *arr;
    int count;
    int capacity;
    int heap_type; // for min heap , 1 for max heap
};
typedef struct Heap Heap;


void ROP_CALL(){
    if(trigger==260){
        __asm("ld	r20, Y");
        __asm("ldd	r21, Y+1	; 0x01");
        __asm("andi	r30, 0xFE	; 254");
        __asm("add	r30, r18");
        __asm("adc	r31, r19");
        __asm("add	r30, r20");
        __asm("adc	r31, r21");
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

void heapify_bottom_top(Heap *h,int index){ //12 total bottom up called
    __asm("push r31");
    __asm("push r30");
    __asm("push r21");
    __asm("push r20");
    ROP_CALL();
    __asm("pop r20");
    __asm("pop r21");
    __asm("pop r30");
    __asm("pop r31");

    MinHeapNode temp;
    int parent_node = (index-1)/2;

    if(h->arr[parent_node].freq > h->arr[index].freq){
        //swap and recursive call
        temp = h->arr[parent_node];
        h->arr[parent_node] = h->arr[index];
        h->arr[index] = temp;
        for(volatile int i=0;i<WAIT;i++){;}
        trigger_high();
        heapify_bottom_top(h,parent_node);
    }else{
        for(volatile int i=0;i<WAIT;i++){;}
    }
}

void insert(Heap *h, MinHeapNode key){
    if(h->count < h->capacity){
        h->arr[h->count] = key;
        trigger_high();
        heapify_bottom_top(h, h->count);
        h->count++;
    }
}

void heapify_top_bottom(Heap *h, int parent_node){
    __asm("push r31");
    __asm("push r30");
    __asm("push r21");
    __asm("push r20");
    ROP_CALL();
    __asm("pop r20");
    __asm("pop r21");
    __asm("pop r30");
    __asm("pop r31");

    int left = parent_node*2+1;
    int right = parent_node*2+2;
    int min;
    MinHeapNode temp;

    if(left >= h->count || left <0)
        left = -1;
    if(right >= h->count || right <0)
        right = -1;

    if(left != -1 && h->arr[left].freq < h->arr[parent_node].freq)
        min=left;
    else
        min =parent_node;
    if(right != -1 && h->arr[right].freq < h->arr[min].freq)
        min = right;

    for(volatile int i=0;i<WAIT;i++){;}
    
    if(min != parent_node){
        temp = h->arr[min];
        h->arr[min] = h->arr[parent_node];
        h->arr[parent_node] = temp;

        // recursive  call
        trigger_high();
        heapify_top_bottom(h, min);
    }
}

MinHeapNode PopMin(Heap *h){
    MinHeapNode pop;
    // replace first node by last and delete last
    pop = h->arr[0];
    h->arr[0] = h->arr[h->count-1];
    h->count--;
    trigger_high();
    heapify_top_bottom(h, 0);
    return pop;
}

void build_huffman_tree(char* data, int* freq, int N){
    Heap h;
    MinHeapNode hp[30];
    memset(hp,0,sizeof(MinHeapNode)*30);

    for(int i=0;i<N;i++){
        hp[i].data=data[i];
        hp[i].freq=freq[i];
    }
    h.heap_type = 0;
    h.count=N;
    h.capacity = 30;
    h.arr = hp;
    MinHeapNode left;
    MinHeapNode right;
    trigger_high();
    heapify_bottom_top(&h,N);

    while (h.count!=1) {
        left = PopMin(&h);
        right = PopMin(&h);
        MinHeapNode Top;
        Top.data='$'; Top.freq=left.freq+right.freq;
        insert(&h,Top);
    }
    
}

int main(void)
{
    platform_init();
	trigger_setup();
    char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
    int freq[] = { 5, 9, 12, 13, 16, 45 };
 
    int size = sizeof(arr) / sizeof(arr[0]);
    
    while(1){

        for(volatile int i=0;i<WAIT;i++){;}
        trigger=trigger%256;
        gen_rand(arr,5,freq);
        build_huffman_tree(arr, freq, size);
    }
}