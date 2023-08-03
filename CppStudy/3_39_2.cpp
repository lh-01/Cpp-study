#include <iostream>
#include <cstring>

using namespace std;

int main() {
    const char* str1 = "Hello";
    const char* str2 = "World";

    int result = strcmp(str1, str2);

    if (result == 0) {
        cout << "str1 和 str2 相等" << endl;
    } else {
        cout << "str1 和 str2 不相等" << endl;
    }

    return 0;
}
