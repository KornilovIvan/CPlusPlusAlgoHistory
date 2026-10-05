#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
 
int main() {
    int n;
    cin >> n;
    vector <int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    sort(a.begin(), a.end());
    vector <int> dp(n);
    dp[0] = 0;
    dp[1] = a[1] - a[0];
    dp[2] = a[2] - a[1] + dp[1];
    for (int i = 3; i < n; i++){
        dp[i] = min(dp[i - 1], dp[i - 2]) + a[i] - a[i - 1];
    }
    cout << dp[dp.size() - 1];
    return 0;
}