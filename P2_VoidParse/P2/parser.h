#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "scanner.h"
#include "testScanner.h"
#include "parser.h"
#include "node.h"

using namespace std;

extern FILE *fp;
extern int linenumber;

node_t *makeNode(string label);
void error(string errorString);
void parser();
void program();
void block();
void vars();
void mvars();
void expr();
void M();
void T();
void F();
void R();
void stats();
void mStat();
void stat();
void in();
void out();
void ifFunction();
void loopFunction();
void assignFunction();
void RO();

#endif
