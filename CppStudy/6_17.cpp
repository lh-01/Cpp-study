#include <iostream>
#include <string>
#include <cctype>

using namespace std;

bool containsUppercase(const string& str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (isupper(str[i])) {
            return true;
        }
    }
    return false;
}

void toLowercase(string& str) {
    for (size_t i = 0; i < str.length(); i++) {
        str[i] = tolower(str[i]);
    }
}

int main() {
    string text;

    cout << "ÇëÊäÈëÒ»¸ö×Ö·û´®£º";
    getline(cin, text);

    // ÅÐ¶ÏÊÇ·ñ°üº¬´óÐ´×ÖÄ¸
    if (containsUppercase(text)) {
        cout << "×Ö·û´®°üº¬´óÐ´×ÖÄ¸" << endl;
    } else {
        cout << "×Ö·û´®²»°üº¬´óÐ´×ÖÄ¸" << endl;
    }

    // ×ª»»ÎªÐ¡Ð´ÐÎÊ½
    toLowercase(text);
    cout << "×ª»»ºóµÄ×Ö·û´®£º" << text << endl;

    return 0;
}
