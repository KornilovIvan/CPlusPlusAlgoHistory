#include <iostream>
#include <string>
#include <math.h>
using namespace std;
    

// число в системе счисления с основанием K делится на K-1 тогда и только тогда, когда сумма цифр данного числа так же делится на K-1




// Перевод из 2 в 8 сс
// 10011011
// 010 011 011
// (0 * 4 + 1 * 2 + 0 * 1)     (0 * 4 + 1 * 2 + 1 * 1)      (0 * 4 + 1 * 2 + 1 * 1)
//         2                              3                           3
//    ==>   10011011 в 2 == 233 в 8 сс


int main(){
    int n;
    cin >> n;
    for (int i = 0; i < n; i++){
        string str;
        cin >> str;
        int x = 0;

        while (str.size() % 3 != 0){     // добавление незначащих нулей (чтобы сформировать тройки двоичных цифр)
            str = '0' + str;
        }

        while (str > ""){

            x += ((int)(str[0]) - '0') * 4 + ((int)(str[1]) - '0') * 2 + ((int)(str[2]) - '0');
            str.erase(0, 3);

        }



        if (x % 7 == 0){
            cout << "Yes" << endl;
        }else{
            cout << "No" << endl;
        }
    }
    return 0;
}


