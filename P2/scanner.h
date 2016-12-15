#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
//For mapping of tokens
typedef struct
{
  int key;
  tokenId name;
}tokenMap;

//For mapping of alphabet
struct alphabetMap
{
  int key;
  int value;
};

int columnMatch(char lookahead);
token tokenMatch(int value, string word);
token scan(FILE *fp, int &linenumber);

#endif
