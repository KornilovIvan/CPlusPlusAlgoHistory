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

// O(n * W)

int knapsack(vector< int >& weights, vector< int >& values, int W)
{
	int n = weights.size();
	vector< vector< int > > dp(n + 1, vector< int >(W + 1, 0));

	for (int i = 1; i <= n; i++)
	{
		for (int w = 0; w <= W; w++)
		{
			if (weights[i - 1] <= w)
			{
				// Максимум из: не брать предмет или брать предмет
				dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - weights[i - 1]] + values[i - 1]);
			}
			else
			{
				dp[i][w] = dp[i - 1][w];	// не можем взять - наследуем
			}
		}
	}
	return dp[n][W];
}

int knapsackOptimized(vector< int >& weights, vector< int >& values, int W)
{
	int n = weights.size();
	vector< int > dp(W + 1, 0);

	for (int i = 0; i < n; i++)
	{
		// ОБЯЗАТЕЛЬНО в обратном порядке!
		for (int w = W; w >= weights[i]; w--)
		{
			dp[w] = max(dp[w], dp[w - weights[i]] + values[i]);
		}
	}
	return dp[W];
}

vector< int > knapsackWithReconstruction(vector< int >& weights, vector< int >& values, int W)
{
	int n = weights.size();
	vector< vector< int > > dp(n + 1, vector< int >(W + 1, 0));
	vector< vector< bool > > take(n + 1, vector< bool >(W + 1, false));

	for (int i = 1; i <= n; i++)
	{
		for (int w = 0; w <= W; w++)
		{
			if (weights[i - 1] <= w && dp[i - 1][w - weights[i - 1]] + values[i - 1] > dp[i - 1][w])
			{
				dp[i][w] = dp[i - 1][w - weights[i - 1]] + values[i - 1];
				take[i][w] = true;
			}
			else
			{
				dp[i][w] = dp[i - 1][w];
				take[i][w] = false;
			}
		}
	}

	// Восстановление выбранных предметов
	vector< int > selected;
	int w = W;
	for (int i = n; i > 0; i--)
	{
		if (take[i][w])
		{
			selected.push_back(i - 1);	  // индексы с 0
			w -= weights[i - 1];
		}
	}
	reverse(selected.begin(), selected.end());
	return selected;
}
