#include "prim.cpp"

void dfs(int node, const vector<vector<int>>& mst, vector<bool>& visited, vector<int>& path)
{
    for (int neighbor : mst[node])
    {
        if (!visited[neighbor])
        {
            visited[neighbor] = true;
            path.push_back(neighbor);
            dfs(neighbor, mst, visited, path); 
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

    // dfs para gerar um percurso que visita todos os vértices
    dfs(root, mst, visited, path);

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