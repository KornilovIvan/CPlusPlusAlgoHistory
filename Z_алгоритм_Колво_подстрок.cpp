#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <math.h>
#include <string>
#include <set>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
#define pb push_back
#define fr first
#define sc second
#define all(A) A.begin(), A.end()

//    https://e-maxx.ru/algo/z_function - листай ниже

// Количество различных подстрок в строке

int z_alg(string s){
    int l = 0, r = 0, n = s.size();
    vector <int> z(n);
    int maxi = 0;
    for (int i = 1; i < n; i++){
        if (i <= r){
            z[i] = min(z[i - l], r - i + 1);
        }
        while (z[i] + i < n and s[z[i]] == s[z[i] + i]){
            z[i] ++;
        }
        if (r < i + z[i] - 1){
            l = i;
            r = i + z[i] - 1;
        }
        maxi = max(maxi, z[i]);
    }
    return maxi;
}

int main(){
    string str;
    cin >> str;
    string re = "";
    int res = 0;
    for (int j = 0; j < str.size(); j++){
        re += str[j];
        string s = re;
        reverse(all(s));
        res += s.size() - z_alg(s);
    }
    cout << res;
    return 0;
}   