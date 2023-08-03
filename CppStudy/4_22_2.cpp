#include <iostream>

using namespace std;

int main() {
    int score;
    cout << "请输入成绩: ";
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
    
    cout << "成绩类别: " << category << endl;

    return 0;
}
