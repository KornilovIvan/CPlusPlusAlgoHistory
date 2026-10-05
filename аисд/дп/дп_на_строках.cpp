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
// Наибольшая общая подпоследовательность (LCS)
int longestCommonSubsequence(string s1, string s2)
{
	int n = s1.length(), m = s2.length();
	vector< vector< int > > dp(n + 1, vector< int >(m + 1, 0));

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			if (s1[i - 1] == s2[j - 1])
			{
				dp[i][j] = dp[i - 1][j - 1] + 1;
			}
			else
			{
				dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
			}
		}
	}
	return dp[n][m];
}

// Восстановление LCS
string getLCS(string s1, string s2)
{
	int n = s1.length(), m = s2.length();
	vector< vector< int > > dp(n + 1, vector< int >(m + 1, 0));

	// Заполнение таблицы
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			if (s1[i - 1] == s2[j - 1])
			{
				dp[i][j] = dp[i - 1][j - 1] + 1;
			}
			else
			{
				dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
			}
		}
	}

	// Восстановление
	string lcs = "";
	int i = n, j = m;
	while (i > 0 && j > 0)
	{
		if (s1[i - 1] == s2[j - 1])
		{
			lcs = s1[i - 1] + lcs;
			i--;
			j--;
		}
		else if (dp[i - 1][j] > dp[i][j - 1])
		{
			i--;
		}
		else
		{
			j--;
		}
	}
	return lcs;
}

// O(n * m)
// Расстояние Левенштейна (редакционное расстояние) — это минимальное количество операций, необходимых для
// преобразования одной строки в другую. Операции: Вставка символа, Удаление символа, Замена символа.
int editDistance(string s1, string s2)
{
	int n = s1.length(), m = s2.length();
	vector< vector< int > > dp(n + 1, vector< int >(m + 1, 0));

	// Базовые случаи
	for (int i = 0; i <= n; i++)
		dp[i][0] = i;	 // удалить все символы из s1
	for (int j = 0; j <= m; j++)
		dp[0][j] = j;	 // вставить все символы в s1

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			if (s1[i - 1] == s2[j - 1])
			{
				dp[i][j] = dp[i - 1][j - 1];	// символы совпадают - без изменений
			}
			else
			{
				dp[i][j] =
					1 + min({
							dp[i - 1][j],		// удаление
							dp[i][j - 1],		// вставка
							dp[i - 1][j - 1]	// замена
						});
			}
		}
	}
	return dp[n][m];
}
