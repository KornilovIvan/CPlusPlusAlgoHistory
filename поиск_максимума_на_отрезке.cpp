#include <iostream>
#include <vector>
#include <math.h>
#include <map>
#include <string>
#include <algorithm>
#include <queue>
#include <set>
#include <iomanip>
using namespace std;
typedef long long ll;

vector <ll> b, c;

void p(vector <ll> a, int k){
	int n = a.size();
	b.resize(n);
	c.resize(n);
	k--;
	for(int i = 0; i < n; i++){
		if(i % k == 0) {
            b[i] = a[i];
        }else{
            b[i] = max(a[i], b[i - 1]);
        }
	}

	c.back() = a.back();
	for(int i = n - 2; i >= 0; i--){
		if((i + 1) % k == 0){
            c[i] = a[i];
        }else{
            c[i] = max(a[i], c[i + 1]);
        }
	}	
}

int main(){
    int n;
    cin >> n;
    vector <ll> a(n);
    
    return 0;
}