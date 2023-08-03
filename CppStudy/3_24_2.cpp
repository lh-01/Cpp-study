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

    // 使用迭代器计算相邻元素的和并输出
    cout << "对应位置元素的和：" << endl;
    vector<int>::iterator front = numbers.begin();
    vector<int>::iterator back = numbers.end() - 1;

    while (front <= back)
    {
        int sum = *front + *back;
        cout << sum << " ";

        // 处理开始和结束位置重合的情况
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
