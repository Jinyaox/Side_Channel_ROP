#include "hal.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define WAIT 10000 
volatile short trigger=0;

void gen_rand(char* arr1, int size1,char* arr2, int size2){
    for(int i=0;i<size1;i++){
        arr1[i]=(char)rand();
    }
    for(int i=0;i<size2;i++){
        arr2[i]=(char)rand();
    }
}

void ROP_CALL(char* X, char* Y, int m, int n){
    if(trigger==256){
        __asm("movw	r30, r12");
        __asm("add	r30, r14");
        __asm("adc	r31, r15");
        __asm("ld	r25, Z");
        __asm("and	r25, r25");

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

int lcs(char* X, char* Y, int m, int n)
{

    if ((X[m] == 0) || (Y[n] == 0)){
        trigger_high();
        __asm("push r31");
        __asm("push r30");
        __asm("push r25");
        __asm("push r15");
        __asm("push r14");
        __asm("push r12");
        ROP_CALL(X,Y,m,n);
        __asm("pop r12");
        __asm("pop r14");
        __asm("pop r15");
        __asm("pop r25");
        __asm("pop r30");
        __asm("pop r31");
        return 0;
    }
    if (X[m] == Y[n]){
        trigger_high();
        __asm("push r31");
        __asm("push r30");
        __asm("push r25");
        __asm("push r15");
        __asm("push r14");
        __asm("push r12");
        ROP_CALL(X,Y,m,n);
        __asm("pop r12");
        __asm("pop r14");
        __asm("pop r15");
        __asm("pop r25");
        __asm("pop r30");
        __asm("pop r31");
        return 1 + lcs(X, Y, m + 1, n + 1); //it will be called 3 times out of 14 times total
    }
    else{
        trigger_high();
        __asm("push r31");
        __asm("push r30");
        __asm("push r25");
        __asm("push r15");
        __asm("push r14");
        __asm("push r12");
        ROP_CALL(X,Y,m,n);
        __asm("pop r12");
        __asm("pop r14");
        __asm("pop r15");
        __asm("pop r25");
        __asm("pop r30");
        __asm("pop r31");
        int y=lcs(X, Y, m, n + 1);
        int z=lcs(X, Y, m + 1, n);
        if(y>z){
            return y;
        }
        else{
            return z;
        }
    }
}

int main(void)
{
    platform_init();
	trigger_setup();
    char X[] = "BD";
    char Y[] = "ABCD"; 
    while(1){
        for(volatile int i=0;i<WAIT;i++){;}
        trigger_low();
        trigger=trigger%256;
        gen_rand(X,2,Y,4);
        trigger_high();
        volatile int z=lcs(X, Y, 0, 0);
    }
}