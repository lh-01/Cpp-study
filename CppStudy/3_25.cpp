#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> scores;
    vector<int> counts(11, 0);  // 11�������Σ���ʼֵ��Ϊ0

    // ����һ��ɼ�������vector����
    cout << "������һ��ɼ�����-1��Ϊ��������" << endl;
    int score;
    while (cin >> score && score != -1)
    {
        // ���ɼ�����scores
        scores.push_back(score);

        // ���������
        int index = score / 10;
        counts[index]++;
    }

    // ������������εĳɼ�����
    cout << "���������εĳɼ�������" << endl;
    for (int i = 0; i < 11; i++)
    {
        int lowerBound = i * 10;
        int upperBound = (i + 1) * 10 - 1;
        cout << lowerBound << "-" << upperBound << "�֣�" << counts[i] << "���ɼ�" << endl;
    }

    return 0;
}
