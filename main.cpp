#include <bits/stdc++.h>
#include <sstream>
#include <string>

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

int manhattan_distance(float x1, float x2, float y1, float y2){
    float x = abs(x1 - x2);
    float y = abs(y1 - y2);
    return round(x + y);
}

int maximum_distance(float x1, float x2, float y1, float y2){
    float x = abs(x1 - x2);
    float y = abs(y1 - y2);
    return max(round(x), round(y));
}

int geographical_distance(float x1, float x2, float y1, float y2) {
    int deg1 = round(x1); 
    float min1 = x1 - deg1; 
    float latitude1 = PI * (deg1 + 5.0 * min1 / 3.0) / 180.0; 

    deg1 = round(y1); 
    min1 = y1 - deg1; 
    float longitude1 = PI * (deg1 + 5.0 * min1 / 3.0) / 180.0; 

    
    int deg2 = round(x2); 
    float min2 = x2 - deg2; 
    float latitude2 = PI * (deg2 + 5.0 * min2 / 3.0) / 180.0; 

    deg2 = round(y2); 
    min2 = y2 - deg2; 
    float longitude2 = PI * (deg2 + 5.0 * min2 / 3.0) / 180.0; 

    float q1 = cos(longitude1 - longitude2);
    float q2 = cos(latitude1 - latitude2);
    float q3 = cos(latitude1 + latitude2);

    int dij = static_cast<int>(RRR * acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0);

    return dij;
}

int pseudo_euclidian_distance(float x1, float x2, float y1, float y2){
    float x = x1 - x2;
    float y = y1 - y2;
    float r = sqrt((x*x + y*y)/ 10);
    int t = round(r);
    if(t < r) return t+1;
    return t;
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

    string line, keyword, value;
    int n = 0; // Dimensão
    string weightType, weightFormat, edgeFormat, coordType = "NO_COORDS";
    vector<pair<float, float>> positions; // Posição de cada ponto no plano
    vector<vector<int>> graph; // Conexões de cada ponto
    vector<vector<float>> edgeWeights; // Peso de cada aresta, é indexado da mesma forma que no grafo

    while (getline(file, line)) {
        istringstream iss(line); iss >> keyword;

        if (keyword == "TYPE") {
            iss >> value;
            if (value != "TSP") {
                cerr << "Error: Unsupported problem type: " << value << endl;
                return 1;
            }
        } else if(keyword == "EDGE_WEIGHT_TYPE"){
            iss >> weightType;
            if(weightType == "EUC_3D" || weightType == "MAX_3D" || weightType == "MAN_3D"){
                cerr << "Error: Unsupported edge weight type: " << weightType << endl;
                return 1;
            }
        } else if(keyword == "EDGE_WEIGHT_FORMAT") iss >> weightFormat;
        else if(keyword == "DIMENSION"){
            iss >> n; graph.resize(n); positions.resize(n);
        }
        else if(keyword == "EDGE_DATA_FORMAT") iss >> edgeFormat;
        else if(keyword == "NODE_COORD_TYPE") iss >> coordType;
        else if(keyword == "NODE_COORD_SECTION"){
            while (getline(file, line)){
                istringstream temp(line);
                int p, x, y; temp >> p >> x >> y;
                positions[p-1].ff = x; positions[p-1].ss = y;
            }
        } else if(keyword == "EDGE_DATA_SECTION"){
            if(edgeFormat == "EDGE_LIST"){
                while (getline(file, line)){
                    istringstream temp(line);
                    string x, y;
                    while(temp >> x >> y){
                        graph[stoi(x)-1].pb(stoi(y)-1);
                        graph[stoi(y)-1].pb(stoi(x)-1);
                    }
                }
            } else if(edgeFormat == "ADJ_LIST"){
                while (getline(file, line)){
                    istringstream temp(line);
                    string x, y;
                    temp >> x;
                    while(temp >> y){
                        if(y == "-1") break;
                        graph[stoi(x)-1].pb(stoi(y)-1);
                    }
                }
            }
        } else if(keyword == "EDGE_WEIGHT_SECTION"){
            if(weightFormat == "FUNCTION"){
                
            }
        } else if (keyword == "NAME" || keyword == "COMMENT" || keyword == "DISPLAY_DATA_TYPE") continue;
        else {
            cerr << "Error: Unsupported input type: " << keyword << " " << value << endl;
            return 1;
        }
    }

    cout << "Dimension: " << n << endl;

    file.close();
    return 0;
}