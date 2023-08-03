#include <iostream>
using namespace std;

int main() {
    int sum = 0;
    int num = 50;
    
    while (num <= 100) {
        sum += num;
        num++;
    }
    
    cout << "50到100的和为: " << sum << endl;
    
    return 0;
}
