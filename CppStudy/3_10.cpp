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
    cout << "������һ�����������ŵ��ַ���: ";
    getline(cin, input);
    
    string output = removePunctuation(input);
    
    cout << "ȥ�������ź���ַ���: " << output << endl;

    return 0;
}
