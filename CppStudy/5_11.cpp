#include <iostream>
using namespace std;

int main() {
    int vowelCount = 0;
    int spaceCount = 0;
    int tabCount = 0;
    int newlineCount = 0;
    string text;
    
    cout << "请输入文本: ";
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

    cout << "元音字母数量: " << vowelCount << endl;
    cout << "空格数量: " << spaceCount << endl;
    cout << "制表符数量: " << tabCount << endl;
    cout << "换行符数量: " << newlineCount << endl;

    return 0;
}
