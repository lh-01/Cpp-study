#include <iostream>

using namespace std;

int main() {
    int score;
    cout << "������ɼ�: ";
    cin >> score;

    char grade = (score >= 90) ? 'A' : (score >= 80) ? 'B' : (score >= 70) ? 'C' : (score >= 60) ? 'D' : 'F';
    
    cout << "��ĸ�ɼ�: " << grade << endl;

    return 0;
}
