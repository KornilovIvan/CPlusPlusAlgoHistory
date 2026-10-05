#include <iostream>
#include <queue>
#include <vector>
using namespace std;
typedef long long ll;

struct st
{
	int x, y, weight;
};

vector< int > FordBellman(vector< st > graph, int start, int n)
{
	vector< int > dist(n, INT_MAX);
	dist[start] = 0;

	for (int i = 0; i < n - 1; i++)
	{	 // n - 1 итерация
		for (auto &[x, y, weight] : graph)
		{
			if (dist[x] != INT_MAX and dist[y] > dist[x] + weight)
			{
				dist[y] = dist[x] + weight;
			}
		}
	}
	return dist;
}

vector< int > FordBellmanSPFA(vector< vector< pair< int, int > > > graph, int start)
{
	vector< int > dist(graph.size(), INT_MAX);
	dist[start] = 0;
	queue< int > q;
	q.push(start);
	vector< int > inQueue(graph.size(), false);
	inQueue[start] = true;
	while (!q.empty())
	{
		int v = q.front();
		q.pop();
		inQueue[v] = false;
		for (auto &[to, weight] : graph[v])
		{
			if (dist[to] > dist[v] + weight)
			{
				dist[to] = dist[v] + weight;
				if (!inQueue[to])
				{
					q.push(to);
					inQueue[to] = true;
				}
			}
		}
	}
	return dist;
}

int main()
{
	int n, m;
	cin >> n >> m;
	vector< st > graph2(m);
	for (auto &[x, y, weight] : graph2)
	{
		cin >> x >> y >> weight;
	}
	vector< vector< pair< int, int > > > graph(n);
	for (int i = 0; i < m; i++)
	{
		int x, y, weight;
		cin >> x >> y >> weight;
		graph[x - 1].push_back({ y - 1, weight });
	}
	auto dist = FordBellmanSPFA(graph, 0);
	for (auto el : dist)
	{
		cout << el << " ";
	}
	return 0;
}
