#include <iostream>
using namespace std;

int main() {
    int sequenceCount = 0;
    string text;

    cout << "请输入文本: ";
    getline(cin, text);

    for (unsigned int i = 0; i < text.length() - 1; i++) {
        string sequence = text.substr(i, 2);
        if (sequence == "ff" || sequence == "fl" || sequence == "fi") {
            sequenceCount++;
        }
    }

    cout << "含有两个字符的字符序列的数量: " << sequenceCount << endl;

    return 0;
}
