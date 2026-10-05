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
vector <pair<int, int>> most;

void DFS(vector <vector<int>> &graph, int v, int last, vector <int> &visited, vector <int> &enter, vector <int> &ret) {
    visited[v] = 1;
    enter[v] = Time;
    ret[v] = Time;
    Time++;
    for (auto el : graph[v]) {
        if (el == last) continue;
        if (visited[el] == 1) {   // если (el, v) - обратное ребро
            ret[v] = min(ret[v], enter[el]);
        } else {
            DFS(graph, el, v, visited, enter, ret);
            ret[v] = min(ret[v], ret[el]);
            if (ret[el] > enter[v]) {   // если ребро (el, v) - мост
                most.pb({el, v});
            }
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    vector <vector<int>> graph(n, vector <int> ());
    map <pair<int, int>, int> mapa;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u - 1].pb(v - 1);
        graph[v - 1].pb(u - 1);
        mapa[{u - 1, v - 1}] = i;
        mapa[{v - 1, u - 1}] = i;
    }
    vector <int> visited(n, 0);
    vector <int> ret(n, 0);
    vector <int> enter(n, 0);
    for (int i = 0; i < n; i++) {
        if (visited[i] == 0) {
            DFS(graph, i, i, visited, enter, ret);
        }
    }
    cout << most.size() << '\n';
    for (auto el : most) {
        cout << mapa[el] + 1 << " ";
    }
    return 0;
}       