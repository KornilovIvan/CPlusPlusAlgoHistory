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

class DSU
{
  private:
	vector< int > parent, rank;

  public:
	DSU(int n)
	{
		parent.resize(n);
		rank.resize(n, 0);
		for (int i = 0; i < n; i++)
		{
			parent[i] = i;
		}
	}

	int find_set(int v)
	{
		if (v == parent[v])
			return v;
		return parent[v] = find_set(parent[v]);	   // путь сжатия
	}

	void union_sets(int a, int b)
	{
		a = find_set(a);
		b = find_set(b);
		if (a != b)
		{
			if (rank[a] < rank[b])
				swap(a, b);
			parent[b] = a;
			if (rank[a] == rank[b])
				rank[a]++;
		}
	}
};

// Минимальное остовное дерево (алгоритм Краскала)
int kruskal(int n, vector< tuple< int, int, int > >& edges)
{
	sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) { return get< 2 >(a) < get< 2 >(b); });

	DSU dsu(n);
	int total_weight = 0;

	for (auto& [u, v, weight] : edges)
	{
		if (dsu.find_set(u) != dsu.find_set(v))
		{
			dsu.union_sets(u, v);
			total_weight += weight;
		}
	}
	return total_weight;
}

// Алгоритм Прима (Prim)
int prim_mst(int n, vector< vector< pair< int, int > > >& graph)
{
	vector< bool > in_mst(n, false);
	vector< int > min_edge(n, INT_MAX);
	min_edge[0] = 0;
	int mst_weight = 0;

	priority_queue< pair< int, int >, vector< pair< int, int > >, greater< pair< int, int > > > pq;
	pq.push({ 0, 0 });

	while (!pq.empty())
	{
		int u = pq.top().second;
		int weight = pq.top().first;
		pq.pop();

		if (in_mst[u])
			continue;
		in_mst[u] = true;
		mst_weight += weight;

		for (auto& [v, w] : graph[u])
		{
			if (!in_mst[v] && w < min_edge[v])
			{
				min_edge[v] = w;
				pq.push({ w, v });
			}
		}
	}
	return mst_weight;
}
