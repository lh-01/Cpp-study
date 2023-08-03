#include <iostream>

using namespace std;

int main() {
    int ia[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    // ∞Ê±æ1£∫ π”√∑∂Œß for ”Ôæ‰
    for (const int(&row)[4] : ia) {
        for (int col : row) {
            cout << col << " ";
        }
    }

    return 0;
}
