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
    
int main() {
    int n, m;
    cin >> n >> m;
    vector <vector<pair<int, int>>> graph(n, vector <pair<int, int>> ());
    while (m--) {
        int x, y, l;
        cin >> x >> y >> l;
        graph[x - 1].pb({y - 1, l});
        graph[y - 1].pb({x - 1, l});
    }
    vector <int> used(n), minEdge(n, INT_MAX), endEdge(n, -1);
    int res = 0;
    vector <pair<int, int>> result;
    minEdge[0] = 0;
    for (int i = 0; i < n; i++) {
        int v = -1;
        for (int j = 0; j < n; j++) {
            if (!used[j] && (v == -1 || minEdge[j] < minEdge[v])) {
                v = j;
            }
        }
        used[v] = 1;
        if (endEdge[v] != -1) {
            res += minEdge[v];
            result.pb({endEdge[v] + 1, v + 1});
        }
        for (auto to : graph[v]) {
            if (to.sc < minEdge[to.fr]) {
                minEdge[to.fr] = to.sc;
                endEdge[to.fr] = v;
            }
        }
    }
    cout << res << '\n';
    for (auto el : result) {
        cout << el.fr << ' ' << el.sc << '\n';
    }
    return 0;
}       