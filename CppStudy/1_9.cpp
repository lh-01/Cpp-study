#include <iostream>
using namespace std;

int main() {
    int sum = 0;
    int num = 50;
    
    while (num <= 100) {
        sum += num;
        num++;
    }
    
    cout << "50��100�ĺ�Ϊ: " << sum << endl;
    
    return 0;
}
