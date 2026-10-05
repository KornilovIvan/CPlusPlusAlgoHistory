#include <algorithm>
#include <bitset>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <math.h>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>
using namespace std;
typedef long long ll;
#define pb push_back
#define fr first
#define sc second
#define all(A) A.begin(), A.end()
#define IOS                                                                                                            \
	ios_base ::sync_with_stdio(false);                                                                                 \
	cin.tie(0);                                                                                                        \
	cout.tie(0)

// Breadth-First Search (поиск в ширину) — алгоритм обхода графа по уровням, который обрабатывает все вершины на текущей
// глубине перед переходом на следующую.
// Основные свойства BFS

//     Итеративный по природе

//     Использует очередь

//     Находит кратчайшие пути в невзвешенном графе

//     Временная сложность: O(V + E)

//     Память: O(V)

pair< vector< int >, vector< int > > bfs(vector< vector< int > > graph, int start)
{
	vector< int > dist(graph.size(), INT_MAX);	  // длина пути
	vector< int > from(graph.size(), -1);
	queue< int > q;	   // очередь в котрой будем хранить следующие вершины

	dist[start] = 0;
	q.push(start);

	while (!q.empty())
	{
		int v = q.front();
		q.pop();

		for (auto to : graph[v])
		{
			if (dist[to] > dist[v] + 1)
			{
				dist[to] = dist[v] + 1;
				from[to] = v;
				q.push(to);
			}
		}
	}
	return { dist, from };
}

vector< int > getPath(vector< int > from, int finish)
{	 // востановление пути от finish до start
	vector< int > path;
	for (int v = finish; v != -1; v = from[v])
	{
		path.push_back(v);
	}
	reverse(path.begin(), path.end());
	return path;
}

int main()
{
	vector< vector< int > > graph;
	int start;
	auto [dist, from] = bfs(graph, start);
	return 0;
}
