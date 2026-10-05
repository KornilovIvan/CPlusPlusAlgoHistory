#include<iostream>
#include<ctime> // библиотека для time
using namespace std;

int main(){
    srand(time(NULL));
    int a = rand() % 10; // будет генерироваться числа от 0 до 10
    cout<< a << endl;

    int b = rand() % 10 + 5; // будет генерироваться числа от 5 до 15
    cout<<b << endl;
}