#include <iostream>
#include <string>
using namespace std;

int main() {
    string input;
    string result;

    cout << "���������ַ������Կո�ָ������Կ��н�������: " << endl;

    while (getline(cin, input) && !input.empty()) {
        result += input;
    }

    cout << "���Ӻ�Ĵ��ַ���Ϊ: " << result << endl;

    return 0;
}
