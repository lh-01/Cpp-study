#include <iostream>
#include <string>
using namespace std;

int main() {
    string str1, str2;

    do {
        cout << "�������һ���ַ�����";
        getline(cin, str1);

        cout << "������ڶ����ַ�����";
        getline(cin, str2);

        // �Ƚ��ַ������Ȳ�����϶̵��ַ���
        if (str1.length() < str2.length()) {
            cout << "�϶̵��ַ����ǣ�" << str1 << endl;
        } else if (str2.length() < str1.length()) {
            cout << "�϶̵��ַ����ǣ�" << str2 << endl;
        } else {
            cout << "�����ַ����ĳ�����ͬ" << endl;
        }

        // ѯ���Ƿ����ִ��
        cout << "�Ƿ����ִ�У�(Y/N): ";
        char choice;
        cin >> choice;

        // ������뻺����
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice != 'Y' && choice != 'y') {
            break;
        }

        cout << endl;
    } while (true);

    return 0;
}
