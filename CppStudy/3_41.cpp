#include <iostream>
#include <vector>

using namespace std;

int main() {
    int arr[] = {1, 2, 3, 4, 5};

    vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));

    // ��� vector �е�Ԫ��
    for (int i = 0; i < vec.size(); ++i) {
        cout << vec[i] << " ";
    }
    cout << endl;

    return 0;
}
