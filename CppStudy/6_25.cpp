#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    string result;

    if (argc == 3) {
        result = argv[1];
        result += " ";
        result += argv[2];

        cout << "�ϲ�����ַ���: " << result << endl;
    } else {
        cout << "����������������" << endl;
    }

    return 0;
}
