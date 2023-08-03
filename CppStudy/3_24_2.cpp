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

    // ʹ�õ�������������Ԫ�صĺͲ����
    cout << "��Ӧλ��Ԫ�صĺͣ�" << endl;
    vector<int>::iterator front = numbers.begin();
    vector<int>::iterator back = numbers.end() - 1;

    while (front <= back)
    {
        int sum = *front + *back;
        cout << sum << " ";

        // ����ʼ�ͽ���λ���غϵ����
        if (front == back)
        {
            break;
        }

        ++front;
        --back;
    }

    cout << endl;

    return 0;
}
