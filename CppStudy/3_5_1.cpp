#include <iostream>
#include <string>
using namespace std;

int main() {
    string input;
    string result;

    cout << "请输入多个字符串（以空格分隔），以空行结束输入: " << endl;

    while (getline(cin, input) && !input.empty()) {
        result += input;
    }

    cout << "连接后的大字符串为: " << result << endl;

    return 0;
}
