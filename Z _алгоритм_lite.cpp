#include <iostream>
#include <vector>
using namespace std;

// наивный алгоритм нахождения Z функции строки

int main(){
    string str = "abacaba";
    int n = str.size();
    vector <int> res(n);
    for (int i = 1; i < n; i++){
        while (res[i] + i < n and str[res[i] + i] == str[res[i]]){
            res[i] ++;
        }
    }
    for (auto el : res){
        cout << el << " ";
    }
    return 0;
}