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
    
    cout << "每对相应位置的元素之和：" << endl;
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
