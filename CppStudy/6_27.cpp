#include <iostream>
#include <initializer_list>

using namespace std;

int sumInitializerList(const initializer_list<int>& list) {
    int sum = 0;

    for (auto num : list) {
        sum += num;
    }

    return sum;
}

int main() {
    initializer_list<int> numbers = {1, 2, 3, 4, 5};
    
    int result = sumInitializerList(numbers);
    cout << "列表中所有元素之和: " << result << endl;

    return 0;
}
