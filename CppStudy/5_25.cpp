#include <iostream>
#include <stdexcept>
using namespace std;

int main() {
    int num1, num2;
    cout << "请输入两个整数: ";
    cin >> num1 >> num2;

    try {
        while (num2 == 0) {
            cout << "除数不能为零，请重新输入第二个数: ";
            cin >> num2;
        }

        double result = static_cast<double>(num1) / num2;
        cout << "除法结果: " << result << endl;
    } catch (const exception& e) {
        cout << "捕获异常: " << e.what() << endl;
        cout << "是否要输入新的数? (y/n): ";
        char choice;
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            main();  // 重新执行 try 语句块
        }
    }

    return 0;
}
