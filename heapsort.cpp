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
const int INF = int(1e9);

void heapify(vector <int> &a, int n, int i) {
    int largest = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < n and a[l] > a[largest]) {
        largest = l;
    }
    if (r < n and a[r] > a[largest]) {
        largest = r;
    }
    if (largest != i) {
        swap(a[largest], a[i]);
        heapify(a, n, largest);
    }
}

void heapsort(vector <int> &a) {
    for (int i = a.size() / 2 + 1; i > -1; i--) {
        heapify(a, a.size(), i);
    }
    for (int i = a.size() - 1; i > -1; i--) {
        swap(a[0], a[i]);
        heapify(a, i, 0);
    }
}

void siftup(vector <int> &a, int i) {
    while (a[i] < a[(i - 1) / 2]) {
        swap(a[i], a[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void siftdown(vector <int> &a, int i) {
    while (2 * i + 1 < a.size()) {    
        int left = 2 * i + 1;          
        int right = 2 * i + 2;           
        int j = left;
        if (right < a.size() and a[right] < a[left]) {
            j = right;
        }
        if (a[i] <= a[j]) break;
        swap(a[i], a[j]);
        i = j;
    }
}

int main(){
    int n;
    cin >> n;
    vector <int> a(n);
    for (int i = 0; i < n; i++) cin >> a[n];
    heapsort(a);
    for (auto el : a) {
        cout << el << ' ';
    }
    return 0;
}