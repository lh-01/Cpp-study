#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> numbers;

    // ����һ������������vector����
    cout << "������һ����������-1��Ϊ��������" << endl;
    int num;
    while (cin >> num && num != -1)
    {
        numbers.push_back(num);
    }

    // ʹ�õ������������������ĺͲ����
    cout << "���������ĺͣ�" << endl;
    for (vector<int>::iterator it = numbers.begin(); it != numbers.end() - 1; ++it)
    {
        int sum = *it + *(it + 1);
        cout << sum << " ";
    }
    cout << endl;

    return 0;
}
