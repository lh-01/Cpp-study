#include <iostream>

using namespace std;

int main() {
    int ia[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    for (auto row = begin(ia); row != end(ia); ++row) {
        for (auto col = begin(*row); col != end(*row); ++col) {
            cout << *col << " ";
        }
    }

    return 0;
}
