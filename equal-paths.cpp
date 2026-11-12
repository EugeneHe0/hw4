#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool helper(Node* root, int depth, int& leafDepth);

bool equalPaths(Node * root)
{
    // Add your code below
    int leafDepth = -1;
    return helper(root, 0, leafDepth);
}

bool helper(Node* root, int depth, int& leafDepth)
{
    if(root == nullptr)
    {
        return true;
    }

    // If leaf
    if(root->left == nullptr && root->right == nullptr)
    {
        // 1st leaf found
        if(leafDepth == -1)
        {
            leafDepth = depth;
            return true;
        }

        // all other leaf must at the same depth
        return depth == leafDepth;
    }

    return helper(root->left, depth + 1, leafDepth) && helper(root->right, depth + 1, leafDepth);
}