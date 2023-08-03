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
    cout << "�������һ������������������";
    int size1;
    cin >> size1;

    vector<int> vec1(size1);
    cout << "�������һ��������������";
    for (int i = 0; i < size1; ++i) {
        cin >> vec1[i];
    }

    cout << "������ڶ�������������������";
    int size2;
    cin >> size2;

    vector<int> vec2(size2);
    cout << "������ڶ���������������";
    for (int i = 0; i < size2; ++i) {
        cin >> vec2[i];
    }

    if (isPrefix(vec1, vec2)) {
        cout << "��һ�������ǵڶ���������ǰ׺" << endl;
    } else if (isPrefix(vec2, vec1)) {
        cout << "�ڶ��������ǵ�һ��������ǰ׺" << endl;
    } else {
        cout << "��������û��ǰ׺��ϵ" << endl;
    }

    return 0;
}
