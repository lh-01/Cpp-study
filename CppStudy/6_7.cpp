#include <iostream>
using namespace std;

int incrementValue() {
    static int count = 0;
    return count++;
}

int main() {
    for (int i = 0; i < 5; i++) {
        int result = incrementValue();
        cout << "调用结果: " << result << endl;
    }

    return 0;
}
