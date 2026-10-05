#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;
typedef long long ll;

// Проверка графа на Двудольность

void dfs(vector<vector<int>> graph, int v, vector <int> &visited, vector <int> &from, int color, bool &res){
    visited[v] = color;    // красим предка и листы в разные цвета
    for (auto to : graph[v]){
        if (to == from[v]){
            continue;
        }
        if (visited[to] == 0){
            from[to] = v;
            dfs(graph, to, visited, from, 3 - color, res);     // color принимает значения 1 или 2
            if (res == true){
                return;
            }
        }else if (visited[to] == color){     // eсли цвет предка и листа совпадают, то граф не двудольный
            res = true;
            return;
        }
    }
}

int main(){
    int n, m;
    cin >> n >> m;
    vector <vector<int>> graph(n);
    vector <int> visited(n);
    vector <int> from(n, -1);
    for (int i = 0; i < m; i++){
        int x, y;
        cin >> x >> y;
        graph[x - 1].push_back(y - 1);
        graph[y - 1].push_back(x - 1);
    }
    bool res = false;
    for (int i = 0; i < n; i++){
        if (visited[i] == 0){
            dfs(graph, i, visited, from, 1, res);
            if (res == true){
                break;
            }
        }
    }
    if (res == true){
        cout << "YES";
    }else{
        cout << "NO";
    }
    return 0;
}