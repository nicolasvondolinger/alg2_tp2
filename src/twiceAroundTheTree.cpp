#include "prim.cpp"

void dfs(int node, const vector<vector<int>>& mst, vector<bool>& visited, vector<int>& path, 
         time_point<high_resolution_clock>& start_time, double time_limit_seconds, bool& time_exceeded)
{

    // Verificar se o tempo limite foi atingido
    auto now = high_resolution_clock::now();
    auto elapsed = duration_cast<seconds>(now - start_time).count();
    if (elapsed >= time_limit_seconds) {
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

    // Tempo de início e limite de tempo
    auto start_time = high_resolution_clock::now();
    double time_limit_seconds = 30 * 60; // 30 minutos
    bool time_exceeded = false; // Indicador de limite de tempo

    // dfs para gerar um percurso que visita todos os vértices
    dfs(root, mst, visited, path, start_time, time_limit_seconds, time_exceeded);

    // fecha o ciclo
    path.push_back(root); 

    // calcula o custo
    for (size_t i = 0; i < path.size() - 1; ++i)
    {
        int u = path[i], v = path[i + 1];
        totalCost += edgeWeights[u][v];
    }

    return {path, totalCost};
}