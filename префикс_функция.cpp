#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <set>
using namespace std;

vector<int> prefix_function (string s){
	vector<int> res (s.size());
	for (int i = 1; i < s.size(); i++) {
		int j = res[i - 1];
		while (j > 0 and s[i] != s[j]){
            j = res[j - 1];
        }
		if (s[i] == s[j]){
            j++;
        }
		res[i] = j;
	}
	return res;
}

int main(){
    string str;
    cin >> str;
    auto res = prefix_function(str);
    for (auto el : res){
        cout << el << " ";
    }
    return 0;
}