#include <iostream>
#include <string>
using namespace std;

int main() {
    string str1, str2;

    cout << "�����һ���ַ���: ";
    cin >> str1;

    cout << "����ڶ����ַ���: ";
    cin >> str2;

    if (str1.length() == str2.length()) {
        cout << "�����ַ����������" << endl;
    } else {
        string longerStr = (str1.length() > str2.length()) ? str1 : str2;
        cout << "���Ƚϴ���ַ���Ϊ: " << longerStr << endl;
    }

    return 0;
}
