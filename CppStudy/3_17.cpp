#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

int main()
{
    vector<string> words;
    
    cout << "������ʣ�ÿ��һ�����������ֹͣ��" << endl;

    string line;
    while(getline(cin, line) && !line.empty())
    {
        words.push_back(line);
    }
    
    cout << "��д��Ľ����" << endl;
    for(vector<string>::iterator it = words.begin(); it != words.end(); ++it)
    {
        for(string::iterator ch = it->begin(); ch != it->end(); ++ch)
        {
            *ch = toupper(*ch);
        }
        cout << *it << endl;
    }
    
    return 0;
}
