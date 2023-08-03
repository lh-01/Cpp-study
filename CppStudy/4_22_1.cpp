#include <iostream>

using namespace std;

int main() {
    int score;
    cout << "请输入成绩: ";
    cin >> score;

    string category = (score >= 90) ? "high pass"
                    : (score >= 60 && score <= 75) ? "low pass"
                    : (score < 60) ? "fail"
                    : "pass";

    cout << "成绩类别: " << category << endl;
    
    return 0;
}
