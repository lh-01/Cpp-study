#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> arr1(10);
    vector<int> arr2(10);

    // ������Ԫ�ظ�ֵ
    for (int i = 0; i < 10; i++) {
        arr1[i] = i;
    }

    // ��arr1������arr2
    arr2 = arr1;

    // �������Ԫ��ֵ
    cout << "����1Ԫ�ص�ֵΪ��" << endl;
    for (int i = 0; i < 10; i++) {
        cout << arr1[i] << " ";
    }
    cout << endl;

    cout << "����2Ԫ�ص�ֵΪ��" << endl;
    for (int i = 0; i < 10; i++) {
        cout << arr2[i] << " ";
    }
    cout << endl;

    return 0;
}
