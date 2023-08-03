#include <iostream>
#include <string>
using namespace std;

int main() {
    string prevWord, currWord;
    bool foundDuplicate = false;

    while (getline(cin, currWord) && !currWord.empty()) {
        if (prevWord == currWord) {
            foundDuplicate = true;
            cout << "�����ظ��ĵ���Ϊ��" << currWord << endl;
            break;
        }

        prevWord = currWord;
    }

    if (!foundDuplicate) {
        cout << "û�������ظ����ֵĵ���" << endl;
    }

    return 0;
}
