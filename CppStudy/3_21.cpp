#include <iostream>
#include <vector>
#include <string>

using namespace std;

template <typename T>
void printVectorInfo(const vector<T>& vec)
{
    cout << "容量：" << vec.capacity() << endl;
    cout << "具体内容：" << endl;
    for (typename vector<T>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        cout << *it << " ";
    }
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
    
    cout << "v1:" << endl;
    printVectorInfo(v1);
    
    cout << "v2:" << endl;
    printVectorInfo(v2);
    
    cout << "v3:" << endl;
    printVectorInfo(v3);
    
    cout << "v4:" << endl;
    printVectorInfo(v4);
    
    cout << "v5:" << endl;
    printVectorInfo(v5);
    
    cout << "v6:" << endl;
    printVectorInfo(v6);
    
    cout << "v7:" << endl;
    printVectorInfo(v7);
    
    return 0;
}
