#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> numbers;

    // ����10������������vector����
    cout << "������10��������" << endl;
    for (int i = 0; i < 10; ++i)
    {
        int num;
        cin >> num;
        numbers.push_back(num);
    }

    // ʹ�õ�����������Ԫ�ص�ֵ���ԭ��������
    for (vector<int>::iterator it = numbers.begin(); it != numbers.end(); ++it)
    {
        *it = *it * 2;
    }

    // ����任���ֵ��ԭ����ֵ
    cout << "�任���ֵ��" << endl;
    for (vector<int>::iterator it = numbers.begin(); it != numbers.end(); ++it)
    {
        cout << *it << " ";
    }
    cout << endl;

    cout << "ԭ����ֵ��" << endl;
    for (vector<int>::iterator it = numbers.begin(); it != numbers.end(); ++it)
    {
        cout << *it / 2 << " ";
    }
    cout << endl;

    return 0;
}
