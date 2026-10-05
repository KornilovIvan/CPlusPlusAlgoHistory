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

int x = 0;

int random(int n){
    int a = 1009, b = 1013, m = 1046527;
    x = (a * x + b) % m;
    return x % n;
}

pair <int, int> partition(vector <int> &a, int left, int right){
    int x1 = left, y1 = right, p = a[left + random(right - left + 1)];
    while (x1 <= y1){
        while (a[x1] < p) x1++;
        while (a[y1] > p) y1--;
        if (x1 <= y1){
            swap(a[x1], a[y1]);
            x1++;
            y1--;
        }
    }
    swap(x1, y1);
    return {x1, y1};
}

void sort(vector <int> &a, int left, int right){
    if (left == right) return;
    if (left + 1 == right){
        if (a[left] > a[right]) swap(a[left], a[right]);
        return;
    }
    pair <int, int> para = partition(a, left, right);
    int x1 = para.fr, y1 = para.sc;
    if (left <= x1) sort(a, left, x1);
    if (y1 <= right) sort(a, y1, right);
}
    
int main(){
    ios_base :: sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int n;
    cin >> n;
    vector <int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    sort(a, 0, n - 1);
    for (auto el : a) cout << el << " ";
    return 0;
}