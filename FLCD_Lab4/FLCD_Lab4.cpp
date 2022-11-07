#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class FA {

    vector<string> Q;
    /*vector<string> E;
    vector<string> F;
    vector<tuple<string, string, string>> S;*/

    void readQ(string line)
    {
        stringstream ss;

        int pos = line.find("{");
        line = line.erase(0, pos);
        ss << line;

        string x;
        ss >> x;
        while (x != "}")
        {
            ss >> x;
            Q.push_back(x);
            ss >> x;
        }
    }

public:
    FA()
    {
        ifstream f("FA.in");
        string line;
        while (getline(f, line))
        {
            if (line.find("Q") != std::string::npos)
            {
                readQ(line);
            }
        }
        f.close();
    }

};
int main()
{
    FA fa;
    return 0;
}


