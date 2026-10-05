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
    ll size;
    tree* left;
    tree* right;
    tree(ll x) {
        value = x; 
        left = right = 0;
        size = 1;
    }
};

int getsize(tree* a) {
	if (!a) return 0; 
	return a->size; 
}

void fixsize(tree* a) {
	a->size = getsize(a->left) + getsize(a->right) + 1; 
}

tree* find(tree* a, ll x) {
    if (!a) return 0;
    if (x == a->value) return a;
    if (x < a-> value) {
        return find(a->left, x);
    } else {
        return find(a->right, x);
    }
}

tree* rotateright(tree* p) {
	tree* q = p->left; 
	if (!q) return p; 
	p->left = q->right; 
	q->right = p; 
	q->size = p->size; 
	fixsize(p); 
	return q; 
}

tree* rotateleft(tree* q) {
	tree* p = q->right;
	if (!p) return q;
	q->right = p->left;
	p->left = q;
	p->size = q->size;
	fixsize(q);
	return p;
}

tree* insertroot(tree* p, ll k) {
	if (!p) return new tree(k); 
    if (p->value == k) return p;
	if (k < p->value) {
		p->left = insertroot(p->left,k); 
		return rotateright(p); 
	} else {
		p->right = insertroot(p->right,k);
		return rotateleft(p);
	}
}

tree* insert(tree* p, ll k) {
	if (!p) return new tree(k); 
    if (p->value == k) return p;
	if (rand() % (p->size + 1) == 0) return insertroot(p,k); 
	if (p->value > k) {
        p->left = insert(p->left,k); 
    } else {
		p->right = insert(p->right,k); 
    }
    fixsize(p); 
	return p; 
}

tree* join(tree* p, tree* q) {
	if (!p) return q;
	if (!q) return p;
	if (rand() % (p->size + q->size) < p->size ) {
		p->right = join(p->right,q); 
		fixsize(p); 
		return p; 
	} else {
		q->left = join(p,q->left); 
		fixsize(q); 
		return q; 
	}
}

tree* remove(tree* p, ll k) {
	if (!p) return p; 
	if (p->value == k) {
		tree* q = join(p->left,p->right); 
		delete p; 
		return q; 
	} else if (k < p->value) {
        p->left = remove(p->left,k); 
    } else {
        p->right = remove(p->right,k); 
    }
    fixsize(p);
	return p; 
}
    
int main(){
    IOS;
    ll n;
    cin >> n;
    tree* a = new tree(LLONG_MAX);
    while (n --) {
        ll p, x;
        cin >> p >> x;
        if (p == 0) {
            insert(a, x);
        } else if (p == 1) {
            if (find(a, x) == 0) {
                cout << "FALSE" << '\n';
            } else {
                cout << "TRUE" << '\n';
            }
        } else {
            remove(a, x);
        }
    }
    return 0;
}       