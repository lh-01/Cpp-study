#include <iostream>
#include <string>
#include <cctype>
using namespace std;

int main() {
    string prevWord, currWord;
    bool foundDuplicate = false;

    while (getline(cin, currWord) && !currWord.empty()) {
        // 检查当前单词是否以大写字母开头
        if (prevWord == currWord && isupper(currWord[0])) {
            foundDuplicate = true;
            cout << "连续重复的以大写字母开头的单词为：" << currWord << endl;
            break;
        }

        prevWord = currWord;
    }

    if (!foundDuplicate) {
        cout << "没有连续重复出现的以大写字母开头的单词" << endl;
    }

    return 0;
}
