#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <regex>
#include <vector>

using namespace std;

class Scanner {
private:
	set<string> reservedWords;
	set<char> delimiters;
	set<string> operators;
	string code_text;

	void load_reserved_words()
	{
		ifstream f("Reserved_Words.in");
		string word;
		while (f >> word)
		{
			reservedWords.insert(word);
		}
		f.close();
	}

	void load_delimiters()
	{
		ifstream f("Delimiters.in");
		char chr;
		while (f >> chr)
		{
			delimiters.insert(chr);
		}
		f.close();
	}

	void load_operators()
	{
		ifstream f("Operators.in");
		string word;
		while (f >> word)
		{
			operators.insert(word);
		}
		f.close();
	}

	bool isspecial(char c)
	{
		return !(isalnum(c) || c == '_');
	}

	bool isdelim(char c)
	{
		return delimiters.find(c) != delimiters.end();
	}

	string preprocess(string text)
	{

		for (int i = 0; i < text.size(); i++)
		{
			if (isdelim(text[i]))
			{
				text.insert(i + 1, " ");
				text.insert(i, " ");
				i += 2;
			}
			else
			if (text[i] == '\"')
			{
				i++;
				while (i < text.size() and text[i] != '\"')
					i++;
			}
			else
			if (isspecial(text[i]))
			{
				text.insert(i, " ");

				int j = i + 1;
				while (j < text.size() and isspecial(text[j]) and !isdelim(text[j]))
				{
					j++;
				}
				if (j < text.size())
					text.insert(j, " ");
				i = j;
			}
		}
		regex extra_spaces(" {2,}");
		auto tmp = regex_replace(text, extra_spaces, " ");


		return tmp;

	}

	bool isKeyword(string candidate)
	{
		return reservedWords.find(candidate) != reservedWords.end();
	}

	bool isOperator(string candidate)
	{
		return operators.find(candidate) != operators.end();
	}

	bool isSeparator(string token)
	{
		char c = token[0];
		return isdelim(c);
	}

	bool isConstantString(string token)
	{
		return token[0] == '\"' and token[token.size() - 1] == '\"';
	}
	bool isConstantInt(string candidate)
	{
		for (auto c : candidate)
		{
			if (!isdigit(c))
				return false;
		}
		return true;
	}

	bool isIdentifier(string candidate)
	{
		for (auto c : candidate)
		{
			if (isspecial(c))
				return false;
		}
		return true;
	}


	vector<string> tokenize(string text)
	{
		vector<string> tokens;
		size_t pos = 0;
		string token;
		bool inString = 0;
		string const_string_token;
		while ((pos = text.find(' ')) != std::string::npos)
		{
			
			token = text.substr(0, pos);

			if (token[0] == '\"')
			{
				int pos2 = text.find('\"', pos + 1);
				if (pos2 != std::string::npos)
				{
					token = text.substr(0, pos2 + 1);
					pos = pos2+1;
				}
				else
				{
					token = text.substr(0, pos2);
					pos = pos2;
				}
			}

			tokens.push_back(token);

			if (pos != std::string::npos)
				text.erase(0, pos + 1);
			else
				text.erase(0, pos);
		}
		if(!text.empty()) tokens.push_back(text);
		return tokens;
	}

	vector<pair<string, int>> classify(vector<string> tokens)
	{
		vector<pair<string, int>> PIF;
		for (auto tokens_it = tokens.begin(); tokens_it != tokens.end(); tokens_it++)
		{
			if (isKeyword(*tokens_it) || isOperator(*tokens_it) || isSeparator(*tokens_it))
			{
				PIF.push_back(make_pair(*tokens_it, 0));
			}
			else if (isConstantString(*tokens_it) || isIdentifier(*tokens_it) || isConstantInt(*tokens_it))
			{
				PIF.push_back(make_pair(*tokens_it, 1));
			}
			else
			{
				cout << "[Scanner Error]: Cannot clasify token " << *tokens_it << '\n';
			}
		}
		return PIF;
	}

public:

	Scanner()
	{
		load_reserved_words();
		load_delimiters();
		load_operators();
	}

	int load_code(const char* filename)
	{
		string text, line;
		ifstream f(filename);
		while (getline(f, line))
		{
			text.append(line);
			text.append(" ");
		}
		f.close();
		this->code_text = text;
		return 0;
	}

	vector<pair<string,int>> run()
	{

		string formatted_text = preprocess(code_text);

	    vector<string> tokens = tokenize(formatted_text);

		/*for (int i = 0; i < tokens.size(); i++)
		{
			cout << "Token #" << i << ": " << tokens[i] << '\n';
		}*/

		vector<pair<string, int>> PIF = classify(tokens);

		return PIF;

	}
};

int main()
{
	Scanner s;
	s.load_code("P1.txt");
	vector<pair<string, int>> PIF = s.run();

	for (auto x : PIF)
	{
		cout << "("<<x.first<<", "<<x.second<<")\n";
	}
	return 0;
}
