#include <iostream>
#include <string>
using namespace std;

int main() {
    string str1, str2;

    do {
        cout << "请输入第一个字符串：";
        getline(cin, str1);

        cout << "请输入第二个字符串：";
        getline(cin, str2);

        // 比较字符串长度并输出较短的字符串
        if (str1.length() < str2.length()) {
            cout << "较短的字符串是：" << str1 << endl;
        } else if (str2.length() < str1.length()) {
            cout << "较短的字符串是：" << str2 << endl;
        } else {
            cout << "两个字符串的长度相同" << endl;
        }

        // 询问是否继续执行
        cout << "是否继续执行？(Y/N): ";
        char choice;
        cin >> choice;

        // 清空输入缓冲区
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice != 'Y' && choice != 'y') {
            break;
        }

        cout << endl;
    } while (true);

    return 0;
}
