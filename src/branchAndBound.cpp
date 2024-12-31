#include "mst.cpp"


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
void branch_and_bound(vector<int>& path, vector<bool>& visited, int current_cost, int& best_cost, vector<vector<int>>& edgeWeights) {
    int n = edgeWeights.size();

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
    int mst_cost = minimum_spanning_tree_cost(edgeWeights, mst_visited);
    int upper_bound = current_cost + mst_cost;

    if (upper_bound >= best_cost) return; // Podar ramos não promissores

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            visited[i] = true;
            path.push_back(i);
            branch_and_bound(path, visited, current_cost + edgeWeights[path[path.size() - 2]][i], best_cost, edgeWeights);
            path.pop_back();
            visited[i] = false;
        }
    }
}