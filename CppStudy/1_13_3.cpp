#include <iostream>
using namespace std;

int main() {
    int start, end;
    
    cout << "������ʼ����: ";
    cin >> start;

    cout << "������ֹ����: ";
    cin >> end;
    
    cout << "����������ָ���ķ�Χ�ڵ���������Ϊ: ";
    
    for (int i = start; i <= end; i++) {
        cout << i << " ";
    }
    
    cout << endl;
    
    return 0;
}
