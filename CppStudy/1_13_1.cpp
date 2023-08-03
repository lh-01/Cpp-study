#include <iostream>
using namespace std;

int main() {
    int sum = 0;
    
    for (int num = 50; num <= 100; num++) {
        sum += num;
    }
    
    cout << "50到100的和为: " << sum << endl;
    
    return 0;
}
