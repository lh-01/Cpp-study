#include <iostream>
#include <vector>

using namespace std;

void recursivePrint(const vector<int>& vec, size_t index) {
    if (index >= vec.size()) {
        return;
    }

    cout << vec[index] << " ";
    recursivePrint(vec, index + 1);
}

int main() {
    vector<int> numbers = {1, 2, 3, 4, 5};

    recursivePrint(numbers, 0);

    return 0;
}
