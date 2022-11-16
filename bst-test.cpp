#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // Binary Search Tree tests
    // BinarySearchTree<char,int> bt;
    // bt.insert(std::make_pair('a',1));
    // cout << "Is balanced: " << bt.isBalanced() << endl;
    // bt.insert(std::make_pair('b',2));
    
    // cout << "Binary Search Tree contents:" << endl;
    // for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
    //     cout << it->first << " " << it->second << endl;
    // }
    // if(bt.find('b') != bt.end()) {
    //     cout << "Found b" << endl;
    // }
    // else {
    //     cout << "Did not find b" << endl;
    // }
    // cout << "Erasing b" << endl;
    // bt.remove('b');
    // cout << "Is balanced: " << bt.isBalanced() << endl;

    // AVL Tree Tests
    AVLTree<char,int> at;
    at.insert(make_pair('a', 2));
    at.insert(make_pair('b', 3));
    // at.insert(std::make_pair(5,1));
    // at.insert(std::make_pair(2,2));
    // at.insert(std::make_pair(3,2));
    // at.insert(std::make_pair(11,2));
    // at.insert(std::make_pair(6,2));
    // at.insert(std::make_pair(7,2));
    // at.insert(std::make_pair(9,2));

    // cout << "Is balanced: " << at.isBalanced() << endl;

    cout << "\nAVLTree contents:" << endl;
    for(AVLTree<char,int>::iterator it = at.begin(); it != at.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(at.find('b') != at.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    at.remove('b');

    return 0;
}
