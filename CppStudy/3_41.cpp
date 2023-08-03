#include <iostream>
#include <vector>

using namespace std;

int main() {
    int arr[] = {1, 2, 3, 4, 5};

    vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));

    // 输出 vector 中的元素
    for (int i = 0; i < vec.size(); ++i) {
        cout << vec[i] << " ";
    }
    cout << endl;

    return 0;
}
