#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> numbers;
    int num;
    
    cout << "������һ������: ";
    
    while (cin >> num) {
        numbers.push_back(num);
    }
    
    cout << "������������£�" << endl;
    for (vector<int>::iterator it = numbers.begin(); it != numbers.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    
    return 0;
}
