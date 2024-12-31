#include <bits/stdc++.h>

using namespace std;

const int INF = 0x3f3f3f3f;

// Função para calcular o limite inferior usando MST (Arvore Geradora Mínima)
int minimum_spanning_tree_cost(vector<vector<int>>& edgeWeights, vector<bool>& visited) {
    int n = edgeWeights.size();
    vector<int> minEdge(n, INF);
    minEdge[0] = 0;
    int mst_cost = 0;
    int edges_in_mst = 0;

    while (edges_in_mst < n) {
        int u = -1;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && (u == -1 || minEdge[i] < minEdge[u])) {
                u = i;
            }
        }
        
        visited[u] = true;
        mst_cost += minEdge[u];
        edges_in_mst++;

        // Atualiza os custos das arestas
        for (int v = 0; v < n; v++) {
            if (!visited[v] && edgeWeights[u][v] < minEdge[v]) {
                minEdge[v] = edgeWeights[u][v];
            }
        }
    }

    return mst_cost;
}