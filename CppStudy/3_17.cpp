#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

int main()
{
    vector<string> words;
    
    cout << "请输入词，每行一个，输入空行停止：" << endl;

    string line;
    while(getline(cin, line) && !line.empty())
    {
        words.push_back(line);
    }
    
    cout << "改写后的结果：" << endl;
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
