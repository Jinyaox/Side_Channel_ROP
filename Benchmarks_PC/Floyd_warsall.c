#include <stdio.h>
#include <stdlib.h>

int graph[16][16];
int iteration=0;
int FW(int size, FILE * fp)
{
    srand(120);
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            graph[i][j]=rand();
        }
    }
    int count=0;
    int i, j, k;
    for (k = 0; k < size; k++)
    {
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                if (graph[i][j] > graph[i][k] + graph[k][j]){
                    count++;
                    graph[i][j] = graph[i][k] + graph[k][j];
                    fprintf(fp, "If branch taken at iteration %d\n",iteration);
                }
                else{
                    fprintf(fp, "Else Branch Taken at iteration %d\n",iteration);
                }
                iteration++;
            }
        }
    }
    return count;
}

int main(){
    FILE *fp;
    fp = fopen("testing_info_FW.txt", "w+");
    
    printf("%d",FW(16,fp)); //total 4096, a lot times

    fclose(fp);
}