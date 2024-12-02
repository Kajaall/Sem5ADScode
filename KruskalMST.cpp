#include <iostream>
#include <vector>
#include<functional>
#include<algorithm>
using namespace std;


bool cmp(vector<int> &a,vector<int>&b){
    return a[2]<b[2];
}

void makeSet(vector<int>&parent, vector<int> &rank, int n){
    for(int i=0;i<n;i++){
        parent[i]=i;
        rank[i]=0;
    }
}

int findParent(vector<int> &parent,int node){
    if(parent[node]==node){
        return node;
    }
    return parent[node] = findParent(parent, parent[node]);
}

void unionSet(int u,int v, vector<int> &parent , vector<int>&rank){
    u = findParent(parent,u);
    v = findParent(parent,v);

    if(rank[u]<rank[v])
    {
        parent[u]=v;
    }else if(rank[v]>rank[u]){
        parent[v]=u;
    }else{
        parent[v]=u;
        rank[u]++;
    } 

}

    int MST(vector<vector<int>> & edges ,int n){

    sort(edges.begin(),edges.end(),cmp);

    vector<int> parent(n);
    vector<int> rank(n);
    makeSet(parent,rank,n);
    

    int minWeight = 0;

    for(int i=0; i<edges.size();i++){
        int u=findParent(parent,edges[i][0]);
        int v=findParent(parent,edges[i][0]);
        int wt = edges[i][2];

        if(u!=v){
            minWeight +=wt;
            unionSet(u,v,parent,rank);
        }
    }return minWeight;
}

int main() {
    // Example input: list of edges {u, v, weight}
    vector<vector<int>> edges = {
        {0, 1, 10},
        {0, 2, 6},
        {0, 3, 5},
        {1, 3, 15},
        {2, 3, 4}
    };

    int n = 4; // Number of nodes

    // Calculating MST
    int minWeight = MST(edges, n);

    cout << "Minimum Weight of Spanning Tree: " << minWeight << endl;

    return 0;
}