#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> scores;
    vector<int> counts(11, 0);  // 11个分数段，初始值都为0

    // 输入一组成绩，存入vector对象
    cout << "请输入一组成绩，以-1作为结束符：" << endl;
    int score;
    while (cin >> score && score != -1)
    {
        // 将成绩存入scores
        scores.push_back(score);

        // 计算分数段
        int index = score / 10;
        counts[index]++;
    }

    // 输出各个分数段的成绩数量
    cout << "各个分数段的成绩数量：" << endl;
    for (int i = 0; i < 11; i++)
    {
        int lowerBound = i * 10;
        int upperBound = (i + 1) * 10 - 1;
        cout << lowerBound << "-" << upperBound << "分：" << counts[i] << "个成绩" << endl;
    }

    return 0;
}
