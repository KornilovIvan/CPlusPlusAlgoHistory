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

// Эйлеров путь (Eulerian Path)
//     Проходит через все рёбра графа ровно по одному разу
//     Может быть не замкнутым
//  1.2 Эйлеров цикл (Eulerian Circuit)
//     Эйлеров путь, который начинается и заканчивается в одной вершине
//     Замкнутый обход всех рёбер
// 2. Условия существования

//  2.1 Для неориентированного графа
//  Эйлеров цикл:

//     Все вершины имеют чётные степени

//     Граф связный (кроме изолированных вершин)

//  Эйлеров путь:

//     Ровно 2 вершины имеют нечётные степени (начало и конец)

//     Остальные вершины имеют чётные степени

//     Граф связный (кроме изолированных вершин)

// 2.2 Для ориентированного графа
//  Эйлеров цикл:

//     Для всех вершин: in_degree = out_degree

//     Граф слабо связный

// Эйлеров путь:

//     Одна вершина: out_degree = in_degree + 1 (начало)

//     Одна вершина: in_degree = out_degree + 1 (конец)

//     Остальные: in_degree = out_degree

//     Граф слабо связный

vector< int > fleury_algorithm(vector< vector< int > >& graph, vector< int >& degrees)
{
	int n = graph.size();

	// Находим стартовую вершину
	int start = 0;
	for (int i = 0; i < n; i++)
	{
		if (degrees[i] % 2 == 1)
		{
			start = i;
			break;
		}
	}

	stack< int > st;
	vector< int > path;
	st.push(start);

	while (!st.empty())
	{
		int v = st.top();

		if (degrees[v] == 0)
		{
			// Нет исходящих рёбер - добавляем в путь
			path.push_back(v);
			st.pop();
		}
		else
		{
			// Находим следующее ребро
			int u = -1;
			for (int i = 0; i < n; i++)
			{
				if (graph[v][i] > 0)
				{
					// Проверяем, не является ли ребро мостом
					if (count_edges(v, graph) > 1 || is_bridge(v, i, graph))
					{
						u = i;
						break;
					}
				}
			}

			if (u != -1)
			{
				// Удаляем ребро
				graph[v][u]--;
				graph[u][v]--;
				degrees[v]--;
				degrees[u]--;
				st.push(u);
			}
		}
	}

	return path;
}
