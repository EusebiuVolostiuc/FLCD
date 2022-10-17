#include <iostream>

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
        node->Index = Index++;
        node->LeftChild = NULL;
        node->RightChild = NULL;
        return node;
    }

    int recursiveSearch(tree_node* node, const char* value)
    {
        if (node->Value == value)
        {
            return node->Index;
        }
        bool resLeft = 0, resRight = 0;
        if (node->LeftChild != NULL)
        {
            resLeft = recursiveSearch(node->LeftChild, value);
        }
        if (node->RightChild != NULL)
        {
            resRight = recursiveSearch(node->RightChild, value);
        }
        return resLeft || resRight;
    }

public:

    int count()
    {
        return ElementsCount;
    }

    bool contains(const char* Value)
    {
       
        if (Root == NULL)
            return false;

        auto pos = Root;

        while (pos != NULL)
        {
            if (pos->Value == Value)
            {
                return true;
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

        return false;
    }

    int search(const char* Value)
    {
        if (Root == NULL)
            return -1;

        return recursiveSearch(Root, Value);

    }

    int insert(const char* value) {
        auto node = NewNode(value);

        if (node == NULL) {
            return -1;
        }

        if (contains(value) == 1)
        {
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

};


int main()
{
    SymbolTable myTable;
    cout << "Initial Count: " << myTable.count() <<'\n';
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

    if (myTable.contains("a") != 1) {
        cout << "myTable.contains('a') returned false!!\n";
    }
    if (myTable.contains("b") != 1) {
        cout << "myTable.contains('b') returned false!!\n";
    }

}
