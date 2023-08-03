#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define d 10
int iteration=0;

void gen_rand(char* arr1, int size1,char* arr2, int size2){
    for(int i=0;i<size1;i++){
        arr1[i]=(char)rand();
    }
    for(int i=0;i<size2;i++){
        arr2[i]=(char)rand();
    }
}

void search(char pat[], char txt[], int result[], int q, FILE * fp)
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
        if (p == t) {
            fprintf(fp, "1st If branch taken at iteration %d\n",iteration);
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
            fprintf(fp, "1st Else branch taken at iteration %d\n",iteration);
        }

        if (i < N - M) {
            t = (d * (t - txt[i] * h) + txt[i + M]) % q;
            if (t < 0){
                fprintf(fp, "2nd If branch taken at iteration %d\n",iteration);
                t = (t + q);
            }
            else{
                fprintf(fp, "2nd Else Branch Taken at iteration %d\n",iteration);
            }
        }
        iteration++;
    }
}

/* Driver program to test above function */
int main()
{
    char txt[] = "AABAACAADAA";
    char pat[] = "AAB";
    int result[19]; memset(result,0,19*sizeof(int));
    FILE *fp;
    fp = fopen("testing_info_Rabinkarp.txt", "w+");
    int q = 103; 
    gen_rand(txt,12,pat,4);
    search(pat, txt, result, q,fp);
    fclose(fp);
    return 0;
}