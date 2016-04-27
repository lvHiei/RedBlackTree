#include <iostream>
#include <vector>
#include <cassert>

#include "Tree.h"
#include "Tree.hpp"

using namespace std;

int main()
{
    int cases[] = {12,1,9,2,0,11,7,19,4,15,18,5,14,13,10,16,6,3,8,17};
    
    RedBlackTree<int, int> myTree;

    for (int i = 0; i < sizeof(cases) / sizeof(int); ++i)
    {
        myTree.insertToTree(cases[i], cases[i]);
        cout << "after insert " << cases[i] << " : " << endl;
        myTree.print();
        cout << endl;
    }
    
    for (int i = 0; i < sizeof(cases) / sizeof(int); ++i)
    {
        myTree.deleteFromTree(cases[i]);
        cout << "after delete " << cases[i] << " : " << endl;
        myTree.print();
        cout << endl;
    }

    return 0;
}