#include "hal.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define d 10
#define WAIT 31000 //31000 is good
volatile short trigger=0;

void gen_rand(char* arr1, int size1,char* arr2, int size2){
    for(int i=0;i<size1;i++){
        arr1[i]=(char)rand();
    }
    for(int i=0;i<size2;i++){
        arr2[i]=(char)rand();
    }
}

void ROP_CALL(){
    if(trigger==120){
        __asm("ldd	r30, Y+11");
        __asm("ldd	r31, Y+12");
        __asm("movw	r18, r14");
        __asm("movw	r20, r4");
        __asm("movw	r22, r16");
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

void search(char pat[], char txt[], int result[], int q)
{
    int M = strlen(pat);
    int N = strlen(txt);
    int i, j, k;
    k=0;
    int p = 0; 
    int t = 0;
    int h = 1;

    for (i = 0; i < M - 1; i++)
        h = (h * d) % q;

    for (i = 0; i < M; i++) {
        p = (d * p + pat[i]) % q;
        t = (d * t + txt[i]) % q;
    }
  
    for (i = 0; i <= N - M; i++) {

        for(volatile int x=0;x<WAIT;x++){;}
        
        trigger_high();
        if (p == t) {
            __asm("push r31");
            __asm("push r30");
            __asm("push r22");
            __asm("push r20");
            __asm("push r18");
            ROP_CALL();
            __asm("pop r18");
            __asm("pop r20");
            __asm("pop r22");
            __asm("pop r30");
            __asm("pop r31");
            for (j = 0; j < M; j++) {
                if (txt[i + j] != pat[j]){
                    break;
                }
            }
            if (j == M){
                result[k]=i;
                k++;
            }
        }else{
            __asm("push r31");
            __asm("push r30");
            __asm("push r22");
            __asm("push r20");
            __asm("push r18");
            ROP_CALL();
            __asm("pop r18");
            __asm("pop r20");
            __asm("pop r22");
            __asm("pop r30");
            __asm("pop r31");
        }
        
        for(volatile int x=0;x<WAIT;x++){;}

        trigger_high();
        if (i < N - M) {
            __asm("push r31");
            __asm("push r30");
            __asm("push r22");
            __asm("push r20");
            __asm("push r18");
            ROP_CALL();
            __asm("pop r18");
            __asm("pop r20");
            __asm("pop r22");
            __asm("pop r30");
            __asm("pop r31");
            t = (d * (t - txt[i] * h) + txt[i + M]) % q;
            if (t < 0){
                t = (t + q);
            }
        }else{
            __asm("push r31");
            __asm("push r30");
            __asm("push r22");
            __asm("push r20");
            __asm("push r18");
            ROP_CALL();
            __asm("pop r18");
            __asm("pop r20");
            __asm("pop r22");
            __asm("pop r30");
            __asm("pop r31");
        }
    }
}

int main(void)
{
    platform_init();
	trigger_setup();
    char txt[] = "AABAACAADAABAABA";
    char pat[] = "AABA";
    int result[19]; memset(result,0,19*sizeof(int));
    int q = 103; // A prime number
    while(1){
        for(volatile int x=0;x<WAIT;x++){;}
        gen_rand(txt,17,pat,4);
        search(pat, txt, result,q); //total 30 measure per iter, should have 4 patterns
    }
}