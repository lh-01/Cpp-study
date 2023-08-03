#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    vector<string> strings;
    string str;
    
    cout << "请输入一组字符串: ";
    
    while (getline(cin, str)) {
        if (str.empty())
            break;
        strings.push_back(str);
    }
    
    cout << "输入的字符串如下：" << endl;
    for (vector<string>::iterator it = strings.begin(); it != strings.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    
    return 0;
}
