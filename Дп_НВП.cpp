#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <math.h>
#include <string>
#include <set>
#include <iomanip>
#include <queue>
#include <bitset>
using namespace std;
typedef long long ll;
#define pb push_back
#define fr first
#define sc second
#define all(A) A.begin(), A.end()
    
int main(){
    // НВП за квадрат (с восстановлением ответа)
    // в dp[i] хранится длина наибольшей возрастающей последовательности заканчивающейся на i элементе
    int n;
    cin >> n;
    vector <int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    vector <int> dp(n), p(n);
    for (int i = 0; i < n; i++){
        dp[i] = 1;
        p[i] = -1;
        for (int j = 0; j < i; j++){
            if (a[j] < a[i] and dp[j] + 1 > dp[i]){
                dp[i] = dp[j] + 1;
                p[i] = j;
            }
        }
    }
    int res = -1, poz;
    for (int i = 0; i < n; i++){
        if (dp[i] > res){
            res = dp[i];
            poz = i;
        }
    }
    vector <int> path;
    for (int v = poz; v != -1; v = p[v]){
        path.pb(a[v]);
    }
    reverse(all(path));
    for (auto el : path){
        cout << el << " ";
    }

    // НВП за nlogn
    //  в dp[i] хранится число на которое заканчивается наибольшая возрастающая последовательность длины i
    int n;
    cin >> n;
    vector <int> a(n);
    vector <int> dp(n + 1, 1000000000), p(n, -1), p1(n, -1);
    dp[0] = -1000000000;
    for (int i = 0; i < n; i++){
        int j = int(upper_bound(all(dp), a[i]) - dp.begin());
        if (dp[j - 1] < a[i] and a[i] < dp[j]){
            dp[j] = a[i];
            p[j] = i;                                         // храним индекс элемента на который оканчивается последовательность длины j
            p1[i] = p[j - 1];                                 // храним для каждого a[i] его предка в оптимальной последовательности
        }
    }
    int indx;
    for(int i = 0; i < n; i++){
        if (dp[i] == 1000000000) break;
        indx = i;
    }
    vector <int> path;
    for (int v = p[indx]; v != -1; v = p1[v]){
        path.pb(a[v]);
    }
    reverse(all(path));
    for (auto el : path){
        cout << el << " ";
    }
    return 0;
}