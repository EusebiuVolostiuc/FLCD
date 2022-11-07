#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <regex>
#include <vector>

using namespace std;

class SymbolTable {

	struct tree_node {
		string Value;
		unsigned int Index;
		tree_node* LeftChild;
		tree_node* RightChild;
	};

	tree_node* Root = NULL;

	unsigned int ElementsCount = 0;
	unsigned int Index = 0;

	tree_node* NewNode(string value) {
		auto node = new tree_node;
		node->Value = value;
		node->Index = ++Index;
		node->LeftChild = NULL;
		node->RightChild = NULL;
		return node;
	}

	string recursiveSearch(tree_node* node, int Index)
	{

		if (node->Index == Index)
		{
			return node->Value;
		}

		if (node->LeftChild != NULL)
		{
			auto res = recursiveSearch(node->LeftChild, Index);
			if (!res.empty())
				return res;
		}

		if (node->RightChild != NULL)
		{
			auto res = recursiveSearch(node->RightChild, Index);
			if (!res.empty())
				return res;
		}
		return "";
	}

	void recursiveDelete(tree_node* Node)
	{
		if (Node == NULL)
			return;

		if (Node->LeftChild != NULL)
		{
			recursiveDelete(Node->LeftChild);
		}
		if (Node->RightChild != NULL)
		{
			recursiveDelete(Node->RightChild);
		}

		delete Node;
		Node = NULL;
		return;
	}

public:

	unsigned int count()
	{
		return ElementsCount;
	}

	string search_by_index(unsigned int Index)
	{

		if (Root != NULL)
		{
			return recursiveSearch(Root, Index);
		}
		else
		{
			return "";
		}
	}

	int search_by_value(string Value)
	{
		if (Root == NULL)
			return -1;

		auto pos = Root;

		while (pos != NULL)
		{
			if (pos->Value == Value)
			{
				return pos->Index;
			}
			else
			{
				if (pos->Value > Value)
				{
					pos = pos->LeftChild;
				}
				else
				{
					pos = pos->RightChild;
				}
			}
		}
		return -1;

	}

	int insert(string value) {

		if (value.empty() || search_by_value(value) > 0)
			return -1;

		auto node = NewNode(value);

		if (node == NULL) {
			return -1;
		}

		auto pos = Root;

		if (pos == NULL) {
			//if pos == NULL => Root == NULL => node is the first node in the tree
			Root = node;
		}
		else
		{
			auto cursor = pos;

			//search where to place the node
			do
			{
				pos = cursor;
				if (node->Value < pos->Value)
				{
					cursor = pos->LeftChild;
				}
				else
				{
					cursor = pos->RightChild;
				}
			} while (cursor != NULL);

			//pos will be the parent of the new node;
			if (node->Value < pos->Value)
			{
				pos->LeftChild = node;
			}
			else
			{
				pos->RightChild = node;
			}
		}

		ElementsCount++;
		return node->Index;

	}

	void printall()
	{
		for (unsigned int i = 1; i <= Index; i++)
		{
			cout << "ST[" << i << "]: " << search_by_index(i) << '\n';
		}
		cout << endl;
	}

	string toString()
	{
		string s = "";
		for (unsigned int i = 1; i <= Index; i++)
		{
			s += "ST[" + to_string(i) + "]: " + search_by_index(i) + '\n';
		}
		s += '\n';
		return s;
	}
	

	~SymbolTable() {
		recursiveDelete(Root);
	}

};

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
		string token_delim = " ";
		for (int i = 0; i < text.size(); i++)
		{
			if (text[i] == ' ')
			{
				continue;
			}
			else
			if (isdelim(text[i]))
			{
				text.insert(i + 1,token_delim);
				text.insert(i, token_delim);
				i += 2;
			}
			else
			if (text[i] == '\"')
			{
				i++;
				while (i < text.size() and text[i] != '\"')
					i++;
			}
			if (text[i] == '\'')
			{
				i++;
				while (i < text.size() and text[i] != '\'')
					i++;
			}
			else
			if (isspecial(text[i]))
			{
				text.insert(i, token_delim);

				int j = i + 1;
				while (j < text.size() and text[j]!= ' ' and isspecial(text[j]) and !isdelim(text[j]))
				{
					j++;
				}
				if (j < text.size())
					text.insert(j, token_delim);
				i = j;
			}
		}
		regex extra_token_delims(token_delim + "{2,}");
		auto tmp = regex_replace(text, extra_token_delims, token_delim);
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
		//a constant string will be delimited by " "
		return token[0] == '\"' and token[token.size() - 1] == '\"';
	}
	int isConstantChar(string token)
	{
		//a constant string will be delimited by ' '
		if (token[0] == '\'' and token[token.size() - 1] == '\'')
		{
			if (token.size() == 3)
				return 1;
			else 
			if (token.size() == 4)
			{
				if (token[1] == '\\')
					return 1;
				else
					return 2;
			}
			else
				return 2;
		}
		return 0;
	}

	int isConstantInt(string token)
	{
		if (token == "0")
			return 1;
		if(token[0] == '+' || token[0] == '-' )
		{
			if (token[1] == '0')
				return 2;

			for(int i = 2; i < token.size(); i++)
			{
				if (!isdigit(token[i]))
					return 0;
			}

			if (token[0] == '+')
				return 2;
		}
		else {
			
			for (int i = 0; i < token.size(); i++)
			{
				if (!isdigit(token[i]))
					return 0;
			}
			if (token[0] == '0')
				return 2;
		}
		
		return true;
	}

	int isIdentifier(string candidate)
	{

		for (auto c : candidate)
		{
			if (isspecial(c))
				return 0;
		}

		if (isdigit(candidate[0]))
			return 2;

		return 1;
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
				size_t pos2 = text.find('\"', pos + 1);
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
			else
			if (token[0] == '\'')
			{
				size_t pos2 = text.find('\'', 1);
				if (pos2 != std::string::npos)
				{
					token = text.substr(0, pos2 + 1);
					pos = pos2 + 1;
				}
				else
				{
					token = text.substr(0, pos2);
					pos = pos2;
				}
			}

			if(!token.empty())
				tokens.push_back(token);

			if (pos != std::string::npos)
				text.erase(0, pos + 1);
			else
				text.erase(0, pos);
		}
		if(!text.empty()) tokens.push_back(text);

		for (int i = 0; i < tokens.size(); i++)
		{
			if (tokens[i] == "+" || tokens[i] == "-" and (i+1)<tokens.size())
			{
				if (i > 0 and (isOperator(tokens[i - 1]) || isSeparator(tokens[i-1])))
				{
					tokens[i] += tokens[i + 1];
					tokens.erase(tokens.begin() + i + 1);
				}
			}
		}


		return tokens;
	}

	vector<pair<string, int>> classify(vector<string> tokens, SymbolTable &ST)
	{
		vector<pair<string, int>> PIF;
		for (auto tokens_it = tokens.begin(); tokens_it != tokens.end(); tokens_it++)
		{
			if (isKeyword(*tokens_it) || isOperator(*tokens_it) || isSeparator(*tokens_it))
			{
				PIF.push_back(make_pair(*tokens_it, 0));
			}
			else if (isConstantString(*tokens_it) || isConstantChar(*tokens_it) || isConstantInt(*tokens_it))
			{
				if (isConstantChar(*tokens_it) == 2)
				{
					throw* tokens_it + " is not a valid char constant";
				}

				if (isConstantInt(*tokens_it) == 2)
				{
					throw* tokens_it + " is not a valid integer";
				}
				
				auto str = *tokens_it;
				int index = ST.search_by_value(str);
				if (index == -1)
				{
					index = ST.insert(str);
				}
			    PIF.push_back(make_pair("CONST", index));
			}
			else if (isIdentifier(*tokens_it))
			{
				if (isIdentifier(*tokens_it) == 2)
				{
					throw* tokens_it + " is not a valid identifier";
				}
				auto str = *tokens_it;
				int index = ST.search_by_value(str);
				if (index == -1)
				{
					index = ST.insert(str);
				}
				PIF.push_back(make_pair("ID", index));

			}
			else
			{
				throw "Cannot clasify token " + *tokens_it;
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

	vector<pair<string,int>> run(const char* filename, SymbolTable &ST)
	{
		ifstream f(filename);
		string code_line;
		int line_count = 0;
		vector<pair<string, int>> PIF;
		while (getline(f, code_line))
		{
			line_count++;
			string formatted_text = preprocess(code_line);

			vector<string> tokens = tokenize(formatted_text);

			try {
				vector<pair<string, int>> Line_PIF = classify(tokens, ST);
				for (auto token : Line_PIF)
				{
					PIF.emplace_back(token);
				}
			}
			catch (string e) {
				cout << "[Scanner Error]: "<< e << " | Line: " << line_count;
				ST = SymbolTable{};
				return  vector<pair<string, int>>{};

			}
		}
		return PIF;
	}
};

void write_PIF(vector<pair<string, int>> &PIF)
{
	ofstream fout("PIF.out");
	for (auto x : PIF)
	{
		fout << "( " << x.first << " , " << x.second << " )\n";
	}
	fout.close();
}

void write_ST(SymbolTable &ST)
{
	ofstream fout("ST.out");
	fout << ST.toString();
	fout.close();
}

int main()
{
	Scanner s;
	SymbolTable ST;

	vector<pair<string, int>> PIF = s.run("P1.txt",ST);

	write_PIF(PIF);

	write_ST(ST);

	return 0;
}
