#include <iostream>
#include <cstring>

using namespace std;

int main() {
    const char* str1 = "Hello";
    const char* str2 = "World";
    char str3[20]; // ����������ַ����飬��С�㹻����������������ӽ��

    strcpy(str3, str1); // �� str1 �����ݿ����� str3
    strcat(str3, str2); // �� str2 �����ݿ����� str3 ��ĩβ

    cout << "���ӽ���� " << str3 << endl;

    return 0;
}
