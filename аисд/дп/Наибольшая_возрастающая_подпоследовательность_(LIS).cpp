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

// Базовая версия O(n²)
int longestIncreasingSubsequence(vector< int >& nums)
{
	int n = nums.size();
	vector< int > dp(n, 1);	   // dp[i] - длина НВП, оканчивающейся в i

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (nums[j] < nums[i])
			{
				dp[i] = max(dp[i], dp[j] + 1);
			}
		}
	}

	return *max_element(dp.begin(), dp.end());
}

// Восстановление LIS
vector< int > getLIS(vector< int >& nums)
{
	int n = nums.size();
	vector< int > dp(n, 1);
	vector< int > prev(n, -1);
	int maxIndex = 0;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (nums[j] < nums[i] && dp[j] + 1 > dp[i])
			{
				dp[i] = dp[j] + 1;
				prev[i] = j;
			}
		}
		if (dp[i] > dp[maxIndex])
		{
			maxIndex = i;
		}
	}

	// Восстановление
	vector< int > lis;
	int current = maxIndex;
	while (current != -1)
	{
		lis.push_back(nums[current]);
		current = prev[current];
	}
	reverse(lis.begin(), lis.end());
	return lis;
}
