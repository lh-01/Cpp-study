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
    
    cout << "ÿ����Ӧλ�õ�Ԫ��֮�ͣ�" << endl;
    size_t start = 0;
    size_t end = numbers.size() - 1;
    while(start <= end)
    {
        int sum = numbers[start] + numbers[end];
        cout << sum << endl;
        start++;
        end--;
    }
    
    return 0;
}
