#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> numbers;
    
    cout << "�������������Կո�ָ��������������հ��ַ�ֹͣ��" << endl;

    int num;
    while(cin >> num)
    {
        numbers.push_back(num);
    }
    
    cout << "��������֮�ͣ�" << endl;
    for(size_t i = 0; i < numbers.size() - 1; ++i)
    {
        int sum = numbers[i] + numbers[i+1];
        cout << sum << endl;
    }
    
    return 0;
}
