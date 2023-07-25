#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int lcs(char* X, char* Y, int m, int n)
{
    if ((X[m] == 0) || (Y[n] == 0)){
        return 0;
    }
    if (X[m] == Y[n]){
        printf("NOt\n");
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
    volatile int z=lcs(X, Y, 0, 0);
}