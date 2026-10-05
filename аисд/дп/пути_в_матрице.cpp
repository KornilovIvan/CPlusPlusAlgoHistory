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

// O(n * m)
// Уникальные пути
int uniquePaths(int m, int n)
{
	vector< vector< int > > dp(m, vector< int >(n, 1));

	for (int i = 1; i < m; i++)
	{
		for (int j = 1; j < n; j++)
		{
			dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
		}
	}

	return dp[m - 1][n - 1];
}

// Минимальная сумма пути
int minPathSum(vector< vector< int > >& grid)
{
	int m = grid.size(), n = grid[0].size();
	vector< vector< int > > dp(m, vector< int >(n, 0));

	dp[0][0] = grid[0][0];

	// Первая строка
	for (int j = 1; j < n; j++)
	{
		dp[0][j] = dp[0][j - 1] + grid[0][j];
	}

	// Первый столбец
	for (int i = 1; i < m; i++)
	{
		dp[i][0] = dp[i - 1][0] + grid[i][0];
	}

	// Остальные клетки
	for (int i = 1; i < m; i++)
	{
		for (int j = 1; j < n; j++)
		{
			dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
		}
	}

	return dp[m - 1][n - 1];
}
