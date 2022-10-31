#include "SymbolTable.h"

using namespace std;

struct tree_node {
    string Value;
    unsigned int Index;
    tree_node* LeftChild;
    tree_node* RightChild;
};


string SymbolTable::recursiveSearch(tree_node* node, int Index)
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

    return string{ "" };

}

void SymbolTable::recursiveDelete(tree_node* Node)
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

unsigned int SymbolTable::count()
{
    return ElementsCount;
}

string SymbolTable::search_by_index(unsigned int Index)
{

    if (Root != NULL)
    {
        return recursiveSearch(Root, Index);
    }
    else
    {
        return string{""};
    }
}

int SymbolTable::search_by_value(string Value)
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

int SymbolTable::insert(string value) {

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

SymbolTable::~SymbolTable() {
    recursiveDelete(Root);
}
