#pragma once
#include<string>

using namespace std;

struct tree_node;

class SymbolTable {


    tree_node* Root = NULL;

    unsigned int ElementsCount = 0;
    unsigned int Index = 0;

    tree_node* NewNode(string value);

    string recursiveSearch(tree_node* node, int Index);

    void recursiveDelete(tree_node* Node);

public:

    unsigned int count();

    string search_by_index(unsigned int Index);

    int search_by_value(string Value);

    int insert(string value);

    ~SymbolTable();

};