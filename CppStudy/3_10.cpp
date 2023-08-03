#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

bool isPunctuation(char c) {
    return ispunct(static_cast<unsigned char>(c));
}

string removePunctuation(const string& str) {
    string result = str;
    result.erase(remove_if(result.begin(), result.end(), isPunctuation), result.end());
    return result;
}

int main() {
    string input;
    cout << "请输入一个包含标点符号的字符串: ";
    getline(cin, input);
    
    string output = removePunctuation(input);
    
    cout << "去除标点符号后的字符串: " << output << endl;

    return 0;
}
