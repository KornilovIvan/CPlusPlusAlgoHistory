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
typedef long long ll;
#define pb push_back
#define fr first
#define sc second
#define all(A) A.begin(), A.end()
#define IOS ios_base :: sync_with_stdio(false); cin.tie(0); cout.tie(0)
const int INF = int(1e9);

vector <int> ord;
void DFS(vector <vector<int>> &graph, int v, vector <int> &visited) {
    visited[v] = 1;
    for (auto el : graph[v]) {
        if (!visited[el]) {
            DFS(graph, el, visited);
        }
    }
    ord.pb(v);
}

void DFS1(vector <vector<int>> &graph, int v, vector <int> &visited, vector <int> &component, int col) {
    visited[v] = 1;
    component[v] = col;
    for (auto el : graph[v]) {
        if (!visited[el]) {
            DFS1(graph, el, visited, component, col);
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    vector <vector<int>> graph(n, vector <int> ());
    vector <vector<int>> graph1(n, vector <int> ());
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u - 1].pb(v - 1);
        graph1[v - 1].pb(u - 1);
    }
    vector <int> visited(n, 0);
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            DFS(graph, i, visited);
        }
    }
    vector <int> visited1(n, 0);
    vector <int> component(n, 0);
    reverse(all(ord));
    int col = 0;
    for (auto el : ord) {
        if (!visited1[el]) {
            col ++;
            DFS1(graph1, el, visited1, component, col);
        }
    }
    cout << col << '\n';
    for (auto el : component) {
        cout << el << ' ';
    }
    return 0;
}       