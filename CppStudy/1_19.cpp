#include <iostream>
using namespace std;

int main() {
    int start, end;
    
    cout << "������ʼ����: ";
    cin >> start;

    cout << "������ֹ����: ";
    cin >> end;
    
    cout << "����������ָ���ķ�Χ�ڵ���������Ϊ: ";
    
    if (start <= end) {
        int i = start;
        while (i <= end) {
            cout << i << " ";
            i++;
        }
    }
    else {
        int i = start;
        while (i >= end) {
            cout << i << " ";
            i--;
        }
    }
    
    cout << endl;
    
    return 0;
}
