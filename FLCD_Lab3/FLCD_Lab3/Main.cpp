#include <iostream>
#include <fstream>
#include <vector>
#include <set>
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
				if (pos->Index > Index)
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
		for (int i = 1; i <= Index; i++)
		{
			cout << "ST[" << i << "]: " << search_by_index(i) << '\n';
		}
		cout << endl;
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

	bool isConstantInt(string token)
	{
		if (token == "0")
			return true;
		if(token[0] == '+' || token[0] == '-' )
		{
			if (token[1] == '0')
				return false;

			for (int i = 2; i > token.size(); i++)
			{
				if (!isdigit(token[i]))
					return false;
			}
		}
		else {
			if (token[0] == '0')
				return false;

			for (int i = 1; i > token.size(); i++)
			{
				if (!isdigit(token[i]))
					return false;
			}
		}
		
		return true;
	}

	bool isIdentifier(string candidate)
	{

		if(isdigit(candidate[0]))
			return false;

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
				if (i > 0 and isOperator(tokens[i - 1]))
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
			else if (isConstantString(*tokens_it) || isIdentifier(*tokens_it) || isConstantInt(*tokens_it))
			{

				auto str = *tokens_it;
				int index = ST.search_by_value(str);
				if (index == -1)
				{
					index = ST.insert(str);
				}
				if (isIdentifier(*tokens_it))
				{
					PIF.push_back(make_pair("ID", index));
				}
				else
				{
					PIF.push_back(make_pair("CONST", index));
				}
			}
			else
			{
				string msg = "[Scanner Error]: Cannot clasify token " + *tokens_it;
				throw msg;
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

	vector<pair<string,int>> run(SymbolTable &ST)
	{

		string formatted_text = preprocess(code_text);

	    vector<string> tokens = tokenize(formatted_text);

		try {
			vector<pair<string, int>> PIF = classify(tokens, ST);
			return PIF;
		}
		catch(string e) {
			cout << e;
			ST = SymbolTable{};
			return  vector<pair<string, int>>{};

		}
	}
};

void write_PIF(vector<pair<string, int>> PIF)
{
	ofstream fout("PIF.out");
	for (auto x : PIF)
	{
		fout << "(" << x.first << ", " << x.second << ")\n";
	}
	fout.close();
}

int main()
{
	Scanner s;
	SymbolTable ST;
	s.load_code("P1.txt");
	vector<pair<string, int>> PIF = s.run(ST);

	/*for (auto x : PIF)
	{
		cout << "("<<x.first<<", "<<x.second<<")\n";
	}
	cout << endl;*/

	write_PIF(PIF);

	ST.printall();

	return 0;
}
