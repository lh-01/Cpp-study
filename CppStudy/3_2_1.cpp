#include <iostream>
#include <string>
using namespace std;

int main() {
    string line;
    cout << "请输入一整行: ";
    
    getline(cin, line);
    
    cout << "输入的整行内容为: " << line << endl;
    
    return 0;
}
