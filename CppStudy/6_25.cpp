#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    string result;

    if (argc == 3) {
        result = argv[1];
        result += " ";
        result += argv[2];

        cout << "合并后的字符串: " << result << endl;
    } else {
        cout << "请输入两个参数。" << endl;
    }

    return 0;
}
