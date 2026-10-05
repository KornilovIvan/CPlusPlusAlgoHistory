#include <iostream>
using namespace std;
 
int main() {
    int n, m;
    cin >> n >> m;
    int a[n][m];
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            cin >> a[i][j];
        }
    }
    int summ[n][m];
    summ[0][0] = a[0][0];
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            if (i == 0 and j == 0){
                continue;
            }
            if (i == 0){
                summ[i][j] = summ[i][j - 1] + a[i][j];
            }
            else if (j == 0){
                summ[i][j] = summ[i - 1][j] + a[i][j];
            }else{
                summ[i][j] = min(summ[i - 1][j], summ[i][j - 1]) + a[i][j];
            }
        }
    }
    cout << summ[n - 1][m - 1];
    return 0;
}