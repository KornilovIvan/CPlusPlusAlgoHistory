#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
using namespace std;

int main(){
    int n;
    long long w, resw = 0;
    double res = 0, c1, w1;
    cin >> n >> w;
    vector <pair <double, pair <long long, long long>>> a(n);

    for (int i = 0; i < n; i ++){
        cin >> c1 >> w1;
        pair <long long, long long> s;
        s = make_pair(c1, w1);
        a[i] = make_pair(c1 / w1, s);
    }
    
    sort(a.begin(), a.end());
    reverse(a.begin(), a.end());

    for (int i = 0; i < n; i++){

        if (resw + a[i].second.second < w){

            resw += a[i].second.second;
            res += a[i].second.first;

        }else{
            res += (w - resw) *  a[i].first;
            break;
        }
    }

    cout << fixed << setprecision(3) << res; 
}
