#include <iostream>
#include <stdexcept>
using namespace std;

int main() {
    int num1, num2;
    cout << "��������������: ";
    cin >> num1 >> num2;

    try {
        if (num2 == 0) {
            throw runtime_error("��������Ϊ�㣡");
        }

        double result = static_cast<double>(num1) / num2;
        cout << "�������: " << result << endl;
    } catch (const exception& e) {
        cout << "�����쳣: " << e.what() << endl;
    }

    return 0;
}
