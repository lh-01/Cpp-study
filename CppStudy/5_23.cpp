#include <iostream>
using namespace std;

int main() {
    int num1, num2;
    cout << "��������������: ";
    cin >> num1 >> num2;

    if (num2 != 0) {
        double result = static_cast<double>(num1) / num2;
        cout << "�������: " << result << endl;
    } else {
        cout << "���󣺳�������Ϊ�㣡" << endl;
    }

    return 0;
}
