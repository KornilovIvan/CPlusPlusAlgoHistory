#include <iostream>
#include <cstring>
#include <iomanip>
using namespace std;

int main(){
    int n, m, ryd = 0, stl = 1, i = 1, x = 0, y = -1;
    cin >> n >> m;
    int a[n][m];
    memset(a, 0, sizeof(int) * n * m);
    while (i < n * m + 1){
        if (0 <= x + ryd and x + ryd < n and 0 <= y + stl and y + stl < m and a[x + ryd][y + stl] == 0){
            a[x + ryd][y + stl] = i;
            i++;
            x += ryd;
            y += stl;
        }
        else{
            if (stl == 1){
                ryd = 1;
                stl = 0;
            }
            else if (ryd == 1){
                ryd = 0;
                stl = -1;
            }
            else if (stl == -1){
                ryd = -1;
                stl = 0;
            }
            else if (ryd == -1){
                ryd = 0;
                stl = 1;
            }
        }
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            cout << a[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}