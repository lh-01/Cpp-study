#include <iostream>

using namespace std;

int main() {
    int ia[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    // 版本2：使用普通的 for 语句和下标运算符
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            cout << ia[i][j] << " ";
        }
    }

    return 0;
}
