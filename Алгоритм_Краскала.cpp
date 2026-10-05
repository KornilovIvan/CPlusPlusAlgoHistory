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

struct Edge {
    int from, to, weight;
};

vector <int> Size, parent;

void makeSet(int v) {
    parent[v] = v;
    Size[v] = 1;
}

int findSet(int v) {
    if (v == parent[v]) {
        return v;
    }
    return parent[v] = findSet(parent[v]);
}

void unionSets(int a, int b) {
    a = findSet(a);
    b = findSet(b);
    if (a != b) {
        if (Size[a] < Size[b]) {
            swap(a, b);
        }
        parent[b] = a;
        Size[a] += Size[b];
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    Size.resize(n);
    parent.resize(n);
    for (int i = 0; i < n; i++) makeSet(i);
    vector <Edge> edges;
    while (m--) {
        int x, y, l;
        cin >> x >> y >> l;
        Edge edge = {x - 1, y - 1, l};
        edges.pb(edge);
    }

    sort(all(edges), [](Edge a, Edge b) {
        if (a.weight == b.weight && a.from == b.from) {
            return a.to < b.to;
        } else if (a.weight == b.weight) {
            return a.from < b.from;
        } else {
            return a.weight < b.weight;
        }
    });

    int res = 0;
    vector <pair<int, int>> result;

    for (auto [a, b, weight] : edges) {
        if (findSet(a) != findSet(b)) {
            res += weight;
            result.pb({a, b});
            unionSets(a, b);
        }
    }
    
    cout << res << '\n';
    for (auto [a, b] : result) {
        cout << a + 1 << ' ' << b + 1 << '\n';
    }
    return 0;
}       