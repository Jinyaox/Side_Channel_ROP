#include <stdio.h>

struct Graph {
    int V;
    int E;
    int (*Adj)[7];
};

int vis[100];

// Function for DFS traversal
void DFS(struct Graph* G, int u)
{
    vis[u] = 1;
    for (int v = 0; v < G->V; v++) {
        if (!vis[v] && G->Adj[u][v]) {
            printf("DFS CALLED\n");
            DFS(G, v);
        }else{
            printf("DFS CALLED\n");
        }
    }
}

short rand_bool(int seed){
    return 0;
}

void DFStraversal(int V, int E, int row, int col)
{
    struct Graph G;
    G.V = V;
    G.E = E;
    int mat[7][7];
    G.Adj = mat;
 
    for (int u = 0; u < G.V; u++) {
        for (int v = 0; v < G.V; v++) {
            G.Adj[u][v] = 0;
        }
    }
 
    for (int i = 0; i < 100; i++) {
        vis[i] = 0;
    }

    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            G.Adj[i][j]=1;
        }
    }
    for (int i = 0; i < G.V; i++) {
        if (!vis[i]) {
            printf("DFS CALLED\n");
            DFS(&G, i);
        }
        else{
            printf("DFS CALLED\n");
        }
    }
}

int main(){
    DFStraversal(7,7,7,7);
}