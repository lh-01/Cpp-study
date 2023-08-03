#include <iostream>
#include <vector>
using namespace std;

bool isPrefix(vector<int>& vec1, vector<int>& vec2) {
    size_t size1 = vec1.size();
    size_t size2 = vec2.size();
    size_t minSize = min(size1, size2);

    for (size_t i = 0; i < minSize; ++i) {
        if (vec1[i] != vec2[i]) {
            return false;
        }
    }

    return true;
}

int main() {
    cout << "请输入第一个向量的整数个数：";
    int size1;
    cin >> size1;

    vector<int> vec1(size1);
    cout << "请输入第一个向量的整数：";
    for (int i = 0; i < size1; ++i) {
        cin >> vec1[i];
    }

    cout << "请输入第二个向量的整数个数：";
    int size2;
    cin >> size2;

    vector<int> vec2(size2);
    cout << "请输入第二个向量的整数：";
    for (int i = 0; i < size2; ++i) {
        cin >> vec2[i];
    }

    if (isPrefix(vec1, vec2)) {
        cout << "第一个向量是第二个向量的前缀" << endl;
    } else if (isPrefix(vec2, vec1)) {
        cout << "第二个向量是第一个向量的前缀" << endl;
    } else {
        cout << "两个向量没有前缀关系" << endl;
    }

    return 0;
}
