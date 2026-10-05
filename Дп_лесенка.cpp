#include <iostream>
#include <vector>
using namespace std;
 
int main() {
    int n;
    cin >> n;
    vector <long long> a(n + 1);
    if (n == 1){
        cout << 1;
        return 0;
    }
    if (n == 2){
        cout << 2;
        return 0;
    }
    a[0] = 0;
    a[1] = 1;
    a[2] = 2;
    a[3] = 4;
    for (int i = 4; i <= n; i++){
        a[i] = a[i - 1] + a[i - 2] + a[i - 3];
    }
    cout << a[n];
    return 0;
}