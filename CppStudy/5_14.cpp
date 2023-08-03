#include <iostream>
#include <sstream>
#include <map>
using namespace std;

int main() {
    map<string, int> wordCount;
    string line;
    int maxCount = 0;
    string maxWord;

    cout << "请输入一些单词（以空格分隔）：" << endl;

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
        cout << "重复出现的最大次数：" << maxCount << "，对应的单词是：" << maxWord << endl;
    } else {
        cout << "任何单词都没有连续出现过。" << endl;
    }

    return 0;
}
