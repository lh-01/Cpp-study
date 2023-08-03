#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> numbers;
    int num;
    
    cout << "请输入一组整数: ";
    
    while (cin >> num) {
        numbers.push_back(num);
    }
    
    cout << "输入的整数如下：" << endl;
    for (vector<int>::iterator it = numbers.begin(); it != numbers.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    
    return 0;
}
