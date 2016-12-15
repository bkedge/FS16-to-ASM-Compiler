#ifndef NODE_H
#define NODE_H

struct Node
{
  int val;
  int count;
  int level;
  struct Node *left;
  struct Node *right;
};

#endif
