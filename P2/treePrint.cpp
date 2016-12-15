#include "token.h"
#include "scanner.h"
#include "node.h"

#include <string>
#include <iostream>

using namespace std;

string tokenNames[] =
{
  "Identifier", "Number", "End of File", "Error token",

  "Begin Keyword", "End Keyword", "Start Keyword", "Stop Keyword", "IFF Keyword", "Loop Keyword",
  "Void Keyword", "Var Keyword", "Int Keyword", "Call Keyword", "Return Keyword", "Scan Keyword",
  "Print Keyword", "Program Keyword",

  "Equals operator", "Less than operator", "Greater than operator", "Not equals operator",
  "Greater than or equal operator", "less than or equal operator", "Double equals operator",

  "Colon operator", "Plus operator", "Minus operator", "Asterisk operator", "Slash operator",
  "Ampersand operator", "Percent operator",

  "Period delimiter", "Left parentheses", "Right parantheses", "Comma delimiter", "Left brace",
  "Right brace", "SemiColon", "Left Bracket", "Right Bracket"

};

void treePrint(node_t *tree)
{
  if(tree == NULL)
  {
    return;
  }
  else
  {
    for(int i = 0; i < tree->level; i++)
    {
      cout << "  ";
    }

    cout << tree->label << "\t";
    token treeTk = tree->nodeToken;

    if(tree->tempToken.tokenID == DoubleEquals_Tk)
    {
      cout << "Token ID: " << tokenNames[treeTk.tokenID] << " Instance: " << treeTk.instance << " Line Number: " << treeTk.lineNum << "\t"
           << " Token 2 ID: " << tokenNames[tree->tempToken.tokenID] << " Instance: " << tree->tempToken.instance << " Line number: " << tree->tempToken.lineNum;
    }
    else if(treeTk.instance != "")
    {
      cout << "Token ID: " + tokenNames[treeTk.tokenID] + " Instance: " + treeTk.instance + " Line Number: " << treeTk.lineNum;

    }

    cout << "\n";

    if(tree->child1 != NULL)
    {
      treePrint(tree->child1);
    }
    if(tree->child2 != NULL)
    {
      treePrint(tree->child2);
    }
    if(tree->child3 != NULL)
    {
      treePrint(tree->child3);
    }
    if(tree->child4 != NULL)
    {
      treePrint(tree->child4);
    }

  }
}
