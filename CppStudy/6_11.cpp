#include <iostream>
using namespace std;

void reset(int& ref) {
    ref = 0;
}

int main() {
    int number = 10;

    cout << "����ǰ��ֵ: " << number << endl;
    reset(number);
    cout << "���ú��ֵ: " << number << endl;

    return 0;
}
