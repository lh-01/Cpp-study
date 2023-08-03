#include <iostream>

using namespace std;

int main() {
    int arr[10];

    // 给数组元素赋值
    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }

    // 输出数组元素值
    cout << "数组元素的值为：" << endl;
    for (int i = 0; i < 10; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}
