#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> arr1(10);
    vector<int> arr2(10);

    // 给数组元素赋值
    for (int i = 0; i < 10; i++) {
        arr1[i] = i;
    }

    // 将arr1拷贝给arr2
    arr2 = arr1;

    // 输出数组元素值
    cout << "数组1元素的值为：" << endl;
    for (int i = 0; i < 10; i++) {
        cout << arr1[i] << " ";
    }
    cout << endl;

    cout << "数组2元素的值为：" << endl;
    for (int i = 0; i < 10; i++) {
        cout << arr2[i] << " ";
    }
    cout << endl;

    return 0;
}
