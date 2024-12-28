#include <bits/stdc++.h>
#include <math.h>

using namespace std;

#define _ ios_base::sync_with_stdio(0); cin.tie(0);
#define PI 3.141592
#define RRR 6378.388
#define ff first
#define ss second
#define pb push_back
     
typedef long long ll;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

int euclidean_distance(float x1, float x2, float y1, float y2){
    float x = x1 - x2;
    float y = y1 - y2;
    return round(sqrt(x*x + y*y));
}

int pseudo_euclidian_distance(float x1, float x2, float y1, float y2){
    float x = x1 - x2;
    float y = y1 - y2;
    float r = sqrt((x*x + y*y)/ 10);
    int t = round(r);
    if(t < r) return t+1;
    return t;
}

void print_graph(vector<vector<int>> graph){
    int i = 0;
    for(int i = 0; i < graph.size(); i++){
        cout << i << " ";
        for(int j = 0; j < graph[i].size(); j++){
            cout << graph[i][j] << " ";
        }
        cout << endl; 
    }
    
}

void print_edgeWeights(vector<vector<int>> edgeWeights){
    int i = 0;
    for(int i = 0; i < edgeWeights.size(); i++){
        cout << i << " ";
        for(int j = 0; j < edgeWeights[i].size(); j++){
            if(edgeWeights[i][j] != INF) cout << edgeWeights[i][j] << " ";
        }
        cout << endl; 
    }
}

void print_positions(vector<pair<float, float>> positions){
    int i = 0;
    for(auto p: positions){
        cout << i << " " << p.ff << " " << p.ss << endl; 
        i++;
    }
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    string filename = argv[1];
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }

    string line, keyword, value; // Variáveis para ler as instâncias
    int n = 0; // Dimensão
    bool complete = true; // O grafo é completo por default
    string weightType, edgeFormat;
    vector<pair<float, float>> positions; // Posição de cada ponto no plano
    vector<vector<int>> graph; // Conexões de cada ponto
    vector<vector<int>> edgeWeights; // Peso de cada aresta, é indexado da mesma forma que no grafo

    while (getline(file, line)){

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
        } else if(keyword == "EDGE_DATA_FORMAT") iss >> edgeFormat;
        else if(keyword == "NODE_COORD_SECTION"){
            while (getline(file, line)){
                if (line == "EOF") break;
                istringstream temp(line);
                int p, x, y; temp >> p >> x >> y;
                positions[p-1].ff = x; positions[p-1].ss = y;
            }
        } else if(keyword == "EDGE_DATA_SECTION"){
            complete = false;
            if(edgeFormat == "EDGE_LIST"){
                while (getline(file, line)){
                    if (line == "EOF") break;
                    istringstream temp(line);
                    string x, y;
                    while(temp >> x >> y){
                        graph[stoi(x)-1].pb(stoi(y)-1);
                        graph[stoi(y)-1].pb(stoi(x)-1);
                    }
                }
            } else if(edgeFormat == "ADJ_LIST"){
                while (getline(file, line)){
                    if (line == "EOF") break;
                    istringstream temp(line);
                    string x, y;
                    temp >> x;
                    while(temp >> y){
                        if(y == "-1") break;
                        graph[stoi(x)-1].pb(stoi(y)-1);
                    }
                }
            }
        } else if (keyword == "NAME" || keyword == "COMMENT" || keyword == "DISPLAY_DATA_TYPE") continue;
        else {
            cerr << "Error: Unsupported input type: " << keyword << " " << value << endl;
            return 1;
        }
    }

    if(complete){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(i!=j)graph[i].push_back(j);
            }
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
    
    print_edgeWeights(edgeWeights);
    print_graph(graph);
    print_positions(positions);

    file.close();
    return 0;
}