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

// O(n^2)
// Наибольшая палиндромная подстрока
string longestPalindrome(string s)
{
	int n = s.length();
	vector< vector< bool > > dp(n, vector< bool >(n, false));
	int start = 0, maxLen = 1;

	// Базовые случаи: подстроки длины 1 и 2
	for (int i = 0; i < n; i++)
	{
		dp[i][i] = true;	// подстрока из одного символа - палиндром
	}

	for (int i = 0; i < n - 1; i++)
	{
		if (s[i] == s[i + 1])
		{
			dp[i][i + 1] = true;
			start = i;
			maxLen = 2;
		}
	}

	// Подстроки длины 3 и больше
	for (int len = 3; len <= n; len++)
	{
		for (int i = 0; i + len <= n; i++)
		{
			int j = i + len - 1;
			if (s[i] == s[j] && dp[i + 1][j - 1])
			{
				dp[i][j] = true;
				if (len > maxLen)
				{
					start = i;
					maxLen = len;
				}
			}
		}
	}

	return s.substr(start, maxLen);
}

// Оптимальное перемножение матриц
int matrixChainMultiplication(vector< int >& dims)
{
	int n = dims.size() - 1;	// количество матриц
	vector< vector< int > > dp(n, vector< int >(n, 0));

	// l - длина цепочки
	for (int l = 2; l <= n; l++)
	{
		for (int i = 0; i + l <= n; i++)
		{
			int j = i + l - 1;
			dp[i][j] = INT_MAX;

			for (int k = i; k < j; k++)
			{
				int cost = dp[i][k] + dp[k + 1][j] + dims[i] * dims[k + 1] * dims[j + 1];
				dp[i][j] = min(dp[i][j], cost);
			}
		}
	}
	return dp[0][n - 1];
}
