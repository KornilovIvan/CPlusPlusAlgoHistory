#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;
typedef long long ll;

// Поиск цикла в ориентированном графе

vector <int> p(vector<int> from, int to){   // восстановление цикла
    vector <int> cycle;
    for (int v = from[to]; v != to; v = from[v]){
        cycle.push_back(v + 1);
    }
    cycle.push_back(to + 1);
    reverse(cycle.begin(), cycle.end());
    return cycle;
}   

void dfs(vector<vector<int>> &graph, int v, vector <int> &visited, vector <int> &from, vector<int> &cycle){
    visited[v] = 1;
    for (auto to : graph[v]){
        if (visited[to] == 0){
            from[to] = v;       // запоминаем предка
            dfs(graph, to, visited, from, cycle);
            if (!cycle.empty()){  // когда нашли цикл прекращаем работу dfs
                return;
            }
        }else if (visited[to] == 1){
            from[to] = v;        // запоминаем предка
            cycle = p(from, to);
            return;
        }
    }
    visited[v] = 2;
}

int main(){
    int n, m;
    cin >> n >> m;
    vector <vector<int>> graph(n);
    vector <int> visited(n);
    vector <int> cycle;
    vector <int> from(n, -1);
    for (int i = 0; i < m; i++){
        int x, y;
        cin >> x >> y;
        graph[x - 1].push_back(y - 1);
    }
    for (int i = 0; i < n; i++){
        if (visited[i] == 0){
            dfs(graph, i, visited, from, cycle);
            if (!cycle.empty()){
                break;
            }
        }
    }
    if (cycle.empty()){
        cout << "NO";
    }else{
        cout << "YES" << '\n';
        cout << cycle.size() << '\n';
        for (auto el : cycle){
            cout << el << " ";
        }
    }
    return 0;
}