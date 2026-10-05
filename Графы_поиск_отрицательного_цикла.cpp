#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
typedef long long ll;

struct st{
    int x, y, weight;
};

vector <int> getcycle(vector <int> from, int y){   // восстановление цикла (как в dfs)
    vector <int> cycle = {y + 1};
    for (int v = from[y]; v != y; v = from[v]){
        cycle.push_back(v + 1);
    }
    cycle.push_back(y + 1);
    reverse(cycle.begin(), cycle.end());
    return cycle;
}

vector <int> FordBellman(vector<st> graph, int n){
    vector <int> dist(n);
    dist[0] = 0;
    vector <int> from(n, -1);
    
    for (int i = 0; i < n - 1; i++){
        for (auto &[x, y, weight] : graph){
            if (dist[y] > dist[x] + weight){
                dist[y] = dist[x] + weight;
                from[y] = x;
            }
        }
    }

    for (auto &[x, y, weight] : graph){    // если можно про релаксировать какое-то ребро, то в графе есть цикл
        if (dist[y] > dist[x] + weight){
            from[y] = x;            // замыкаем цикл в массиве from

            for (int i = 0; i < n - 1; i++){   // возвращаемся от про релаксированного ребра в отрицательный цикл
                y = from[y];
            }

            return getcycle(from, y);
        }
    }
    return {};
}

int main(){
    int n;
    cin >> n;
    vector <st> graph;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            int weight;
            cin >> weight;
            if(weight != 100000){
                st x;
                x.weight = weight;
                x.x = i;
                x.y = j;
                graph.push_back(x);
            }
        }
    }
    auto cycle = FordBellman(graph, n);
    if (cycle.empty()){
        cout << "NO";
    }else{
        cout << "YES" << '\n' << cycle.size() << '\n';
        for (auto el : cycle){
            cout << el << " ";
        }
    }
    return 0;
}