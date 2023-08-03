#include <iostream>
#include <string>
#include <cctype>

using namespace std;

bool containsUppercase(const string& str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (isupper(str[i])) {
            return true;
        }
    }
    return false;
}

void toLowercase(string& str) {
    for (size_t i = 0; i < str.length(); i++) {
        str[i] = tolower(str[i]);
    }
}

int main() {
    string text;

    cout << "������һ���ַ�����";
    getline(cin, text);

    // �ж��Ƿ������д��ĸ
    if (containsUppercase(text)) {
        cout << "�ַ���������д��ĸ" << endl;
    } else {
        cout << "�ַ�����������д��ĸ" << endl;
    }

    // ת��ΪСд��ʽ
    toLowercase(text);
    cout << "ת������ַ�����" << text << endl;

    return 0;
}
