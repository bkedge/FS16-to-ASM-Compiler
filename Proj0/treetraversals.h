#ifndef TREETRAVERSALS_H
#define TREETRAVERSALS_H
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include "node.h"
#include "buildtree.h"


void Inorder(Node *root, ostream &os);
void Postorder(Node *root, ostream &os);
void Preorder(Node *root, ostream &os);

void writeInorder(Node *root, string filename);
void writePostorder(Node *root, string filename);
void writePreorder(Node *root, string filename);

#endif
