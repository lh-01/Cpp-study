#include <iostream>

using namespace std;

int main() {
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    cout << "原始数组元素的值为：" << endl;
    for (int i = 0; i < 10; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // 使用指针将数组元素置0
    int* ptr = arr;
    for (int i = 0; i < 10; i++) {
        *ptr = 0;
        ptr++;
    }

    cout << "修改后数组元素的值为：" << endl;
    for (int i = 0; i < 10; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}
