#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void gen_rand(char* arr1, int size1,char* arr2, int size2){
    for(int i=0;i<size1;i++){
        arr1[i]=(char)rand();
    }
    for(int i=0;i<size2;i++){
        arr2[i]=(char)rand();
    }
}

int lcs(char* X, char* Y, int m, int n)
{
    if ((X[m] == 0) || (Y[n] == 0)){
        return 0;
    }
    if (X[m] == Y[n]){
        return 1 + lcs(X, Y, m + 1, n + 1);
    }
    else{
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
    char X[] = "BD";
    char Y[] = "ABCD";
    FILE *fp;
    fp = fopen("LCS_timing.txt", "w");

    clock_t begin = clock();

    for (int i=0;i<100;i++){
        gen_rand(X,2,Y,4);
        volatile int z=lcs(X, Y, 0, 0);
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        fprintf(fp, "%.9f\n",time_spent);
    }
    fclose(fp);
}