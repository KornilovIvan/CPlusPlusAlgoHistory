#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <math.h>
#include <string>
#include <set>
#include <iomanip>
#include <queue>
#include <bitset>
using namespace std;
typedef unsigned long long ll;
#define pb push_back
#define fr first
#define sc second
#define all(A) A.begin(), A.end()
const int INF = 1e9;

int main(){
    int n, m;
    cin >> n >> m;
    vector<vector<int>> dist(n, vector <int> (n, 30000));
    for (int i = 0; i < n; i++){
        dist[i][i] = 0;
    }
    for (int i = 0; i < m; i++){
        int x, y, weight;
        cin >> x >> y >> weight;
        dist[x - 1][y - 1] = min(weight, dist[x - 1][y - 1]);
    }
    for (int v = 0; v < n; v++){
        for (int x = 0; x < n; x++){
            for (int y = 0; y < n; y++){
                if (dist[x][v] != 30000 and dist[v][y] != 30000 and dist[x][y] > dist[x][v] + dist[v][y]){
                    dist[x][y] = dist[x][v] + dist[v][y];
                }
            }
        }
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            cout << dist[i][j] << " ";
        }
        cout << '\n';
    }
    return 0;
}