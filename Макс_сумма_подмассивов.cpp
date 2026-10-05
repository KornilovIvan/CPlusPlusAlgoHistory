#include <iostream>
#include <vector>
using namespace std;


// Максимальная сумма подмассивов

int main(){
    vector <int> a {-1, 2, 4, -3, 5, 2, -5, 2};
    int summ = 0, best = 0;
    for (int i = 0; i < a.size(); i++) {
        summ = max(a[i], summ + a[i]);
        best = max(best, summ);
    }
    cout << best << '\n';
    return 0;
}