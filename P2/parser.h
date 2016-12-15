#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "scanner.h"
#include "node.h"

using namespace std;

extern FILE *fp;
//extern int linenumber;

node_t *makeNode(string label, int level);
void error(string errorString);
node_t *parser();
node_t *program();
node_t *block(int level);
node_t *vars(int level);
node_t *mvars(int level);
node_t *expr(int level);
node_t *M(int level);
node_t *T(int level);
node_t *F(int level);
node_t *R(int level);
node_t *stats(int level);
node_t *mStat(int level);
node_t *stat(int level);
node_t *in(int level);
node_t *out(int level);
node_t *ifFunction(int level);
node_t *loopFunction(int level);
node_t *assignFunction(int level);
node_t *RO(int level);

#endif
