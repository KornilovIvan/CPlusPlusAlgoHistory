#include <iostream>
using namespace std;


// a * b = НОК(a, b) * НОД(a, b)

// =>      НОК(a, b) = (a * b) / НОД(a, b)

// НОД 2 чисел составленных только из едениц, равен числу, в котором кол-во едениц равно НОД кол-ва ед в этих числах
// #include <iostream>
// using namespace std;
// int main() {
//     int n, m;
//     cin >> n >> m;
//     while (n %= m){
//         swap(m, n);
//     }
//     for (int i = 0; i < m; i++){
//         cout << '1';
//     }
//     return 0;
// }

int main(){
    int a, b;
    cin >> a >> b;
    // нахождение НОД с помощью алгоритма евклида
    while (b %= a){
        swap(a, b);
    }
    cout << a << endl;
    return 0;
}