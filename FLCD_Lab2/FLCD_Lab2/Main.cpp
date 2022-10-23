#include <iostream>
#include <assert.h>

using namespace std;

class SymbolTable {

    struct tree_node {
        const char* Value;
        unsigned int Index;
        tree_node* LeftChild;
        tree_node* RightChild;
    };

    tree_node* Root = NULL;

    unsigned int ElementsCount = 0;
    unsigned int Index = 0;

    tree_node* NewNode(const char* value) {
        auto node = new tree_node;
        node->Value = value;
        node->Index = ++Index;
        node->LeftChild = NULL;
        node->RightChild = NULL;
        return node;
    }

    const char* recursiveSearch(tree_node* node, int Index)
    {

        if (node->Index == Index)
        {
            return node->Value;
        }

        if (node->LeftChild != NULL)
        {
            auto res = recursiveSearch(node->LeftChild, Index);
            if (strlen(res) > 0)
                return res;
        }

        if (node->RightChild != NULL)
        {
            auto res = recursiveSearch(node->RightChild, Index);
            if (strlen(res) > 0)
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

    const char* search_by_index(unsigned int Index)
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

    int search_by_value(const char* Value)
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

    int insert(const char* value) {

        if (strlen(value) == 0 || search_by_value(value) > 0)
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

    ~SymbolTable() {
        recursiveDelete(Root);
    }

};


int main()
{
    SymbolTable myTable;
    cout << "Initial Count: " << myTable.count() << '\n';
    int addrA = myTable.insert("a");
    if (addrA < 0)
    {
        cout << "Could not add 'a' to symbol table!!\n";
    }
    else
    {
        cout << "Added 'a' to symbol table with index " << addrA << "\n";
    }
    cout << "Count: " << myTable.count() << '\n';
    int addrB = myTable.insert("b");
    if (addrB < 0)
    {
        cout << "Could not add 'b' to symbol table!! \n";
    }
    else
    {
        cout << "Added 'b' to symbol table with index " << addrB << "\n";

    }
    cout << "Count: " << myTable.count() << '\n';

    int addrC = myTable.insert("a");
    if (addrC < 0) {
        cout << "Could not add 'a' to symbol table!! ('a' is already in symbol table)\n";
    }
    cout << "Count: " << myTable.count() << '\n';

    auto posA = myTable.search_by_value("a");
    auto posB = myTable.search_by_value("b");
    cout << "Position of 'a' : " << posA << "\n";
    cout << "Position of 'b' : " << posB << "\n";
    cout << "Position of 'c' : " << myTable.search_by_value("c") << "\n";

    cout << "Value at " << posA << ": " << myTable.search_by_index(posA)<<"\n";
    cout << "Value at " << posB << ": " << myTable.search_by_index(posB) <<"\n";
    cout << "Value at " << 3 << ": " << myTable.search_by_index(3) << "\n";

}
