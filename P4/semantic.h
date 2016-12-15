#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "node.h"
#include "treePrint.h"

#include <vector>
#include <sstream>

void push(token tk);
void pop(int scopeStart);
int find(string var);
void semGen(node_t *node, int varCt);
void setupASM(FILE *of);
string newTemp();
string newLabel();

#endif
