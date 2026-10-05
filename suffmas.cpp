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

int main(){
    string str;
    cin >> str;
    str += "$";
    int n = str.size();
    vector <int> p(n), c(n);   // p - порядок, с - класс эквивалентности

    {
        // k = 0
        vector <pair<char, int>> a(n);
        for (int i = 0; i < n; i++) a[i] = {str[i], i};
        sort(all(a));
        
        for (int i = 0; i < n; i++) p[i] = a[i].sc;

        c[p[0]] = 0;
        for (int i = 1; i < n; i++){
            if (a[i].fr == a[i - 1].fr){
                c[p[i]] = c[p[i - 1]];
            }else{
                c[p[i]] = c[p[i - 1]] + 1;
            }
        }
    }

    int k = 0;
    while ((1 << k) < n){
        // k -> k + 1
        vector<pair<pair<int, int>, int>> a(n);
        for (int i = 0; i < n; i++) a[i] = {{c[i], c[(i + (1 << k)) % n]}, i};
        sort(all(a));

        for (int i = 0; i < n; i++) p[i] = a[i].sc;

        c[p[0]] = 0;
        for (int i = 1; i < n; i++){
            if (a[i].fr == a[i - 1].fr){
                c[p[i]] = c[p[i - 1]];
            }else{
                c[p[i]] = c[p[i - 1]] + 1;
            }
        }
        k++;
    }
    for (auto el : p){
        cout << el << " ";
    }

    return 0;   
}