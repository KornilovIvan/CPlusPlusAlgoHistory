#include <iostream>
#include <vector>
#include <string>
using namespace std;
typedef long long ll;

// Имеется калькулятор, который выполняет следующие операции:

//     Умножить число X на 2.
//     Умножить число X на 3.
//     Прибавить к числу XX единицу.

//  Определите, какое наименьшее количество операций требуется, чтобы получить из числа 1 число N.

int main(){
    int n;
    cin >> n;
    vector <ll> dp(n + 1);
    dp[1] = 0;
    for (int i = 2; i < n + 1; i++){
        dp[i] = dp[i - 1] + 1;
        if (i % 2 == 0){
            dp[i] = min(dp[i / 2] + 1, dp[i]);
        }
        if (i % 3 == 0){
            dp[i] = min(dp[i / 3] + 1, dp[i]);
        }
    }
    cout << dp[n];
    return 0;
}