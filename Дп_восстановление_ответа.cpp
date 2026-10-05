#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;

// В верхнем-левом углу таблицы сидит черепаха. Она умеет ходить только вниз, вправо и вниз-вправо. В каждой клетке записано число, и черепашка прибавляет его к своей сумме, когда оказывается на клетке. Черепаха хочет попасть в нижнюю-правую клетку, получив в итоге минимально возможную сумму. Помогите ей - посчитайте, какую сумму она в итоге наберет?

int main() {
    int n, m;
    cin >> n >> m;
    vector <vector<int>> a(n + 1, vector <int> (m + 1));
    for (int i = 1; i < n + 1; i++){
        for (int j = 1; j < m + 1; j++){
            cin >> a[i][j];
        }
    }
    int u = 1000000;
    vector <vector<int>> dp(n + 1, vector <int> (m + 1, u));
    dp[0][0] = 0;
    for (int i = 1; i < n + 1; i++){
        for (int j = 1; j < m + 1; j++){
            dp[i][j] = min(dp[i][j - 1], min(dp[i - 1][j], dp[i - 1][j - 1])) + a[i][j];
        }
    }



    vector <pair<int, int>> res;
    int x = n, y = m;
    while (x > 0 and y > 0){
        res.push_back({x, y});
        if (a[x][y] + dp[x - 1][y] == dp[x][y]){
            x --;
        }
        else if (a[x][y] + dp[x][y - 1] == dp[x][y]){
            y--;
        }else{
            x--;
            y--;
        }
    }



    
    cout << dp[n][m] << " " << res.size() << '\n';
    for (int i = res.size() - 1; i != -1; i--){
        cout << res[i].first << " " << res[i].second << '\n';
    }
    return 0;
}