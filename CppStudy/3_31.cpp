#include <iostream>

using namespace std;

int main() {
    int arr[10];

    // ������Ԫ�ظ�ֵ
    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }

    // �������Ԫ��ֵ
    cout << "����Ԫ�ص�ֵΪ��" << endl;
    for (int i = 0; i < 10; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}
