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

// Минимальное количество монет
int coinChange(vector< int >& coins, int amount)
{
	vector< int > dp(amount + 1, INT_MAX);
	dp[0] = 0;	  // для суммы 0 нужно 0 монет

	for (int s = 1; s <= amount; s++)
	{
		for (int coin : coins)
		{
			if (s >= coin && dp[s - coin] != INT_MAX)
			{
				dp[s] = min(dp[s], dp[s - coin] + 1);
			}
		}
	}

	return dp[amount] == INT_MAX ? -1 : dp[amount];
}

// Количество способов размена
int coinChangeWays(vector< int >& coins, int amount)
{
	vector< int > dp(amount + 1, 0);
	dp[0] = 1;	  // 1 способ получить сумму 0 - не брать монеты

	for (int coin : coins)
	{
		for (int s = coin; s <= amount; s++)
		{
			dp[s] += dp[s - coin];
		}
	}

	return dp[amount];
}
