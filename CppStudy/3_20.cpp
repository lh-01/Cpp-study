#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> numbers;
    
    cout << "请输入整数，以空格分隔，输入非整数或空白字符停止：" << endl;

    int num;
    while(cin >> num)
    {
        numbers.push_back(num);
    }
    
    cout << "相邻整数之和：" << endl;
    for(size_t i = 0; i < numbers.size() - 1; ++i)
    {
        int sum = numbers[i] + numbers[i+1];
        cout << sum << endl;
    }
    
    return 0;
}
