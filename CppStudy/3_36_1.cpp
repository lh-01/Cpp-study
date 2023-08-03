#include <iostream>

using namespace std;

bool arraysAreEqual(int arr1[], int arr2[], int size) {
    for (int i = 0; i < size; i++) {
        if (arr1[i] != arr2[i]) {
            return false;
        }
    }

    return true;
}

int main() {
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {1, 2, 3, 4, 5};
    int arr3[] = {1, 2, 3, 4, 6};
    int size = sizeof(arr1) / sizeof(arr1[0]);

    if (arraysAreEqual(arr1, arr2, size)) {
        cout << "arr1 �� arr2 ���" << endl;
    } else {
        cout << "arr1 �� arr2 �����" << endl;
    }

    if (arraysAreEqual(arr1, arr3, size)) {
        cout << "arr1 �� arr3 ���" << endl;
    } else {
        cout << "arr1 �� arr3 �����" << endl;
    }

    return 0;
}
