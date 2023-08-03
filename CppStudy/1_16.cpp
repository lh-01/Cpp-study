#include <iostream>
using namespace std;

int main() {
    int num;
    int sum = 0;
    
    cout << "输入一组数 (输入-1停止): " << endl;
    
    while (true) {
        cin >> num;
        
        if (num == -1) {
            break;
        }
        
        sum += num;
    }
    
    cout << "这一组数的和为: " << sum << endl;
    
    return 0;
}
