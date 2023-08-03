#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> numbers;

    // 输入10个整数，存入vector对象
    cout << "请输入10个整数：" << endl;
    for (int i = 0; i < 10; ++i)
    {
        int num;
        cin >> num;
        numbers.push_back(num);
    }

    // 使用迭代器将所有元素的值变成原来的两倍
    for (vector<int>::iterator it = numbers.begin(); it != numbers.end(); ++it)
    {
        *it = *it * 2;
    }

    // 输出变换后的值和原来的值
    cout << "变换后的值：" << endl;
    for (vector<int>::iterator it = numbers.begin(); it != numbers.end(); ++it)
    {
        cout << *it << " ";
    }
    cout << endl;

    cout << "原来的值：" << endl;
    for (vector<int>::iterator it = numbers.begin(); it != numbers.end(); ++it)
    {
        cout << *it / 2 << " ";
    }
    cout << endl;

    return 0;
}
