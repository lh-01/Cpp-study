#include <iostream>
using namespace std;

int main() {
    int sequenceCount = 0;
    string text;

    cout << "�������ı�: ";
    getline(cin, text);

    for (unsigned int i = 0; i < text.length() - 1; i++) {
        string sequence = text.substr(i, 2);
        if (sequence == "ff" || sequence == "fl" || sequence == "fi") {
            sequenceCount++;
        }
    }

    cout << "���������ַ����ַ����е�����: " << sequenceCount << endl;

    return 0;
}
