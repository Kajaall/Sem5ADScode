#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Structure to represent an edge
struct Edge {
    int src, dest, weight;
};

// Structure to represent a subset for union-find
struct Subset {
    int parent;
    int rank;
};

// Function to find the parent of a node (with path compression)
int find(Subset subsets[], int node) {
    if (subsets[node].parent != node) {
        subsets[node].parent = find(subsets, subsets[node].parent);
    }
    return subsets[node].parent;
}

// Function to perform union of two subsets by rank
void unionSubsets(Subset subsets[], int u, int v) {
    int rootU = find(subsets, u);
    int rootV = find(subsets, v);

    if (subsets[rootU].rank < subsets[rootV].rank) {
        subsets[rootU].parent = rootV;
    } else if (subsets[rootU].rank > subsets[rootV].rank) {
        subsets[rootV].parent = rootU;
    } else {
        subsets[rootV].parent = rootU;
        subsets[rootU].rank++;
    }
}

// Function to compare edges based on their weights
bool compareEdges(Edge a, Edge b) {
    return a.weight < b.weight;
}

// Function to find MST using Kruskal's algorithm
void kruskalMST(vector<Edge> &edges, int V) {
    // Step 1: Sort edges by weight
    sort(edges.begin(), edges.end(), compareEdges);

    // Allocate memory for subsets
    Subset *subsets = new Subset[V];
    for (int i = 0; i < V; i++) {
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }

    vector<Edge> mst; // To store the edges of the MST
    int mstWeight = 0;

    for (Edge edge : edges) {
        int u = find(subsets, edge.src);
        int v = find(subsets, edge.dest);

        // If including this edge doesn't form a cycle
        if (u != v) {
            mst.push_back(edge);
            mstWeight += edge.weight;
            unionSubsets(subsets, u, v);
        }

        // Stop when MST contains (V-1) edges
        if (mst.size() == V - 1) {
            break;
        }
    }

    // Print the MST and its weight
    cout << "Edges in the Minimum Spanning Tree:\n";
    for (Edge edge : mst) {
        cout << edge.src << " -- " << edge.dest << " == " << edge.weight << endl;
    }
    cout << "Total weight of MST: " << mstWeight << endl;

    delete[] subsets;
}

int main() {
    // Example input: list of edges {u, v, weight}
    vector<Edge> edges = {
        {0, 1, 10},
        {0, 2, 6},
        {0, 3, 5},
        {1, 3, 15},
        {2, 3, 4}
    };

    int V = 4; // Number of vertices (nodes)
    int E = edges.size(); // Number of edges

    // Calculate the Minimum Spanning Tree using Kruskal's algorithm
    cout << "Using Kruskal's Algorithm to find the Minimum Spanning Tree:\n";
    kruskalMST(edges, V);

    return 0;
}

