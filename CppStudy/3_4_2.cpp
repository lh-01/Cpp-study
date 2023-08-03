#include <iostream>
#include <string>
using namespace std;

int main() {
    string str1, str2;

    cout << "输入第一个字符串: ";
    cin >> str1;

    cout << "输入第二个字符串: ";
    cin >> str2;

    if (str1.length() == str2.length()) {
        cout << "两个字符串长度相等" << endl;
    } else {
        string longerStr = (str1.length() > str2.length()) ? str1 : str2;
        cout << "长度较大的字符串为: " << longerStr << endl;
    }

    return 0;
}
