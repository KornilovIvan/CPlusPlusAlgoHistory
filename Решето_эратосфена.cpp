#include <iostream>
#include <vector>
using namespace std;

int main(){
    int n;
    cin >> n;
    vector <int> a(n + 1);
    for (int i = 0; i < n + 1; i++) a[i] = i;
    
    int i = 2, j; 
    while (i <= n){
        if (a[i] != 0){
            j = i + i;
            while (j <= n){
                a[j] = 0;
                j += i;
            }
        }
        i++;
    }
    return 0;
}
