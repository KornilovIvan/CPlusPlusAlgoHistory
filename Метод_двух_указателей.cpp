#include <iostream>
#include <vector>
using namespace std;

// задачу можно решить методом двух указателей если ее можно свести в виду:
//
//
//              l = 0
//              for (int r = 0; r < n; r++){
//                  add(a[r]);
//                  while (!good()){
//                      remove(a[l]);
//                      l++;
//                  }
//               }



// слияние 2 массивов в один (по возрастанию)
int main(){
    int n, m, i = 0, j = 0;
    cin >> n >> m;
    vector <long long> a(n);
    vector <long long> b(m);
    vector <long long> c(m + n);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < m; i++) cin >> b[i];

    while (i < n or j < m){
        if (j == m or i < n and a[i] < b[j]){
            c[i + j] = a[i];
            i++;
        }else{
            c[i + j] = b[j];
            j++;
        }
    }

    for (auto el : c){
        cout << el << " ";
    }
    return 0;
}



// Даны два массива, отсортированные по неубыванию. Для каждого элемента второго массива найдите число элементов первого массива, строго меньших его.

#include <iostream>
#include <vector>
using namespace std;

int main(){
    int n, m;
    cin >> n >> m;
    vector <long long> a(n);
    vector <long long> b(m);
    vector <long long> res(m);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < m; i++) cin >> b[i]; 
    int i = 0;
    for (int j = 0; j < m; j++){
        while (i < n and a[i] < b[j]) i++;
        res[j] = i;
    }
    for (auto el : res){
        cout << el << " ";
    }
    return 0;
}


// Дан массив из n чисел ai. Скажем, что отрезок этого массива a[l..r] (1≤l≤r≤n) хороший, если сумма чисел на этом отрезке не больше s. Ваша задача — найти самый длинный хороший отрезок.

#include <iostream>
#include <vector>
using namespace std;

int main(){
    int n;
    long long s;
    cin >> n >> s;
    vector <long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    long long l = 0;
    long long x = 0, res = 0;


    for (long long r = 0; r < n; r++){
        x += a[r];

        while (x > s){
            x -= a[l];
            l ++;
        }

        res = max(res, (r - l + 1));
    }
    cout << res;
    return 0;
}





// Дан массив из n чисел ai. Скажем, что отрезок этого массива a[l..r] (1≤l≤r≤n) хороший, если сумма чисел на этом отрезке не меньше s. Ваша задача — найти самый короткий хороший отрезок.

#include <iostream>
#include <vector>
using namespace std;

int main(){
    long long n;
    long long s;
    cin >> n >> s;
    vector <long long> a(n);
    for (long long i = 0; i < n; i++) cin >> a[i];
    long long x = 0, res = LONG_LONG_MAX, l = 0;
    for (long long r = 0; r < n; r++){
        x += a[r];
        while (x - a[l] >= s){
            x -= a[l];
            l++;
        }
        if (x >= s){
            res = min(res, (r - l + 1));
        }
    }
    if (res == LONG_LONG_MAX){
        cout << -1;
    }else{
        cout << res;
    }
    return 0;
}



