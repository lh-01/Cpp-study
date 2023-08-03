#include <iostream>
#include <vector>
#include <string>

using namespace std;

template<typename T>
void printVectorInfo(const vector<T>& vec)
{
    cout << "向量容量: " << vec.capacity() << endl;
    cout << "向量内容: ";
    for(const auto& element : vec)
    {
        cout << element << " ";
    }
    cout << endl;
}

int main()
{
    vector<int> v1;
    vector<int> v2(10);
    vector<int> v3(10, 42);
    vector<int> v4(10);
    vector<int> v5(10, 42);
    vector<int> v6(10);
    vector<string> v7(10, "hi");
    
    printVectorInfo(v1);
    printVectorInfo(v2);
    printVectorInfo(v3);
    printVectorInfo(v4);
    printVectorInfo(v5);
    printVectorInfo(v6);
    printVectorInfo(v7);
    
    return 0;
}
