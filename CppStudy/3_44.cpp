#include <iostream>

using namespace std;

int main() {
    using RowPtr = int(*)[4];
    using IntPtr = int*;

    int ia[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    for (RowPtr row = ia; row != ia + 3; ++row) {
        for (IntPtr col = *row; col != *row + 4; ++col) {
            cout << *col << " ";
        }
    }

    return 0;
}
