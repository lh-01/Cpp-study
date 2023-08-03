#include <iostream>
#include <string>
using namespace std;

int main() {
    string prevWord, currWord;
    bool foundDuplicate = false;

    while (getline(cin, currWord) && !currWord.empty()) {
        if (prevWord == currWord) {
            foundDuplicate = true;
            cout << "连续重复的单词为：" << currWord << endl;
            break;
        }

        prevWord = currWord;
    }

    if (!foundDuplicate) {
        cout << "没有连续重复出现的单词" << endl;
    }

    return 0;
}
