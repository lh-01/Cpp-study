#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> vec = {1, 2, 3, 4, 5};

    int arr[vec.size()];

    // �� vector ��Ԫ�ؿ�����������
    for (int i = 0; i < vec.size(); ++i) {
        arr[i] = vec[i];
    }

    // ��������Ԫ��
    for (int i = 0; i < vec.size(); ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}
