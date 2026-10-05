#include <iostream>
#include <vector>
#include <math.h>
#include <map>
#include <string>
#include <algorithm>
#include <queue>
#include <set>
using namespace std;
typedef long long ll;

int main(){
    int n;
    cin >> n;
    vector <ll> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    ll summ = 0;
    ll maxi = LLONG_MIN;
    ll minsumm = 0;
    for (int r = 0; r < n; r++){        // макс сумма подмассива
        summ += a[r];
        maxi =  max(maxi, summ - minsumm);
        minsumm = min(minsumm, summ);
    }
    cout << maxi << '\n';




    ll summ1 = 0;
    ll mini = LLONG_MAX;
    ll maxsumm = 0;
    for (int r = 0; r < n; r++){      // мин сумма подмассива
        summ += a[r];
        mini =  min(mini, summ - maxsumm);
        maxsumm = max(maxsumm, summ1);
    }
    cout << mini << '\n';
    return 0;
}