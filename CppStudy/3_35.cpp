#include <iostream>

using namespace std;

int main() {
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    cout << "ԭʼ����Ԫ�ص�ֵΪ��" << endl;
    for (int i = 0; i < 10; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // ʹ��ָ�뽫����Ԫ����0
    int* ptr = arr;
    for (int i = 0; i < 10; i++) {
        *ptr = 0;
        ptr++;
    }

    cout << "�޸ĺ�����Ԫ�ص�ֵΪ��" << endl;
    for (int i = 0; i < 10; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}
