#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int lengthP(Node * node, int height)
{
    // Base case
    if(node == nullptr){
        return height;
    }
    // Root node with no children
    if (node->left == nullptr && node->right == nullptr){
        return height;
    }
    if (node->right == nullptr){
        return lengthP(node->left, height+1);
    }
    // Recurse
    int cLeft = lengthP(node->left, height+1);
    int cRight = lengthP(node->right, height+1);
    if(cLeft == cRight){
        return cLeft;
    }
    else{
        return -1;
    }
}


/*@brief Returns true if all paths from leaves to root are the same length (height),
 *        and false otherwise
 *
 *        Note: this doesn't mean we are checking if the tree is full, but just that
 *        any leaf node (wherever it may exist) has the same length path to the root 
 *        as all others.
 * 
 * @param root Pointer to the root of the tree to check for equal paths
 */
 
bool equalPaths(Node * root)
{
    if(root == nullptr)
    {
        return true;
    }
    // Add your code below
    if (lengthP(root->left, 1) == lengthP(root->right, 1)){
        return true;
    }
    else{
        return false;
    }
}

