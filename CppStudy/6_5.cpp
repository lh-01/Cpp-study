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
    cout << "������һ����: ";
    cin >> number;

    double result = absoluteValue(number);
    cout << "����ֵ: " << result << endl;

    return 0;
}
