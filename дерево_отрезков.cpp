#include <iostream>
#include <vector>
using namespace std;
typedef long long ll;

struct segtree{
    vector <ll> tree;
    int size;
 
    void init(int n){          
        size = 1;
        while (size < n) size *= 2;  
        tree.assign(2 * size - 1, 0); 
    }

    void build(vector <ll> &a, int x, int lx, int rx){
        if (rx - lx == 1){
            if (lx < a.size()){
                tree[x] = a[lx];
            }
        }else{
            int m = (lx + rx) / 2;
            build(a, 2 * x + 1, lx, m);
            build(a, 2 * x + 2, m, rx);
            tree[x] = tree[2 * x + 1] + tree[2 * x + 2];
        }
    }

    void build(vector <ll> &a){
        init(a.size());
        build(a, 0, 0, size);
    }
 
    void set(int i, int v, int x, int lx, int rx){
        if (rx - lx == 1){          
            tree[x] = v;
            return;
        }
        int m = (rx + lx) / 2;
        if (i < m){
            set(i, v, 2 * x + 1, lx, m);
        }else{
            set(i, v, 2 * x + 2, m, rx);
        }
        tree[x] = tree[2 * x + 1] + tree[2 * x + 2];
    }
 
    void set(int i, int v){
        set(i, v, 0, 0, size);
    }
 
    ll sum(int l, int r, int x, int lx, int rx){
        if (l >= rx or r <= lx){
            return 0;
        }
        if (lx >= l and rx <= r){
            return tree[x];
        }
        int m = (rx + lx) / 2;
        ll s1 = sum(l, r, 2 * x + 1, lx, m);
        ll s2 = sum(l, r, 2 * x + 2, m, rx);
        return s1 + s2;
    }
 
    ll sum(int l, int r){
        return sum(l, r, 0, 0, size);
    }
};


int main(){
    ios_base :: sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int n, m;
    cin >> n >> m;
    segtree st;
    vector <ll> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    st.build(a);
    for (int i = 0; i < m; i++){
        int c;
        cin >> c;
        if (c == 1){
            int l, v;
            cin >> l >> v;
            st.set(l, v);
        }else{
            int l, r;
            cin >> l >> r;
            cout << st.sum(l, r) << '\n';
        }
    }
    return 0;
}