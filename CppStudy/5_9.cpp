#include <iostream>
using namespace std;

int main() {
    int count = 0;
    string text;

    cout << "�������ı�: ";
    getline(cin, text);

    for (unsigned int i = 0; i < text.length(); i++) {
        char c = text[i];
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            count++;
        }
    }

    cout << "Ԫ����ĸ����: " << count << endl;

    return 0;
}
