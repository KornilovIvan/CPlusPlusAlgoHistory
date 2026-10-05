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
#define IOS ios_base :: sync_with_stdio(false); cin.tie(0); cout.tie(0)

ll kth(vector <ll> &a, ll left, ll right, ll k){
    if (right - left == 1) return a[left];
    ll x = a[(right + left - 1) / 2];
    ll i = left, j = right - 1;
    while (i <= j){
        while (a[i] < x) i++;
        while (a[j] > x) j--;
        if (i <= j){
            swap(a[i], a[j]);
            i++;
            j--;
        }
    }
    if (k <= j + 1) return kth(a, left, j + 1, k);
    else if (k < i + 1) return x;
    else return kth(a, i, right, k);
}

int main(){
    ll n, k, x1, x2, a, b, c, r;
    cin >> n >> k >> x1 >> x2 >> a >> b >> c >> r;
    vector <ll> mas(n);
    mas[0] = x1;
    mas[1] = x2;
    for (int i = 2; i < n; i++) mas[i] = (a * mas[i - 1] + b * mas[i - 2] + c) % r;
    cout << kth(mas, 0, n, k);
    return 0;
}