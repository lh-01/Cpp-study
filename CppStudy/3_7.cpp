#include <iostream>
#include <string>
using namespace std;

int main() {
    string str;
    
    cout << "������һ���ַ�����";
    getline(cin, str);

    for (char& c : str) {
        c = 'X';
    }

    cout << "�滻����ַ���Ϊ��" << str << endl;

    return 0;
}
