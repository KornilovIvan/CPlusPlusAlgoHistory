#include<iostream>
using namespace std;


/* Шаблонные функции могут принимать разные типы данных*/


template <class T1, class T2>     // T1 и T2 - разные типы которые принимает функция
T1 summa(T1 a, T2 b){          // перед функцией тип, который будет она возвращать
    return a + b;
}

int main(){
    int a;
    float b;
    cin >> a >> b;
    cout << summa(a, b) << endl;
}