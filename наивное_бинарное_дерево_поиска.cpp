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
    
struct tree {
    ll value;
    tree* left;
    tree* right;
    tree(ll x) {
        value = x; 
        left = right = 0;
    }
};
    
tree* find(tree* a, ll x) {
    if (!a) return 0;
    if (x == a->value) return a;
    if (x < a-> value) {
        return find(a->left, x);
    } else {
        return find(a->right, x);
    }
}
    
tree* insert(tree* a, ll x) {
    if (!a) return new tree(x);
    if (a->value > x) {
        a->left = insert(a->left, x);
    } else {
        a->right = insert(a->right, x);
    }
    return a;
}
    
int main(){
    IOS;
    ll n;
    cin >> n;
    tree* a = new tree(LLONG_MAX);
    while (n--) {
        ll m, x;
        cin >> m >> x;
        if (m == 1) {
            if (find(a, x)) {
                cout << 0 << ' ';
            } else {
                cout << 1 << ' ';
                insert(a, x);
            }
        } else {
            if (find(a, x)) {
                cout << 1 << ' ';
            } else {
                cout << 0 << ' ';
            }
        }
    }
    return 0;
}       