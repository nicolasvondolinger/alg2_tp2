#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

#define _ ios_base::sync_with_stdio(0); cin.tie(0);
#define ff first
#define ss second
#define pb push_back

typedef long long ll;

pair<vector<int>, vector<vector<int>>> primMST(const vector<vector<int>> &edgeWeights)
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

    // recuperando as arestas/ conexoes dos vertices
    vector<vector<int>> adjList(n);
    for (int i = 1; i < n; ++i)
    {
        if (parent[i] != -1)
        {
            adjList[parent[i]].push_back(i);
            adjList[i].push_back(parent[i]);
        }
    }

    // imprime as arestas da árvore
    /*cout << "Arestas da árvore minima:" << endl;
    for (int i = 1; i < n; ++i)
    {
        if (parent[i] != -1)
        {
            cout << "Aresta: (" << parent[i] << ", " << i << "), Custo: " << edgeWeights[parent[i]][i] << endl;
        }
    }*/

    return {parent, adjList};
}