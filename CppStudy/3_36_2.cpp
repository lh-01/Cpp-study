#include <iostream>
#include <vector>

using namespace std;

bool arraysAreEqual(const vector<int>& arr1, const vector<int>& arr2) {
    if (arr1.size() != arr2.size()) {
        return false;
    }

    for (size_t i = 0; i < arr1.size(); i++) {
        if (arr1[i] != arr2[i]) {
            return false;
        }
    }

    return true;
}

int main() {
    vector<int> arr1 = {1, 2, 3, 4, 5};
    vector<int> arr2 = {1, 2, 3, 4, 5};
    vector<int> arr3 = {1, 2, 3, 4, 6};

    if (arraysAreEqual(arr1, arr2)) {
        cout << "arr1 和 arr2 相等" << endl;
    } else {
        cout << "arr1 和 arr2 不相等" << endl;
    }

    if (arraysAreEqual(arr1, arr3)) {
        cout << "arr1 和 arr3 相等" << endl;
    } else {
        cout << "arr1 和 arr3 不相等" << endl;
    }

    return 0;
}
