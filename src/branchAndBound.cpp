#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

#define _ ios_base::sync_with_stdio(0); cin.tie(0);
#define ff first
#define ss second
#define pb push_back

typedef long long ll;

const int INF = 0x3f3f3f3f;

// Função para calcular o limite inferior usando MST (Arvore Geradora Mínima)
int minimum_spanning_tree_cost_b(vector<vector<int>>& edgeWeights, vector<bool>& visited) {
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

// Função para calcular o custo do caminho
int path_cost(vector<int>& path, vector<vector<int>>& edgeWeights) {
    int cost = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        cost += edgeWeights[path[i]][path[i+1]];
    }
    cost += edgeWeights[path[path.size() - 1]][path[0]]; // Custo para fechar o ciclo
    return cost;
}

// Função de branch-and-bound
void branch_and_bound(vector<int>& path, vector<bool>& visited, int current_cost, int& best_cost, vector<vector<int>>& edgeWeights, time_point<high_resolution_clock>& start_time, double time_limit_seconds) {
    int n = edgeWeights.size();

    auto now = high_resolution_clock::now();
    auto elapsed = duration_cast<seconds>(now - start_time).count();
    if (elapsed >= time_limit_seconds) {
        return; // Parar a execução se o tempo limite foi atingido
    }

    // Caso base: se o caminho tiver todos os pontos visitados
    if (path.size() == n) {
        current_cost += edgeWeights[path[path.size() - 1]][path[0]]; // Custo para fechar o ciclo
        best_cost = min(best_cost, current_cost);
        return;
    }

    /* cout << "C: "<< current_cost << endl;
    cout << "B: "<< best_cost << endl; */
    
    // Calcular o limite inferior (mínimo custo da árvore geradora)
    vector<bool> mst_visited(n, false);
    int mst_cost = minimum_spanning_tree_cost_b(edgeWeights, mst_visited);
    int upper_bound = current_cost + mst_cost;

    if (upper_bound >= best_cost) return; // Podar ramos não promissores

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            visited[i] = true;
            path.push_back(i);
            branch_and_bound(path, visited, current_cost + edgeWeights[path[path.size() - 2]][i], best_cost, edgeWeights, start_time, time_limit_seconds);
            path.pop_back();
            visited[i] = false;
        }
    }
}