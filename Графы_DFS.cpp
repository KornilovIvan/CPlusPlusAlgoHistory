#include <iostream>
#include <vector>
using namespace std;

// DFS - Depth First Search (поиск в глубину)

void dfs(vector<vector<int>> &graph, int v, vector <int> &visited){   // для графа который задан списком смежности
    visited[v] = 1;    // помечаем, что были в этой вершине

    for (int to : graph[v]){  // проверяем соседей вершины 
        if (!visited[v]){   // если не были в соседних 
            dfs(graph, to, visited); 
        }
    }
}


void dfs(vector<vector<int>> &graph, int v, vector <int> &visited){   // для графа который задан матрицей смежности
    visited[v] = 1;   

    for (int to = 0; to < graph.size(); to ++){  
        if (!visited[v] and graph[v][to]){   
            dfs(graph, to, visited); 
        }
    }
}



int main(){
    vector <vector<int>> graph; // исходный граф
    vector <int> visited;  // список вершин в которые мы не ходили
    int v;   // начальная вершина
    dfs(graph, v, visited);   
    return 0;
}