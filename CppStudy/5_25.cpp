#include <iostream>
#include <stdexcept>
using namespace std;

int main() {
    int num1, num2;
    cout << "��������������: ";
    cin >> num1 >> num2;

    try {
        while (num2 == 0) {
            cout << "��������Ϊ�㣬����������ڶ�����: ";
            cin >> num2;
        }

        double result = static_cast<double>(num1) / num2;
        cout << "�������: " << result << endl;
    } catch (const exception& e) {
        cout << "�����쳣: " << e.what() << endl;
        cout << "�Ƿ�Ҫ�����µ���? (y/n): ";
        char choice;
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            main();  // ����ִ�� try ����
        }
    }

    return 0;
}
