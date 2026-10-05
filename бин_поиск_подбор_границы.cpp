#include <iostream>
using namespace std;

bool good(long long w, long long h, long long n, long long mid){
    return (mid / w) * (mid / h) >= n;
}



int main(){
    long long w, h, n;
    cin >> w >> h >> n;
    long long left = 0, right = 1, mid;


    while (!good(w, h, n, right)){  // подберем правую границу чтобы избежать переполнения long long
        right *= 2;
    }


    while (left + 1 < right){
        mid = (left + right) / 2;
        if (good(w, h, n, mid)){
            right = mid;
        }else{
            left = mid;
        }
    }
    cout << right;
    return 0;
}