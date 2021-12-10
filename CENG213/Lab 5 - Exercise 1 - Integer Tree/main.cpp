#include <iostream>

#include "IntTree.h"

using namespace std;

int main() {
    
    cout << "Crating a random full tree of height 2." << endl;
    
    int height = 2;
    IntTree myFullTree(height);
    
    cout << "Inorder traversal: " << endl;
    
    myFullTree.print();
    
    cout << "Full tree sideways: " << endl;
    
    myFullTree.printSideways();

    cout << "Height of the full binary tree is " << myFullTree.height() << endl;

    int value = 8;
    cout << "Does the tree contains the value " << value << "? " << (myFullTree.contains(value) ? "yes" : "no") << endl;

    // You can write code to test other functions ...

    return 0;
}
