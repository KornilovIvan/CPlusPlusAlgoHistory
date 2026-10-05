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
        
    vector <ll> R {50021, 50023, 50033, 50047, 50051, 50053, 50069, 50077, 50087, 50093};
    ll Q = 3 * 1e7;
    bitset<30000000> a;
        
    ll mod(ll x, ll q) {
        return (x % q + q) % q;
    }
     
    void add(string &str) {
        for (auto el : R) {
            ll h = 0;
            for (int i = 0; i < str.size(); i++) {
                h = mod(h * el + str[i], Q);
            }
            a[h] = 1;
        }
    }
     
    bool p(string &str) {
        for (auto el : R) {
            ll h = 0;
            for (int i = 0; i < str.size(); i++) {
                h = mod(h * el + str[i], Q);
            }
            if (a[h] == 0) return false;
        }
        return true;
    }
        
    int main(){
        int n;
        cin >> n;
        string res = "";
        while (n--) {
            string str;
            cin >> str;
            string str1 = str.substr(1, str.size() - 1);
            if (str[0] == '+') {
                add(str1);
            } else {
                res += to_string(p(str1));
            }
        }
        cout << res;
        return 0;
    }