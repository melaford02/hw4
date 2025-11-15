//pasted from my vscode to run tests

#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int pathLength(Node* n)
{
    if(n == 0){
      return 0;  
    } 

    int a = pathLength(n->left);
    int b = pathLength(n->right);


    if(a > b){
        return a+1;
    }
    else
    {
       return b+1;
}
}


bool equalPaths(Node* n)
{
    if(n == 0)
    {
        return true;
    }

    if(n->left == 0 && n->right == 0)
    {
        return true;
    }

    if(n->left == 0)
    {
        return equalPaths(n->right) && (pathLength(n->right->left) == pathLength(n->right->right));
    }

    if(n->right == 0)
    {
        return equalPaths(n->left) && (pathLength(n->left->left) == pathLength(n->left->right));
    }

    int l = pathLength(n->left);
    int r = pathLength(n->right);

    //comp both sides
    if(l != r)
    {
        return false;
    }

    bool okL = equalPaths(n->left);
    bool okR = equalPaths(n->right);

    if(okL && okR)
    {
        return true;
    }
    return false;
}