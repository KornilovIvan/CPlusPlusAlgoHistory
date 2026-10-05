#include <iostream>
using namespace std;

int main(){
    int n, k = 0;
    cin >> n;
    int a[n];
    for (int i = 0; i < n; i++){
        cin >> a[i];
    }


    for (int i = 0; i < n; i++){
        for (int j = n - 2; j >= i; j--){
            if (a[j] > a[j + 1]){
                swap(a[j], a[j + 1]);
                k++;
            }
        }
    }


    
    cout << k;
}