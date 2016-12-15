#ifndef BUILDTREE_H
#define BUILDTREE_H
#include "node.h"

class BinaryTree
{
  public:
    void Insert(Node *&, Node *&);
    void Search(int);
    void buildTree(string);

    BinaryTree()
    {
      root = NULL;
    }
};

#endif
