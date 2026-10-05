#include <iostream>
#include <vector>
#include <set>
using namespace std;
typedef long long ll;

vector <int> dijkstra(vector<vector<pair<int, int>>> graph, int start){
    vector <int> dist(graph.size(), INT_MAX);
    dist[start] = 0;

    set <pair<int, int>> q;
    q.insert({dist[start], start});

    while (!q.empty()){
        int nearest = q.begin()->second;
        q.erase({q.begin()});

        for (auto &[to, weight] : graph[nearest]){
            if (dist[to] > dist[nearest] + weight){
                q.erase({dist[to], to});
                dist[to] = dist[nearest] + weight;
                q.insert({dist[to], to});
            }
        }
    }
}

int main(){
    vector <vector<pair<int, int>>> graph;
    for (int i = 0; i < 100; i++){
        int x, y, weight;
        cin >> x >> y >> weight;
        graph[x].push_back({y, weight});
        graph[y].push_back({x, weight});
    }
    return 0;
}