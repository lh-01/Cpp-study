#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    
    for(auto& num : numbers) {
        num = num % 2 == 1 ? 2 * num : num;
    }
    
    cout << "����ֵ������Ľ����";
    for(const auto& num : numbers) {
        cout << num << " ";
    }
    
    return 0;
}
