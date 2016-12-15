#ifndef BUILDTREE_H
#define BUILDTREE_H
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "node.h"

using namespace std;

Node *BuildTree(string filename);
Node *Insert(Node *root, int key);
void getLevels(Node *root, int level);


#endif
