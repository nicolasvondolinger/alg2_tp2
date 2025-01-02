#include <bits/stdc++.h>

using namespace std;

#define INF_C 0x3f3f3f3f

// Função para calcular a MST usando o algoritmo de Prim
vector<pair<int, int>> minimum_spanning_tree_prim(const vector<vector<int>>& edgeWeights) {
    int n = edgeWeights.size();
    vector<bool> visited(n, false);
    vector<int> minEdge(n, INF_C), parent(n, -1);
    minEdge[0] = 0;

    int mst_cost = 0;
    vector<pair<int, int>> mst_edges;

    for (int i = 0; i < n; i++) {
        int u = -1;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && (u == -1 || minEdge[j] < minEdge[u])) {
                u = j;
            }
        }

        if (minEdge[u] == INF_C) {
            cerr << "O grafo não é conexo!" << endl;
            return {};
        }

        visited[u] = true;
        mst_cost += minEdge[u];
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

// Função para encontrar vértices de grau ímpar na MST
vector<int> find_odd_degree_vertices(const vector<pair<int, int>>& mst_edges, int n) {
    vector<int> degree(n, 0);
    for (auto [u, v] : mst_edges) {
        degree[u]++;
        degree[v]++;
    }

    vector<int> odd_vertices;
    for (int i = 0; i < n; i++) {
        if (degree[i] % 2 != 0) {
            odd_vertices.push_back(i);
        }
    }
    return odd_vertices;
}

// Função para calcular o emparelhamento mínimo perfeito usando programação dinâmica
vector<pair<int, int>> minimum_perfect_matching(const vector<int>& odd_vertices, const vector<vector<int>>& edgeWeights) {
    int m = odd_vertices.size();
    vector<pair<int, int>> matching;
    vector<bool> used(m, false);

    for (int i = 0; i < m; i++) {
        if (used[i]) continue;
        int best_match = -1;
        int min_cost = INF_C;

        for (int j = i + 1; j < m; j++) {
            if (!used[j] && edgeWeights[odd_vertices[i]][odd_vertices[j]] < min_cost) {
                min_cost = edgeWeights[odd_vertices[i]][odd_vertices[j]];
                best_match = j;
            }
        }

        if (best_match != -1) {
            matching.emplace_back(odd_vertices[i], odd_vertices[best_match]);
            used[i] = used[best_match] = true;
        }
    }

    return matching;
}

// Função para encontrar um circuito Euleriano
vector<int> eulerian_circuit(int start, vector<multiset<int>>& graph) {
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

// Algoritmo de Christofides
int christofides(vector<vector<int>>& edgeWeights) {
    int n = edgeWeights.size();

    // 1. Construção da Árvore Geradora Mínima (MST)
    vector<pair<int, int>> mst_edges = minimum_spanning_tree_prim(edgeWeights);
    if (mst_edges.empty()) return -1;

    // Constrói o grafo da MST
    vector<multiset<int>> mst_graph(n);
    for (auto [u, v] : mst_edges) {
        mst_graph[u].insert(v);
        mst_graph[v].insert(u);
    }

    // 2. Encontrar vértices de grau ímpar
    vector<int> odd_vertices = find_odd_degree_vertices(mst_edges, n);

    // 3. Emparelhamento mínimo perfeito
    vector<pair<int, int>> matching = minimum_perfect_matching(odd_vertices, edgeWeights);

    // 4. Formar o multigrafo (combinação da MST com o matching)
    for (auto [u, v] : matching) {
        mst_graph[u].insert(v);
        mst_graph[v].insert(u);
    }

    // 5. Encontrar o circuito Euleriano
    vector<int> euler_circuit_path = eulerian_circuit(0, mst_graph);

    // 6. Gerar o ciclo Hamiltoniano (removendo duplicatas)
    vector<bool> visited_hamiltonian(n, false);
    vector<int> hamiltonian_path;
    int total_cost = 0;

    for (int node : euler_circuit_path) {
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

    return total_cost;
}