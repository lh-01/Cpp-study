#include <iostream>
using namespace std;

void reset(int& ref) {
    ref = 0;
}

int main() {
    int number = 10;

    cout << "重置前的值: " << number << endl;
    reset(number);
    cout << "重置后的值: " << number << endl;

    return 0;
}
