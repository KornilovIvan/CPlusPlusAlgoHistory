#include <iostream>
#include <vector>
using namespace std;

// Динамика:
// задача: разменять сумму n, используя как можно меньше монет.

int main(){
    int n = 10;
    vector <int> coins{1, 3, 4};
    vector <int> value(n + 1);
    value[0] = 0;
    for (int x = 1; x <= n; x++){
        value[x] = INT_MAX;
        for (auto c : coins){
            value[x] = min(value[x], value[x - c] + 1);
        }
    }
    cout << value[n];
    return 0;
}