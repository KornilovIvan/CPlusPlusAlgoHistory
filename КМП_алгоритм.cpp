#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <math.h>
#include <string>
#include <set>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
#define pb push_back
#define fr first
#define sc second
#define all(A) A.begin(), A.end()

// Алгоритм КМП (Кнута-Морриса-Пратта) - нахождение подстроки в строке (жрет меньше памяти, чем Z функция, но медленнее)
    
int main(){
    string s = "abacaba";
    vector <int> pi(s.size());
    int j = 0, i = 1;
    while (i < s.size()){     // префикс функция для образца, который ищем в строке
        if (s[j] == s[i]){
            pi[i] = j + 1;
            i ++;
            j ++;
        }else{
            if (j == 0){
                pi[i] = 0;
                i++;
            }else{
                j = pi[j - 1];
            }
        }
    }
    string str = "abacabbabacaba";
    i = 0;
    j = 0;
    while (i < str.size()){
        if (str[i] == s[j]){
            i++;
            j++;
            if (j == s.size()){
                cout << "stroka naidena\n";
                return 0;
            }
        }else{
            if (j > 0){
                j = pi[j - 1];
            }else{
                i += 1;
            }
        }
    }
    if (i == str.size()){
        cout << "stroka ne naidena\n";  
    }
    return 0;
}