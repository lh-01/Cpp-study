#include <iostream>
using namespace std;

int calculateFactorial(int num) {
    int factorial = 1;
    for (int i = 2; i <= num; i++) {
        factorial *= i;
    }
    return factorial;
}

int main() {
    int number;
    cout << "������һ������: ";
    cin >> number;

    int result = calculateFactorial(number);
    cout << number << " �Ľ׳���: " << result << endl;

    return 0;
}
