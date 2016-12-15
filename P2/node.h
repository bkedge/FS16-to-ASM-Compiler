#ifndef NODE_H
#define NODE_H

#include "token.h"


using namespace std;

struct node_t
{
  string label;
  int level;
  struct node_t *child1;
  struct node_t *child2;
  struct node_t *child3;
  struct node_t *child4;
  token nodeToken;
  token tempToken;
};

#endif
