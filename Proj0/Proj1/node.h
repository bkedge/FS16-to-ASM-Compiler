#ifndef NODE_H
#define NODE_H

struct Node
{
  int val;
  int count;
  int level;
  Node *left;
  Node *right;
}*root;

#endif
