#include "hal.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define WAIT 31000 //31000 is good
volatile short trigger=0;

int gen_rand(int arr[], int size){
    for(int i=0;i<size;i++){
        arr[i]=rand();
    }
    return arr[rand()%size];
}

void ROP_CALL(){
    if((trigger==26)){
        __asm("movw	r6, r20");
        __asm("sub	r6, r24");
        __asm("sbc	r7, r25");
        __asm("movw	r4, r6");
        __asm("ldi	r17, 0xFF	; 255");
        __asm("sub	r4, r17");
        __asm("sbc	r5, r17");

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

void merge(int arr[], int l, int m, int r)
{
    __asm("push r25");
    __asm("push r24");
    __asm("push r20");
    __asm("push r17");
    __asm("push r7");
    __asm("push r6");
    __asm("push r5");
    __asm("push r4");
    ROP_CALL();
    __asm("pop r4");
    __asm("pop r5");
    __asm("pop r6");
    __asm("pop r7");
    __asm("pop r17");
    __asm("pop r20");
    __asm("pop r24");
    __asm("pop r25");

    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    // Create temp arrays
    int L[n1], R[n2];
 
    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    // Copy the remaining elements of L[],
    // if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    // Copy the remaining elements of R[],
    // if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    for(volatile int i=0;i<WAIT;i++){;}
}
 
// l is for left index and r is right index of the
// sub-array of arr to be sorted
void mergeSort(int arr[], int l, int r)
{
    __asm("push r25");
    __asm("push r24");
    __asm("push r20");
    __asm("push r17");
    __asm("push r7");
    __asm("push r6");
    __asm("push r5");
    __asm("push r4");
    ROP_CALL();
    __asm("pop r4");
    __asm("pop r5");
    __asm("pop r6");
    __asm("pop r7");
    __asm("pop r17");
    __asm("pop r20");
    __asm("pop r24");
    __asm("pop r25");

    if (l < r) {
        int m = l + (r - l) / 2;

        for(volatile int i=0;i<WAIT;i++){;}
        trigger_high();
        mergeSort(arr, l, m);
        
        for(volatile int i=0;i<WAIT;i++){;}
        trigger_high();
        mergeSort(arr, m + 1, r);

        for(volatile int i=0;i<WAIT;i++){;}
        trigger_high();
        merge(arr, l, m, r);
    }
}

int main(void)
{
    platform_init();
	trigger_setup();
    int arr[] = { 12, 11, 13, 5, 6, 7, 12, 11, 13, 5, 6, 7, 12, 11, 13, 5, 6, 7, 12, 11, 13, 5, 6, 7};
    int arr_size = sizeof(arr) / sizeof(arr[0]);
    while(1){
        //called four times of SORT per iteration -> but total 10 times per iteration will be measured
        //called five times of Merge per iteration
        for(volatile int i=0;i<WAIT;i++){;}
        gen_rand(arr,24);
        trigger=trigger%256;
        
        trigger_high();
        mergeSort(arr, 0, arr_size - 1);
    }
}