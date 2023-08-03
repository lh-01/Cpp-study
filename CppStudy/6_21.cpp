#include <iostream>

using namespace std;

void swapIntPointers(int*& ptr1, int*& ptr2) {
    int* temp = ptr1;
    ptr1 = ptr2;
    ptr2 = temp;
}

int main() {
    int num1, num2;
    int* ptr1;
    int* ptr2;

    cout << "请输入第一个整数: ";
    cin >> num1;
    cout << "请输入第二个整数: ";
    cin >> num2;

    ptr1 = &num1;
    ptr2 = &num2;

    cout << "交换前：" << endl;
    cout << "ptr1 = " << *ptr1 << ", ptr2 = " << *ptr2 << endl;

    swapIntPointers(ptr1, ptr2);

    cout << "交换后：" << endl;
    cout << "ptr1 = " << *ptr1 << ", ptr2 = " << *ptr2 << endl;

    return 0;
}
