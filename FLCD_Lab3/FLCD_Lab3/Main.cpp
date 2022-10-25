#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

class Scanner {

	static vector<string> get_statements(string text) {
		vector<string> statements;
		size_t pos = 0;
		std::string token;
		while ((pos = text.find(";")) != std::string::npos) {
			token = text.substr(0, pos);
			statements.push_back(token);
			text.erase(0, pos + 1);
		}
		return statements;
	}

	static vector<string> get_nondelimiters(string statement)
	{
		set<string> reserved_words = { "BEGIN","END","var","if","else","while","int"};
		set<char> delimiters = { '(',')','{','}',',',':'};

		vector<string> non_delimiters;
		size_t pos = 0;
		std::string token;
		while (pos < statement.size()) 
		{
			token = statement.substr(0, pos);

			if (reserved_words.find(token) != reserved_words.end())
			{
				non_delimiters.push_back(token);
				statement.erase(0, token.length());
				pos = 0;
			}
			else 
			{
				if (delimiters.find(statement.at(pos)) != delimiters.end())
				{
					if (token.length() > 0)
					{
						non_delimiters.push_back(token);
						statement.erase(0, token.length());
					}	
					token = statement.substr(0, 1);
					non_delimiters.push_back(token);
					statement.erase(0, 1);
					pos = 0;
				}
				else
				{
					pos++;
				}
			}
		}
		non_delimiters.push_back(statement);
		return non_delimiters;
	}
public:
	static void scan(const char* filename) {
		ifstream f("P1.txt");
		char s[255] = { 0 };
		string program_text;
		while (!f.eof())
		{
			f >> s;
			program_text.append(s);
		}
		f.close();

		auto statements = get_statements(program_text);

		for (auto statement : statements)
		{
			std::cout << "$ " << statement << " $\n";
			auto tokens = get_nondelimiters(statement);
			for (auto nd : tokens)
			{
				std::cout << "% " << nd << " % \n ";
			}
			std::cout << "\n\n";
		}
	}
};

int main()
{
	Scanner::scan("P1.txt");
	return 0;
}