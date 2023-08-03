#include <iostream>
#include <sstream>
#include <map>
using namespace std;

int main() {
    map<string, int> wordCount;
    string line;
    int maxCount = 0;
    string maxWord;

    cout << "������һЩ���ʣ��Կո�ָ�����" << endl;

    getline(cin, line);
    istringstream iss(line);
    string word;
    string prevWord;

    while (iss >> word) {
        if (word == prevWord) {
            wordCount[word]++;
            if (wordCount[word] > maxCount) {
                maxCount = wordCount[word];
                maxWord = word;
            }
        } else {
            wordCount[word] = 1;
        }
        prevWord = word;
    }

    if (maxCount > 1) {
        cout << "�ظ����ֵ���������" << maxCount << "����Ӧ�ĵ����ǣ�" << maxWord << endl;
    } else {
        cout << "�κε��ʶ�û���������ֹ���" << endl;
    }

    return 0;
}
