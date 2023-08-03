#include <iostream>
#include <string>
#include <cctype>
using namespace std;

int main() {
    string prevWord, currWord;
    bool foundDuplicate = false;

    while (getline(cin, currWord) && !currWord.empty()) {
        // ��鵱ǰ�����Ƿ��Դ�д��ĸ��ͷ
        if (prevWord == currWord && isupper(currWord[0])) {
            foundDuplicate = true;
            cout << "�����ظ����Դ�д��ĸ��ͷ�ĵ���Ϊ��" << currWord << endl;
            break;
        }

        prevWord = currWord;
    }

    if (!foundDuplicate) {
        cout << "û�������ظ����ֵ��Դ�д��ĸ��ͷ�ĵ���" << endl;
    }

    return 0;
}
