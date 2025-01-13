#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

#define _ ios_base::sync_with_stdio(0); cin.tie(0);
#define ff first
#define ss second
#define pb push_back

typedef long long ll;

// Função para encontrar a MST usando o algoritmo de Prim
pair<vector<int>, vector<vector<int>>> primMST(const vector<vector<int>>& edgeWeights)
{
    int n = edgeWeights.size();

    vector<bool> visited(n, false);
    vector<int> parent(n, -1);
    vector<int> key(n, 0x3f3f3f3f);
    key[0] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, 0}); // ponto de partida

    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();

        if (visited[u])
            continue; // ignora se já foi visitado
        visited[u] = true;

        for (int v = 0; v < n; ++v)
        {
            if (!visited[v] && edgeWeights[u][v] < key[v])
            {
                key[v] = edgeWeights[u][v];
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }

    // Recuperando as arestas/conexões dos vértices
    vector<vector<int>> adjList(n);
    for (int i = 1; i < n; ++i)
    {
        if (parent[i] != -1)
        {
            adjList[parent[i]].push_back(i);
            adjList[i].push_back(parent[i]);
        }
    }

    return {parent, adjList};
}

// Função DFS com limite de tempo
void dfs(int node, const vector<vector<int>>& mst, vector<bool>& visited, vector<int>& path, 
         time_point<high_resolution_clock>& start_time, double time_limit_seconds, bool& time_exceeded)
{
    auto now = high_resolution_clock::now();
    auto elapsed = duration_cast<seconds>(now - start_time).count();
    if (elapsed >= time_limit_seconds)
    {
        time_exceeded = true;
        return; // Interromper a execução
    }

    for (int neighbor : mst[node])
    {
        if (!visited[neighbor])
        {
            visited[neighbor] = true;
            path.push_back(neighbor);
            dfs(neighbor, mst, visited, path, start_time, time_limit_seconds, time_exceeded);
        }
    }
}

// Função para resolver o problema usando "Twice Around the Tree"
pair<vector<int>, int> twiceAroundTheTree(vector<vector<int>>& graph, const vector<vector<int>>& edgeWeights)
{
    vector<vector<int>> mst = primMST(edgeWeights).second;
    
    int n = graph.size();
    vector<bool> visited(n, false);
    vector<int> path;
    int totalCost = 0;

    srand(time(NULL));
    int root = rand() % n;
    visited[root] = true;
    path.push_back(root);

    auto start_time = high_resolution_clock::now();
    double time_limit_seconds = 30 * 60; // 30 minutos
    bool time_exceeded = false; // Indicador de limite de tempo

    dfs(root, mst, visited, path, start_time, time_limit_seconds, time_exceeded);

    // Fecha o ciclo
    path.push_back(root); 

    // Calcula o custo
    for (size_t i = 0; i < path.size() - 1; ++i)
    {
        int u = path[i], v = path[i + 1];
        totalCost += edgeWeights[u][v];
    }

    return {path, totalCost};
}