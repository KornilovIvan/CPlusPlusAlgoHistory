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

void merge(vector <int> &a, int left1, int right1, int left2, int right2){
    vector <int> res = a;
    int i = left1, j = left2, k = left1;
    while (i <= right1 and j <= right2){
        if (a[i] <= a[j]){
            res[k] = a[i];
            i ++;
        }else{
            res[k] = a[j];
            j++;
        }
        k++;
    }
    while (i <= right1){
        res[k] = a[i];
        i++;
        k++;
    }
    while (j <= right2){
        res[k] = a[j];
        j++;
        k++;
    }
    a = res;
}

void sort(vector <int> &a, int left, int right){
    if (left == right) return;
    if (left + 1 == right){
        if (a[left] > a[right]) swap(a[left], a[right]);
        return;
    }
    int mid = (left + right) / 2;
    sort(a, left, mid);
    sort(a, mid + 1, right);
    merge(a, left, mid, mid + 1, right);
}
 
int main(){
    int n;
    cin >> n;
    vector <int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    sort(a, 0, n - 1);
    cout << "result:" << " ";
    for (auto el : a) cout << el << " ";
    return 0;
}