#include <iostream>
using namespace std;

int main() {
    int num1, num2;
    cout << "请输入两个整数: ";
    cin >> num1 >> num2;

    if (num2 != 0) {
        double result = static_cast<double>(num1) / num2;
        cout << "除法结果: " << result << endl;
    } else {
        cout << "错误：除数不能为零！" << endl;
    }

    return 0;
}
