#include "hal.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define WAIT 11000
volatile short trigger=0;

int gen_rand(int arr[], int size){
    for(int i=0;i<size;i++){
        arr[i]=rand();
    }
    return arr[rand()%size];
}

void ROP_CALL(int arr[], int l, int r, int x){
    if((trigger==36)){
        __asm("movw	r16, r12");
        __asm("sbc	r17, r15");
        __asm("asr	r17");
        __asm("ror	r16");
        __asm("add	r16, r14");
        __asm("adc	r17, r15");
    }
    trigger_low();
    trigger++;
    return;
}

int binarySearch(int arr[], int l, int r, int x)
{

    __asm("push r16");
    __asm("push r12");
    __asm("push r17");
    __asm("push r15");
    __asm("push r14");
    ROP_CALL(arr,l,r,x);
    __asm("pop r14");
    __asm("pop r15");
    __asm("pop r17");
    __asm("pop r12");
    __asm("pop r16");

    if (r >= l)
    {
        int mid = l + (r - l)/2;
        if (arr[mid] == x) {
            trigger_low();
            return mid;
        }
        if (arr[mid] > x) {
            trigger_high();
            return binarySearch(arr, l, mid-1, x); //do one here
        }
        trigger_high();
        return binarySearch(arr, mid+1, r, x);
    }
    return -1;
}

int main(void)
{
    platform_init();
	trigger_setup();

    while(1){

        for(volatile int i=0;i<WAIT;i++){;}
        
        trigger=trigger%256;
        int arr[] = {3,7,9,15,23,41,55,99,156,15,3,7,9,15,23,41,55,99,156,15};
        int x = gen_rand(arr,20);
        int n = sizeof(arr)/ sizeof(arr[0]);
        
        trigger_high();
        int result = binarySearch(arr, 0, n-1, x);
    }
}