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

// Динамическое программирование — решение сложной задачи через разбиение на простые подзадачи.
// Два главных свойства
// 	Оптимальная подструктура — решение большой задачи содержит решения маленьких
// 	Перекрывающиеся подзадачи — одни и те же подзадачи решаются много раз

// O(n)

// Числа Фибоначчи
int fibonacci(int n)
{
	if (n <= 1)
		return n;
	vector< int > dp(n + 1);
	dp[0] = 0;
	dp[1] = 1;
	for (int i = 2; i <= n; i++)
	{
		dp[i] = dp[i - 1] + dp[i - 2];
	}
	return dp[n];
}

// Подъем по лестнице
int climbStairs(int n)
{
	if (n <= 2)
		return n;
	vector< int > dp(n + 1);
	dp[0] = 1;	  // способов быть на земле - 1
	dp[1] = 1;	  // на 1 ступеньку - 1 способ
	for (int i = 2; i <= n; i++)
	{
		dp[i] = dp[i - 1] + dp[i - 2];	  // шаг с i-1 или i-2
	}
	return dp[n];
}

// Максимальная сумма подмассива
int maxSubarray(vector< int >& nums)
{
	int n = nums.size();
	vector< int > dp(n);	// dp[i] - макс сумма подмассива, оканчивающегося в i
	dp[0] = nums[0];
	int result = dp[0];

	for (int i = 1; i < n; i++)
	{
		// Либо начинаем новый подмассив, либо продолжаем старый
		dp[i] = max(nums[i], dp[i - 1] + nums[i]);
		result = max(result, dp[i]);
	}
	return result;
}
