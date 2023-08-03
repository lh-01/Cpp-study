#include <iostream>
#include <string>
using namespace std;

int main() {
    string str1, str2;
    
    cout << "输入第一个字符串: ";
    cin >> str1;
    
    cout << "输入第二个字符串: ";
    cin >> str2;
    
    if (str1 == str2) {
        cout << "两个字符串相等" << endl;
    } else {
        string maxStr = (str1 > str2) ? str1 : str2;
        cout << "较大的字符串为: " << maxStr << endl;
    }
    
    return 0;
}
