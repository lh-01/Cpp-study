#include <iostream>
using namespace std;

int main() {
    int start, end;
    
    cout << "输入起始数字: ";
    cin >> start;

    cout << "输入终止数字: ";
    cin >> end;
    
    cout << "两个整数所指定的范围内的所有整数为: ";
    
    int i = start;
    while (i <= end) {
        cout << i << " ";
        i++;
    }
    
    cout << endl;
    
    return 0;
}
