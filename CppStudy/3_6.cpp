#include <iostream>
#include <string>
using namespace std;

int main() {
    string str;
    
    cout << "������һ���ַ�����";
    getline(cin, str);

    for (int i = 0; i < str.length(); i++) {
        str[i] = 'X';
    }

    cout << "�滻����ַ���Ϊ��" << str << endl;

    return 0;
}
