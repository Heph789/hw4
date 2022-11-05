#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool equalPathsHelper(Node * root, int & firstHeight, int curHeight)
{
    if (root->right == nullptr && root->left == nullptr) // has no children
    {
        if (firstHeight == 0) {
            firstHeight = curHeight; // NOTE: make sure this is not a problem
            return true;
        }
        return firstHeight == curHeight;
    }
    
    if (root->right != nullptr) // has a right child
    {
        if (!equalPathsHelper(root->right, firstHeight, curHeight+1)) return false;
    }
    if (root->left != nullptr)
    {
        if (!equalPathsHelper(root->left, firstHeight, curHeight+1)) return false;
    }
    return true;
}

bool equalPaths(Node * root)
{
    // Add your code below
    // basically, we're going to traverse down the left side until we find a leaf (while keeping count of height)
    // and then we're going to backtrack to get the next leaf, and compare it to the first height.
    // The first time we find a mismatch in height, we return false. If we never find said mismatch, we return true.
    int firstHeight = 0;
    if (root == nullptr) return true;
    return equalPathsHelper(root, firstHeight, 1);
}