#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

#define _ ios_base::sync_with_stdio(0); cin.tie(0);
#define ff first
#define ss second
#define pb push_back

typedef long long ll;

// Função para calcular o limite inferior usando MST (Arvore Geradora Mínima)
int minimum_spanning_tree_cost_c(vector<vector<int>>& edgeWeights, vector<bool>& visited) {
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

// Função para encontrar os vértices de grau ímpar na MST
vector<int> find_odd_degree_vertices(vector<vector<int>>& mst) {
    int n = mst.size();
    vector<int> degree(n, 0);
    vector<int> odd_vertices;

    // Calcula o grau de cada vértice
    for (int i = 0; i < n; i++) {
        for (int j : mst[i]) {
            degree[i]++;
        }
    }

    // Encontra os vértices de grau ímpar
    for (int i = 0; i < n; i++) {
        if (degree[i] % 2 == 1) {
            odd_vertices.push_back(i);
        }
    }

    return odd_vertices;
}

// Função para calcular o emparelhamento mínimo perfeito
vector<pair<int, int>> minimum_perfect_matching(const vector<int>& odd_vertices, vector<vector<int>>& edgeWeights) {
    vector<pair<int, int>> matching;
    int m = odd_vertices.size();
    vector<bool> used(m, false);

    // Emparelha vértices de forma gananciosa para minimizar o custo
    for (int i = 0; i < m; i++) {
        if (used[i]) continue;
        int best_match = -1;
        int min_cost = INF;

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
vector<int> eulerian_circuit(int start, vector<vector<int>>& graph) {
    vector<int> circuit;
    stack<int> s;
    vector<vector<int>> temp_graph = graph;

    s.push(start);
    while (!s.empty()) {
        int u = s.top();

        if (!temp_graph[u].empty()) {
            int v = temp_graph[u].back();
            temp_graph[u].pop_back();
            s.push(v);
        } else {
            circuit.push_back(u);
            s.pop();
        }
    }

    reverse(circuit.begin(), circuit.end());
    return circuit;
}

// Função principal do algoritmo de Christofides
int christofides(vector<vector<int>>& edgeWeights) {
    int n = edgeWeights.size();

    // 1. Construção da Árvore Geradora Mínima (MST)
    vector<bool> visited(n, false);
    vector<vector<int>> mst(n);
    int mst_cost = minimum_spanning_tree_cost_c(edgeWeights, visited);

    // Constrói o grafo da MST
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (visited[i] && visited[j] && edgeWeights[i][j] < INF) {
                mst[i].push_back(j);
                mst[j].push_back(i);
            }
        }
    }

    // 2. Encontrar vértices de grau ímpar
    vector<int> odd_vertices = find_odd_degree_vertices(mst);

    // 3. Emparelhamento mínimo perfeito
    vector<pair<int, int>> matching = minimum_perfect_matching(odd_vertices, edgeWeights);

    // 4. Formar o multigrafo (combinação da MST com o matching)
    for (auto& match : matching) {
        mst[match.first].push_back(match.second);
        mst[match.second].push_back(match.first);
    }

    // 5. Encontrar o circuito Euleriano
    vector<int> euler_circuit = eulerian_circuit(0, mst);

    // 6. Gerar o ciclo Hamiltoniano (removendo duplicatas)
    vector<bool> visited_hamiltonian(n, false);
    vector<int> hamiltonian_path;
    int total_cost = 0;

    for (int node : euler_circuit) {
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