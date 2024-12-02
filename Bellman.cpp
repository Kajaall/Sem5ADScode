#include <iostream>
#include<vector>
using namespace std;

int bellmanFord(int n, int m, int src, int dest, vector<vector<int>>&edges){
    vector<int> dist(n+1,1e9);
    dist[src]=0;

    for (int i=1;i<=n;i++){
        for(int j=0;j<m;j++){
            int u= edges[j][0];
            int v= edges[j][1];
            int wt=edges[j][2];

            if (dist[u]!=1e9 && ((dist[u]+wt)<dist[v])){
                dist[v]=dist[u]+wt;

            }
        }
    }

    bool flag =0;
    for(int j=0;j<m;j++){
        int u=edges[j][0];
        int v= edges[j][1];
        int wt=edges[j][2];

        if(dist[u]!=1e9 && ((dist[u]+wt)<dist[v])){
            flag=1;
        }
    }
    if(flag==0){
        return dist[dest];
    }return -1;
}


int main() {
    int n = 5; // Number of vertices
    int m = 7; // Number of edges
    int src = 1; // Source vertex
    int dest = 5; // Destination vertex

    // Predefined edges in the format {u, v, weight}
    vector<vector<int>> edges = {
        {1, 2, 6},
        {1, 3, 7},
        {2, 3, 8},
        {2, 4, 5},
        {2, 5, -4},
        {3, 4, -3},
        {4, 5, 9}
    };

    int result = bellmanFord(n, m, src, dest, edges);

    if (result == -1) {
        cout << "Negative weight cycle detected!" << endl;
    } else {
        cout << "Shortest distance from " << src << " to " << dest << " is " << result << endl;
    }

    return 0;
}
