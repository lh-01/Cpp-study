#include <iostream>
#include <stdexcept>
using namespace std;

int main() {
    int num1, num2;
    cout << "请输入两个整数: ";
    cin >> num1 >> num2;

    try {
        if (num2 == 0) {
            throw runtime_error("除数不能为零！");
        }

        double result = static_cast<double>(num1) / num2;
        cout << "除法结果: " << result << endl;
    } catch (const exception& e) {
        cout << "捕获异常: " << e.what() << endl;
    }

    return 0;
}
