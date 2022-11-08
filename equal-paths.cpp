#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int findLeaf(Node* root, int level) {
    if (root==NULL) {
        return true;
    }
    if (root->left==NULL && root->right==NULL) {
        return level;
    }
    return findLeaf(root->left, level+1);
    return findLeaf(root->right, level+1);
}
bool checkLeaves(Node* root, int level, int leaf) {
    if (root==NULL) {
        return true;
    }
    if (root->left==NULL && root->right==NULL) {
        return level==leaf;
    }
    return checkLeaves(root->left, level+1, leaf) && checkLeaves(root->right, level+1, leaf);
}

bool equalPaths(Node * root)
{
    // Add your code below
    int level=0;
    int leaf= findLeaf(root, level);
    return checkLeaves(root, level, leaf);
}
