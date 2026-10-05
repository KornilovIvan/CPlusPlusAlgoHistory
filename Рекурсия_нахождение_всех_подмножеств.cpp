#include <iostream>
#include <vector>
using namespace std;
typedef long long ll;

vector <int> subset;

// нахождение всех подмножеств множества из n элементов

void search(int k, vector <int> a, int n){
    if (k == n + 1){
        for (auto el : subset){
            cout << el << " ";
        }
        cout << endl;
    }else{
        subset.push_back(a[k - 1]);
        search(k + 1, a, n);
        subset.pop_back();
        search(k + 1, a, n);
    }
}
    
int main(){
    vector <int> a{2, 1, 3, 4};
    int n = a.size();
    search(1, a, n);
    return 0;
}
