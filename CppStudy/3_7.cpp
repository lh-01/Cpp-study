#include <iostream>
#include <string>
using namespace std;

int main() {
    string str;
    
    cout << "ÇëÊäÈëÒ»¸ö×Ö·û´®£º";
    getline(cin, str);

    for (char& c : str) {
        c = 'X';
    }

    cout << "Ìæ»»ºóµÄ×Ö·û´®Îª£º" << str << endl;

    return 0;
}
