#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <math.h>
#include <string>
#include <set>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
#define pb push_back
#define fr first
#define sc second
#define all(A) A.begin(), A.end()

int main(){
    string s;
    cin >> s;
    ll l = 0, r = 0, n = s.size();
    vector <ll> z(n);
    for (ll i = 1; i < n; i++){
        if (i <= r){
            z[i] = min(z[i - l], r - i + 1);  // Если z[i-l] < r-i+1, то тогда и z[i] = z[i-l]. В этом случае значение z[i] подсчитано. Если z[i-l]≥r-i+1, то z[i] точно больше или равно r-i+1, то точное значение неизвестно. Наивным способом будем увеличивать z[i] пока можно, получим точное значение.
        }
        while (z[i] + i < n and s[z[i] + i] == s[z[i]]){  // наивный способ
            z[i] ++;
        }
        if (i + z[i] - 1 > r){   // новые границы повторяющихся строк
            l = i;
            r = i + z[i] - 1;
        }
    } 
    for (auto el : z){
        cout << el << " ";
    }
    return 0;
}
