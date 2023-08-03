#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> vec = {1, 2, 3, 4, 5};

    int arr[vec.size()];

    // 将 vector 的元素拷贝到数组中
    for (int i = 0; i < vec.size(); ++i) {
        arr[i] = vec[i];
    }

    // 输出数组的元素
    for (int i = 0; i < vec.size(); ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}
