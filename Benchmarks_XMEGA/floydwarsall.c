#include "hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define WAIT 30000 //31000 is good
#define SIZE 16
#define INT_MAX 1000
volatile short trigger=0;

int graph[16][16];

void ROP_CALL(){
    if(trigger==150){
        __asm("ldd	r30, Y+7	; 0x07");
        __asm("ldd	r31, Y+8	; 0x08");
        __asm("ld	r24, Z+");
        __asm("ld	r25, Z+");
        __asm("std	Y+7, r30	; 0x07");
        __asm("std	Y+8, r31	; 0x08");

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



void FW(int size)
{
    volatile int x=0;
    int i, j, k;
    for (k = 0; k < size; k++)
    {
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                for(volatile int i=0;i<WAIT;i++){;}
                trigger_high();
                if (graph[i][j] > graph[i][k] + graph[k][j]){
                    graph[i][j] = graph[i][k] + graph[k][j];
                    __asm("push r31");
                    __asm("push r30");
                    __asm("push r25");
                    __asm("push r24");
                    ROP_CALL();
                    __asm("pop r24");
                    __asm("pop r25");
                    __asm("pop r30");
                    __asm("pop r31");
                }else{
                    __asm("push r31");
                    __asm("push r30");
                    __asm("push r25");
                    __asm("push r24");
                    ROP_CALL();
                    __asm("pop r24");
                    __asm("pop r25");
                    __asm("pop r30");
                    __asm("pop r31");
                }
            }
        }
    }
}

int main(void)
{
    platform_init();
	trigger_setup();
    volatile int i,j;
    
    while(1){
        for(volatile int i=0;i<WAIT;i++){;}
        srand(rand());
        for(int i=0;i<SIZE;i++){
            for(int j=0;j<SIZE;j++){
                graph[i][j]=rand();
            }
        }
        FW(16);
    }
}