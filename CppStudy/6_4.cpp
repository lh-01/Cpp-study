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
    cout << "请输入一个数字: ";
    cin >> number;

    int result = calculateFactorial(number);
    cout << number << " 的阶乘是: " << result << endl;

    return 0;
}
