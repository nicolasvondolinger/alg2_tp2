#include <bits/stdc++.h>
#include "twiceAroundTheTree.cpp"
#include "branchAndBound.cpp"
#include "christofides.cpp"

using namespace std;
using namespace std::chrono;

#define _ ios_base::sync_with_stdio(0); cin.tie(0);
#define ff first
#define ss second
#define pb push_back

typedef long long ll;

int euclidean_distance(float x1, float x2, float y1, float y2) {
    float x = x1 - x2;
    float y = y1 - y2;
    return round(sqrt(x*x + y*y));
}

void print_graph(vector<vector<int>> graph) {
    int i = 0;
    for(int i = 0; i < graph.size(); i++) {
        cout << i << " ";
        for(int j = 0; j < graph[i].size(); j++) {
            cout << graph[i][j] << " ";
        }
        cout << endl; 
    }
}

void print_edgeWeights(vector<vector<int>> edgeWeights) {
    int i = 0;
    for(int i = 0; i < edgeWeights.size(); i++) {
        cout << i << " ";
        for(int j = 0; j < edgeWeights[i].size(); j++) {
             cout << edgeWeights[i][j] << " ";
        }
        cout << endl; 
    }
}

void print_positions(vector<pair<float, float>> positions) {
    int i = 0;
    for(auto p: positions) {
        cout << i << " " << p.ff << " " << p.ss << endl; 
        i++;
    }
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <filename> <flag>" << endl;
        cerr << "Flags: -b (Branch and Bound)" << endl;
        cerr << "       -t (Twice Around the Tree)" << endl;
        cerr << "       -c (Christofides)" << endl;
        return 1;
    }

    string filename = argv[1];
    string flag = argv[2];

    // Verifica se a flag é válida
    if (flag != "-b" && flag != "-t" && flag != "-c") {
        cerr << "Error: Invalid flag '" << flag << "'" << endl;
        cerr << "Valid flags are:" << endl;
        cerr << "  -b (Branch and Bound)" << endl;
        cerr << "  -t (Twice Around the Tree)" << endl;
        cerr << "  -c (Christofides)" << endl;
        return 1;
    }

    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }

    string line, keyword, value; int n = 0;
    string weightType;
    vector<pair<float, float>> positions;
    vector<vector<int>> graph;
    vector<vector<int>> edgeWeights;

    print_edgeWeights(edgeWeights);
    graph.resize(n); positions.resize(n);

    // Função de leitura do arquivo
    while (getline(file, line)) {
        istringstream iss(line); iss >> keyword;
        string discard; iss >> discard;
        
        if (keyword == "TYPE") {
            iss >> value;
            if (value != "TSP") {
                cerr << "Error: Unsupported problem type: " << value << endl;
                return 1;
            }
        } else if(keyword == "EDGE_WEIGHT_TYPE"){
            iss >> weightType;

            if(weightType != "EUC_2D"){
                cerr << "Error: Unsupported edge weight type " << weightType << endl;
                return 1;
            }
        } else if(keyword == "DIMENSION"){
            iss >> n; graph.resize(n); positions.resize(n); edgeWeights.resize(n, vector<int>(n, INF));
        } else if(keyword == "NODE_COORD_SECTION"){
            while (getline(file, line)){
                if (line == "EOF") break;
                istringstream temp(line);
                int p, x, y; temp >> p >> x >> y;
                positions[p-1].ff = x; positions[p-1].ss = y;
            }
        } else if (keyword == "NAME" || keyword == "COMMENT" || keyword == "DISPLAY_DATA_TYPE") continue;
        else {
            cerr << "Error: Unsupported input type: " << keyword << " " << value << endl;
            return 1;
        }
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(i != j) graph[i].push_back(j);
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < graph[i].size(); j++) {  
            int w = graph[i][j];
            float x1 = positions[i].first;
            float y1 = positions[i].second;
            float x2 = positions[w].first;
            float y2 = positions[w].second;

            edgeWeights[i][w] = euclidean_distance(x1, x2, y1, y2);
        }
    } 
    
    /*
    cout << "Edge Weights:" << endl;
    print_edgeWeights(edgeWeights); // Peso de cada aresta
    cout << "Graph:" << endl;
    print_graph(graph); // Conexões de cada ponto
    cout << "Positions:" << endl;
    print_positions(positions); // Posição de cada ponto no plano
    */

    vector<int> path;
    vector<bool> visited(n, false);
    int best_cost = INF;
    visited[0] = true;
    path.push_back(0);
    
    if(flag == "-b"){
        auto start = high_resolution_clock::now();

        branch_and_bound(path, visited, 0, best_cost, edgeWeights);

        cout << "Custo total: " << best_cost << endl;
        
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop - start);
        double seconds = duration.count() / 1e6; 

        cout << "Tempo de execução: " << seconds << " segundos" << endl;
    } else if (flag == "-t"){
        auto start = high_resolution_clock::now();

        auto result = twiceAroundTheTree(graph, edgeWeights);

        cout << "Custo total: " << result.second << endl;

        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop - start);
        double seconds = duration.count() / 1e6; 

        cout << "Tempo de execução: " << seconds << " segundos" << endl;
    } else {

    }
    
    file.close();
    
    return 0;
}