#include <iostream>
using namespace std;

int main() {
    int vowelCount = 0;
    int spaceCount = 0;
    int tabCount = 0;
    int newlineCount = 0;
    string text;
    
    cout << "�������ı�: ";
    getline(cin, text);

    for (unsigned int i = 0; i < text.length(); i++) {
        char c = text[i];
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            vowelCount++;
        } else if (c == ' ') {
            spaceCount++;
        } else if (c == '\t') {
            tabCount++;
        } else if (c == '\n') {
            newlineCount++;
        }
    }

    cout << "Ԫ����ĸ����: " << vowelCount << endl;
    cout << "�ո�����: " << spaceCount << endl;
    cout << "�Ʊ������: " << tabCount << endl;
    cout << "���з�����: " << newlineCount << endl;

    return 0;
}
