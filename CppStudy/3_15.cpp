#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    vector<string> strings;
    string str;
    
    cout << "������һ���ַ���: ";
    
    while (getline(cin, str)) {
        if (str.empty())
            break;
        strings.push_back(str);
    }
    
    cout << "������ַ������£�" << endl;
    for (vector<string>::iterator it = strings.begin(); it != strings.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    
    return 0;
}
