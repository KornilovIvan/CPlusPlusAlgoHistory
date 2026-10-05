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

int Time = 1;
set <int> res;

void DFS(vector<vector<int>> &graph, int v, int p, vector <int> &visited, vector <int> &tin, vector <int> &fup) {
    visited[v] = 1;
    tin[v] = Time;
    fup[v] = Time;
    Time ++;
    int c = 0;
    for (auto el : graph[v]) {
        if (el == p) continue;
        if (visited[el] == 1) {
            fup[v] = min(fup[v], tin[el]);
        } else {
            DFS(graph, el, v, visited, tin, fup);
            fup[v] = min(fup[v], fup[el]);
            if (fup[el] >= tin[v] && p != -1) {
                res.insert(v);
            }
            c ++;
        }
    }
    if (p == -1 && c > 1) {
        res.insert(v);
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    vector <vector<int>> graph(n, vector <int> ());
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u - 1].pb(v - 1);
        graph[v - 1].pb(u - 1);
    }
    vector <int> visited(n, 0);
    vector <int> tin(n, 0);
    vector <int> fup(n, 0);
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            DFS(graph, i, -1, visited, tin, fup);
        }
    }
    cout << res.size() << '\n';
    for (auto el : res) {
        cout << el + 1 << " ";
    }
    return 0;
}       