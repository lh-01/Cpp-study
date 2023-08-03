#include <iostream>
#include <cstring>

using namespace std;

int main() {
    const char* str1 = "Hello";
    const char* str2 = "World";
    char str3[20]; // 定义第三个字符数组，大小足够容纳两个数组的连接结果

    strcpy(str3, str1); // 将 str1 的内容拷贝到 str3
    strcat(str3, str2); // 将 str2 的内容拷贝到 str3 的末尾

    cout << "连接结果： " << str3 << endl;

    return 0;
}
