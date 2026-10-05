#include <iostream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

bool proverka(string &str){
    if (str.size() != 5 or str[2] != '-' or !isdigit(str[1]) or !isdigit(str[4]) or !isalpha(str[0]) or !isalpha(str[3])){
        return 0;
    }
    if ((int)str[1] - '0' < 1 or (int)str[1] - '0' > 8 or (int)str[4] - '0' > 8 or (int)str[4]- '0' < 1){
        return 0;
    }
    string s = "ABCDEFGH";
    if (s.find(str[0]) == -1 or s.find(str[3]) == -1){
        return 0;
    }
    return 1;
}

int main(){
    string str;
    cin >> str;
    if (proverka(str)){
        int a = str[0] - 65, b = str[1] - '0', c = str[3] - 65, d = str[4] - '0';



        abs((a - c) * (b - d)) == 2? cout << "YES": cout << "NO";    // //// ///// ////// 




    }else{
        cout << "ERROR";
    }
    
}