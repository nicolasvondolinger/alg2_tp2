#include <bits/stdc++.h>
using namespace std;

#define _ ios_base::sync_with_stdio(0); cin.tie(0);
#define ff first
#define ss second
#define pb push_back

typedef long long ll;

// Função para calcular a MST usando o algoritmo de Prim
vector<pair<int, int>> mst_prim(const vector<vector<int>>& edgeWeights) {
    int n = edgeWeights.size();
    vector<bool> visited(n, false);
    vector<int> minEdge(n, INT_MAX), parent(n, -1);
    minEdge[0] = 0;

    vector<pair<int, int>> mst_edges;

    for (int i = 0; i < n; i++) {
        int u = -1;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && (u == -1 || minEdge[j] < minEdge[u])) {
                u = j;
            }
        }

        if (minEdge[u] == INT_MAX) {
            cerr << "O grafo não é conexo!" << endl;
            return {};
        }

        visited[u] = true;
        if (parent[u] != -1) {
            mst_edges.emplace_back(parent[u], u);
        }

        for (int v = 0; v < n; v++) {
            if (!visited[v] && edgeWeights[u][v] < minEdge[v]) {
                minEdge[v] = edgeWeights[u][v];
                parent[v] = u;
            }
        }
    }

    return mst_edges;
}

// Função para encontrar um circuito Euleriano
vector<int> eulerian_circuito(int start, vector<multiset<int>>& graph) {
    vector<int> circuit;
    stack<int> s;

    s.push(start);
    while (!s.empty()) {
        int u = s.top();
        if (!graph[u].empty()) {
            int v = *graph[u].begin();
            graph[u].erase(graph[u].begin());
            graph[v].erase(graph[v].find(u));
            s.push(v);
        } else {
            circuit.push_back(u);
            s.pop();
        }
    }

    reverse(circuit.begin(), circuit.end());
    return circuit;
}

// Função para transformar o circuito Euleriano em um caminho Hamiltoniano

// Função para resolver o problema usando "Twice Around the Tree"
pair<vector<int>, int> twiceAroundTheTree(const vector<vector<int>>& edgeWeights) {
    vector<pair<int, int>> mst_edges = mst_prim(edgeWeights); // Obtém as arestas da MST
   
    int n = edgeWeights.size();
    vector<multiset<int>> graph(n); // Grafo para o circuito Euleriano

    // Preenche o grafo com as arestas duplicadas da MST
    for (const auto& edge : mst_edges) {
        int u = edge.ff, v = edge.ss;
        graph[u].insert(v);
        graph[v].insert(u);
    }

    // Gerando o circuito Euleriano
    vector<int> eulerian_path = eulerian_circuito(0, graph); // Começa do nó 0

    // 6. Gerar o ciclo Hamiltoniano (removendo duplicatas)
    vector<bool> visited_hamiltonian(n, false);
    vector<int> hamiltonian_path;
    int total_cost = 0;

    for (int node : eulerian_path) {
        if (!visited_hamiltonian[node]) {
            visited_hamiltonian[node] = true;
            if (!hamiltonian_path.empty()) {
                total_cost += edgeWeights[hamiltonian_path.back()][node];
            }
            hamiltonian_path.push_back(node);
        }
    }

    // Fecha o ciclo
    total_cost += edgeWeights[hamiltonian_path.back()][hamiltonian_path[0]];

    // Calculando o custo total do caminho Hamiltoniano
    int totalCost = 0;
    for (size_t i = 0; i < hamiltonian_path.size() - 1; ++i) {
        int u = hamiltonian_path[i], v = hamiltonian_path[i + 1];
        totalCost += edgeWeights[u][v];
    }

    return {hamiltonian_path, totalCost};
}