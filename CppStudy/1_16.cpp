#include <iostream>
using namespace std;

int main() {
    int num;
    int sum = 0;
    
    cout << "����һ���� (����-1ֹͣ): " << endl;
    
    while (true) {
        cin >> num;
        
        if (num == -1) {
            break;
        }
        
        sum += num;
    }
    
    cout << "��һ�����ĺ�Ϊ: " << sum << endl;
    
    return 0;
}
