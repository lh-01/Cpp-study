#include <iostream>

using namespace std;

int main() {
    int score;
    cout << "������ɼ�: ";
    cin >> score;

    string category;
    if(score >= 90) {
        category = "high pass";
    } else if(score >= 60 && score <= 75) {
        category = "low pass";
    } else if(score < 60) {
        category = "fail";
    } else {
        category = "pass";
    }
    
    cout << "�ɼ����: " << category << endl;

    return 0;
}
