#include <iostream>
#include <string>
using namespace std;

int main() {
    string str1, str2;
    
    cout << "�����һ���ַ���: ";
    cin >> str1;
    
    cout << "����ڶ����ַ���: ";
    cin >> str2;
    
    if (str1 == str2) {
        cout << "�����ַ������" << endl;
    } else {
        string maxStr = (str1 > str2) ? str1 : str2;
        cout << "�ϴ���ַ���Ϊ: " << maxStr << endl;
    }
    
    return 0;
}
