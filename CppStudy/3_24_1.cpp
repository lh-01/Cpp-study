#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> numbers;

    // 输入一组整数，存入vector对象
    cout << "请输入一组整数，以-1作为结束符：" << endl;
    int num;
    while (cin >> num && num != -1)
    {
        numbers.push_back(num);
    }

    // 使用迭代器计算相邻整数的和并输出
    cout << "相邻整数的和：" << endl;
    for (vector<int>::iterator it = numbers.begin(); it != numbers.end() - 1; ++it)
    {
        int sum = *it + *(it + 1);
        cout << sum << " ";
    }
    cout << endl;

    return 0;
}
