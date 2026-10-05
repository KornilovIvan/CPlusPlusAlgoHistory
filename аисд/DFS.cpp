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

// Depth-First Search (поиск в глубину) — алгоритм обхода графа, который идёт "вглубь" пока это возможно, затем
// возвращается. Основные свойства DFS
//     Рекурсивный по природе

//     Использует стек (явно или неявно через рекурсию)

//     Временная сложность: O(V + E)

//     Память: O(V) для хранения посещённых вершин

vector< vector< int > > graph;
vector< bool > visited;

//  Рекурсивный DFS
void dfs(int v)
{
	visited[v] = true;
	// Обработка вершины v
	cout << v << " ";

	for (int u : graph[v])
	{
		if (!visited[u])
		{
			dfs(u);
		}
	}
}

// Итеративный DFS (лучше для больших графов)
void dfs_iterative(int start)
{
	vector< bool > visited(graph.size(), false);
	stack< int > st;
	st.push(start);
	visited[start] = true;

	while (!st.empty())
	{
		int v = st.top();
		st.pop();

		// Обработка вершины v
		cout << v << " ";

		for (int u : graph[v])
		{
			if (!visited[u])
			{
				visited[u] = true;
				st.push(u);
			}
		}
	}
}

// Время входа/выхода в DFS
vector< int > tin, tout;
vector< bool > visited;
int timer = 0;

void dfs(int v)
{
	visited[v] = true;
	tin[v] = timer++;	 // время входа

	for (int u : graph[v])
	{
		if (!visited[u])
		{
			dfs(u);
		}
	}

	tout[v] = timer++;	  // время выхода
}

// Проверка: u - предок v?
bool is_ancestor(int u, int v)
{
	return tin[u] <= tin[v] && tout[v] <= tout[u];
}

//  Поиск циклов в графе

// В неориентированном графе
bool hasCycleUndirected(int v, int parent)
{
	visited[v] = true;

	for (int u : graph[v])
	{
		if (u == parent)
			continue;	 // пропускаем ребро к родителю

		if (visited[u])
		{
			return true;	// нашли цикл
		}

		if (hasCycleUndirected(u, v))
		{
			return true;
		}
	}
	return false;
}

//  В ориентированном графе
vector< int > color;	// 0 - не посещён, 1 - в обработке, 2 - обработан

bool hasCycleDirected(int v)
{
	color[v] = 1;	 // начинаем обработку

	for (int u : graph[v])
	{
		if (color[u] == 0)
		{
			if (hasCycleDirected(u))
				return true;
		}
		else if (color[u] == 1)
		{
			return true;	// нашли обратное ребро - цикл
		}
	}

	color[v] = 2;	 // завершили обработку
	return false;
}

// Топологическая сортировка
vector< int > order;

void topologicalSort(int v)
{
	visited[v] = true;

	for (int u : graph[v])
	{
		if (!visited[u])
		{
			topologicalSort(u);
		}
	}

	order.push_back(v);	   // добавляем в обратном порядке
}

// После обхода всех вершин:
reverse(order.begin(), order.end());

// Проверка возможности топологической сортировки
bool canTopologicalSort()
{
	// Если есть цикл - топологической сортировки не существует
	for (int i = 0; i < n; i++)
	{
		if (color[i] == 0 && hasCycleDirected(i))
		{
			return false;
		}
	}
	return true;
}

// дальше у меня есть файлы с дфс
