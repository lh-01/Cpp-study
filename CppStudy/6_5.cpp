#include <iostream>
using namespace std;

double absoluteValue(double num) {
    if (num < 0) {
        return -num;
    } else {
        return num;
    }
}

int main() {
    double number;
    cout << "请输入一个数: ";
    cin >> number;

    double result = absoluteValue(number);
    cout << "绝对值: " << result << endl;

    return 0;
}
