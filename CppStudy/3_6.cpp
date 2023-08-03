#include <iostream>
#include <string>
using namespace std;

int main() {
    string str;
    
    cout << "ÇëÊäÈëÒ»¸ö×Ö·û´®£º";
    getline(cin, str);

    for (int i = 0; i < str.length(); i++) {
        str[i] = 'X';
    }

    cout << "Ìæ»»ºóµÄ×Ö·û´®Îª£º" << str << endl;

    return 0;
}
