#include <iostream>
using namespace std;

void swapIntegers(int* num1, int* num2) {
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

int main() {
    int number1 = 10;
    int number2 = 20;

    cout << "����ǰ��ֵ��" << endl;
    cout << "number1 = " << number1 << endl;
    cout << "number2 = " << number2 << endl;

    swapIntegers(&number1, &number2);

    cout << "�������ֵ��" << endl;
    cout << "number1 = " << number1 << endl;
    cout << "number2 = " << number2 << endl;

    return 0;
}
