#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

// BFS - поиск в ширину

pair <vector<int>, vector <int>>  bfs(vector <vector<int>> graph, int start){
    vector <int> dist(graph.size(), INT_MAX);   // длина пути
    vector <int> from(graph.size(), -1);
    queue <int> q;   // очередь в котрой будем хранить следующие вершины

    dist[start] = 0;
    q.push(start);

    while (!q.empty()){
        int v = q.front();
        q.pop();

        for (auto to : graph[v]){
            if (dist[to] > dist[v] + 1){
                dist[to] = dist[v] + 1;
                from[to] = v;
                q.push(to);
            }
        }
    }
    return {dist, from};
}


vector <int> getPath(vector<int> from, int finish){   // востановление пути от finish до start
    vector <int> path;
    for (int v = finish; v != -1; v = from[v]){
        path.push_back(v);
    }
    reverse(path.begin(), path.end());
    return path;
}



int main(){
    vector <vector<int>> graph;
    int start;
    auto [dist, from] = bfs(graph, start);
    return 0;
}
