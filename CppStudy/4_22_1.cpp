#include <iostream>

using namespace std;

int main() {
    int score;
    cout << "������ɼ�: ";
    cin >> score;

    string category = (score >= 90) ? "high pass"
                    : (score >= 60 && score <= 75) ? "low pass"
                    : (score < 60) ? "fail"
                    : "pass";

    cout << "�ɼ����: " << category << endl;
    
    return 0;
}
