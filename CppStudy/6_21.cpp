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

    cout << "�������һ������: ";
    cin >> num1;
    cout << "������ڶ�������: ";
    cin >> num2;

    ptr1 = &num1;
    ptr2 = &num2;

    cout << "����ǰ��" << endl;
    cout << "ptr1 = " << *ptr1 << ", ptr2 = " << *ptr2 << endl;

    swapIntPointers(ptr1, ptr2);

    cout << "������" << endl;
    cout << "ptr1 = " << *ptr1 << ", ptr2 = " << *ptr2 << endl;

    return 0;
}
