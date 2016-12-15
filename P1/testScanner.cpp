//Brady Kedge
//CS4280 - P1

#include "testScanner.h"

using namespace std;

//Matches names with enum of tokens
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

//Call scan until end of file
void testScanner(FILE *fp)
{
  token Tkn;
  int linenumber = 1;
  int lookahead;
  cout << "Working\n";

  while((lookahead = fgetc(fp)) != EOF)
  {
    ungetc(lookahead, fp);

    Tkn = scan(fp, linenumber);

    if(Tkn.tokenID != ERROR_Tk)
    {
      printTkn(Tkn);
    }
    else
    {
      printTkn(Tkn);
      return;
    }
  }

  fclose(fp);
}

//Prints token
void printTkn(token Tkn)
{
  cout << "Line number: " << Tkn.lineNum << ", " << "Token ID: " << tokenNames[Tkn.tokenID] << ", " << "Instance: " << Tkn.instance << endl;
}
